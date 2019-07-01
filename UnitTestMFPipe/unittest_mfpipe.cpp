#include "stdafx.h"
#include "CppUnitTest.h"
#include "../MFPipeTest/MIDL32/MFormats.h"
#include "../MFPipeTest/MIDL32/MFPipeTest_i.h"

#include <string>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define PACKETS_COUNT	(8)
#define DS_MSEC			(REFERENCE_TIME)10000L		// 1 ms in 100 ns DirectShow units

#ifndef SIZEOF_ARRAY
#define SIZEOF_ARRAY(arr)	(sizeof(arr)/sizeof((arr)[0]))
#endif // SIZEOF_ARRAY

namespace UnitTestMFPipe
{		
	TEST_CLASS(UnitTest_MFPipe)
	{
	public:
		
		// Note: 
		// - Code below contain few errors, please fix it's
		// - This is only examples of object usage and tests, refactoring is welcome 
		// - Multithreaded test required
		// - More tests is welcome
		
		TEST_METHOD(TestMethod1)
		{
			//////////////////////////////////////////////////////////////////////////
			// MFBuffer Allocations

			HRESULT __h;

			CComPtr<IMFBuffersAllocator> cpMFBufferAllocator;
			__h = cpMFBufferAllocator.CoCreateInstance(__uuidof(MFFactory));
			Assert::IsNotNull(cpMFBufferAllocator.p);

			// Allocation
			CComPtr<IMFBuffer> arrMFBuffersIn[PACKETS_COUNT];
			for (int i = 0; i < SIZEOF_ARRAY(arrMFBuffersIn); ++i)
			{
				DWORD cbSize = 128 * 1024 + rand() % (256 * 1024);
				cpMFBufferAllocator->MFBufferAlloc(cbSize, NULL, &arrMFBuffersIn[i], eMFBF_Empty);
				Assert::IsNotNull(arrMFBuffersIn[i].p);

				// Fill buffer
				LONG cbMaxSize = 0, cbActualSize = cbSize;
				LONGLONG lpData = NULL;
				arrMFBuffersIn[i]->BufferLock(eMFLT_NoLock, &cbMaxSize, &cbActualSize, &lpData);

				LPBYTE _q = (LPBYTE)lpData;

				_q[0] = 't';
				_q[1] = 'e';
				_q[2] = 's';
				_q[3] = 't';

				for (DWORD i = 4; i < cbSize; i++)
					_q[i] = rand() % 0xFF;
			}

			//////////////////////////////////////////////////////////////////////////
			// MFFrame allocation

			CComPtr<IMFFactory> cpMFFactory;
			__h = cpMFFactory.CoCreateInstance(__uuidof(MFFactory));
			Assert::IsNotNull(cpMFFactory.p);

			eMVideoFormat arrFormats[] = { eMVF_PAL, eMVF_NTSC, eMVF_HD1080_25p, eMVF_HD720_60p, eMVF_HD1080_60i, eMVF_NTSC_16x9, eMVF_HD1080_5994i, eMVF_HD720_60p };
			M_AV_PROPS avProps = {};
			avProps.audProps.nChannels = 4;
			avProps.audProps.nSamplesPerSec = 48000;
			avProps.audProps.nBitsPerSample = 32; // 32-bit float

			// Allocation
			CComPtr<IMFFrame> arrMFFrames[PACKETS_COUNT];
			for (int i = 0; i < SIZEOF_ARRAY(arrMFFrames); ++i)
			{
				LONG lAudioSamples = rand() % 32 * 256;
				avProps.vidProps.eVideoFormat = arrFormats[i % PACKETS_COUNT];
				cpMFFactory->MFFrameCreateFromMem(&avProps, NULL, lAudioSamples, NULL, &arrMFFrames[i], NULL);
				

				// Fill MFFrame by test data
				MF_FRAME_INFO mfInfo = {};
				arrMFFrames[i]->MFAllGet(&mfInfo);

				// Fill audio/video
				// Audio data ptr: (LPBYTE)mfInfo.lpAudio, size: mfInfo.cbAudio
				// Video data ptr: (LPBYTE)mfInfo.lpVideo, size: mfInfo.cbVideo

				LPBYTE _q = (LPBYTE)mfInfo.lpAudio;
				for (DWORD i = 0; i < mfInfo.cbAudio; i++)
					_q[i] = rand() % 0xFF;

				LPBYTE _w = (LPBYTE)mfInfo.lpVideo;
				for (DWORD i = 0; i < mfInfo.cbVideo; i++)
					_w[i] = rand() % 0xFF;

				M_TIME mTime = {};
				mTime.rtStartTime = i * 100 * DS_MSEC;
				mTime.rtEndTime = (i + 1) * 100 * DS_MSEC;
				arrMFFrames[i]->MFTimeSet(&mTime);
			}

			CComBSTR ppszEvents[] = { L"event1", L"event2", L"event3", L"event4", L"event5", L"event6", L"event7", L"event8", NULL };
			CComBSTR ppszMessages[] = { L"message1", L"message2", L"message3", L"message4", L"message5", L"message6", L"message7", L"message8", NULL };
			

			//////////////////////////////////////////////////////////////////////////
			// Pipe creation

			// Write pipe
			CComPtr<IMFPipe> cpMFPipe_Write;
			cpMFPipe_Write.CoCreateInstance(__uuidof(CoMFPipeTest));
			Assert::IsNotNull(cpMFPipe_Write.p);
			cpMFPipe_Write->PipeCreate(L"udp://127.0.0.1:12345", NULL);

			// Read pipe
			CComPtr<IMFPipe> cpMFPipe_Read;
			cpMFPipe_Read.CoCreateInstance(__uuidof(CoMFPipeTest));
			Assert::IsNotNull(cpMFPipe_Read.p);
			cpMFPipe_Read->PipeOpen(L"udp://127.0.0.1:12345", 32, NULL);

			for (int i = 0; i < 2; ++i)
			{
				CComPtr<IMFBuffer> bufferIn = arrMFBuffersIn[i % PACKETS_COUNT];
				
				// Write to pipe
				cpMFPipe_Write->PipePut(L"ch1", bufferIn, 0, NULL);
				cpMFPipe_Write->PipePut(L"ch2", arrMFBuffersIn[(i + 1) % PACKETS_COUNT], 0, NULL);
				cpMFPipe_Write->PipeMessagePut(L"ch1", ppszEvents[i % PACKETS_COUNT], ppszMessages[i % PACKETS_COUNT], arrMFBuffersIn[i % PACKETS_COUNT], NULL);
				cpMFPipe_Write->PipeMessagePut(L"ch2", ppszEvents[(i + 1) % PACKETS_COUNT], ppszMessages[(i + 1) % PACKETS_COUNT], arrMFBuffersIn[(i + 1) % PACKETS_COUNT], NULL);

				cpMFPipe_Write->PipePut(L"ch1", arrMFFrames[i % PACKETS_COUNT], 0, NULL);
				cpMFPipe_Write->PipePut(L"ch2", arrMFFrames[(i + 1) % PACKETS_COUNT], 0, NULL);

				BSTR bsPipeName = NULL;
				cpMFPipe_Write->PipeInfoGet(&bsPipeName, NULL, NULL);

				MF_PIPE_INFO mfInfo = {};
				cpMFPipe_Write->PipeInfoGet(NULL, L"ch2", &mfInfo);
				cpMFPipe_Write->PipeInfoGet(NULL, L"ch1", &mfInfo);

				// Read from pipe
				CComBSTR arrStrings[4];
				CComPtr<IUnknown> arrObjectsOut[8];
				
				cpMFPipe_Write->PipeMessageGet(L"ch1", &arrStrings[0], &arrStrings[1], &arrObjectsOut[2], 100 * DS_MSEC);
				cpMFPipe_Write->PipeMessageGet(L"ch2", &arrStrings[2], &arrStrings[3], NULL, 100 * DS_MSEC);
				cpMFPipe_Write->PipeMessageGet(L"ch2", NULL, &arrStrings[2], &arrObjectsOut[3], 100 * DS_MSEC);

				cpMFPipe_Write->PipeGet(L"ch1", &arrObjectsOut[0], 100 * DS_MSEC, NULL);
				cpMFPipe_Write->PipeGet(L"ch2", &arrObjectsOut[1], 100 * DS_MSEC, NULL);

				cpMFPipe_Write->PipeGet(L"ch1", &arrObjectsOut[4], 100 * DS_MSEC, NULL);
				cpMFPipe_Write->PipeGet(L"ch2", &arrObjectsOut[5], 100 * DS_MSEC, NULL);
				cpMFPipe_Write->PipeGet(L"ch2", &arrObjectsOut[6], 100 * DS_MSEC, NULL);

				IUnknown *unknownOut = arrObjectsOut[0].p;
				Assert::IsNotNull(unknownOut);

				IMFBuffer *bufferOut = dynamic_cast<IMFBuffer*>(unknownOut);
				LONG cbMaxSize = 0, cbActualSize = 0;
				LONGLONG lpData = NULL;
				arrMFBuffersIn[i]->BufferLock(eMFLT_NoLock, &cbMaxSize, &cbActualSize, &lpData);

				LPBYTE _q = (LPBYTE)lpData;

				char data[4];
				data[0] = _q[0];
				data[1] = _q[1];
				data[2] = _q[2];
				data[3] = _q[3];

				string str(data, 4);
				string sentStr("test");
				Assert::AreEqual(sentStr, str);
			}

			cpMFPipe_Write->PipeClose();
		}
	};
}