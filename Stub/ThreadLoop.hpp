#pragma once
#include "pch.h"

class ThreadLoop {
private:
	atomic<bool> isRunning;
	thread* tTask;

protected:
	virtual void loop(atomic<bool>& running, unsigned int sleep) = 0;

public:
	ThreadLoop();
	~ThreadLoop();

	void start();
	void stop();
};