#include "stdafx.h"
#include "QueueItem.h"

void QueueItem::push(char ch) {
	m.lock();
	queue.push_back(ch);
	m.unlock();
}

char QueueItem::pop() {
	m.lock();
	char ch = queue.front();
	queue.pop_front();
	m.unlock();

	return ch;
}

int QueueItem::size() {
	m.lock();
	int size = queue.size();
	m.unlock();

	return size;
}