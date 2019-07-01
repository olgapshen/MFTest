#pragma once

#include "MFPipeTest_i.h"

#include <memory>
#include <queue>
#include <mutex>

#include "MFTypes.h"

class QueueItem
{
private:
	LONG length;
	MFTypes type;
	std::deque<char> queue;
	std::mutex m;

public:
	QueueItem(LONG aLength, MFTypes aType) : 
		length(aLength),
		type(aType) {}

	~QueueItem() {}

	//inline std::deque<char>& getQueue() { return queue; }
	inline LONG getLength() { return length; }
	inline MFTypes getType() { return type; }

	void push(char ch);
	char pop();
	int size();
};

