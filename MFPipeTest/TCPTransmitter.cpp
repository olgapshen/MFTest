#include "stdafx.h"
#include "TCPTransmitter.h"

using namespace std;

TCPTransmitter::TCPTransmitter(string aAdress, int aPort) : TransmitterBase(aAdress, aPort) {}

void TCPTransmitter::receive() {};

TCPTransmitter::~TCPTransmitter() {}

HRESULT TCPTransmitter::Read(string channel, unique_ptr<QueueItem> &item) {
	return E_NOTIMPL;
}

HRESULT TCPTransmitter::Write(const char *aByte, int aLength) {
	return E_NOTIMPL;
}
