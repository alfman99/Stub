#pragma once
#include "pch.h"


class ThreadLoop {
private:
	atomic<bool> isRunning;
	thread* tTask;

	void loop(atomic<bool>& running, unsigned int sleep);

protected:
	virtual void procedure() = 0;
	bool checkIntegrity();

public:
	ThreadLoop();
	~ThreadLoop();

	void start();
	void stop();
};