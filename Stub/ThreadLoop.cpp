#include "pch.h"
#include "ThreadLoop.hpp"
#include "AntiDebugging.hpp"

void ThreadLoop::loop(atomic<bool>& running, unsigned int sleep) {
    RunImp* dImp = RunImp::GetInstance();

#ifndef _DEBUG
    // Hide this thread
    AntiDebugging::HideThread(dImp->dGetCurrentThread());
#endif // !_DEBUG

    do {
        this->procedure();
        this_thread::sleep_for(chrono::seconds(sleep));
    } while (running);
}

// Constructor
ThreadLoop::ThreadLoop() {
    this->isRunning = false;
    this->tTask = nullptr;
}

// Destructor
ThreadLoop::~ThreadLoop() {
    if (this->isRunning) this->stop();
}

void ThreadLoop::start() {
    if (this->isRunning) return;
    this->isRunning = true;
    this->tTask = new thread(&ThreadLoop::loop, this, ref(this->isRunning), 2);
}

void ThreadLoop::stop() {
    this->isRunning = false;
    this->tTask->join();
    delete this->tTask;
    this->tTask = nullptr;
}

// Check if thread is really running or errors
// Returns false if not passing integrity check
bool ThreadLoop::checkIntegrity() {
    // Integrity checks only in release mode
    if (!this->tTask) {
        LOG("[ThreadLoop::checkIntegrity()] Thread pointer is NULL", Logging::Red);
        return false;
    }

    if (!this->isRunning) {
        LOG("[ThreadLoop::checkIntegrity()] Thread is not running", Logging::Red);
        return false;
    }

    if (!this->tTask->joinable()) {
        LOG("[ThreadLoop::checkIntegrity()] Thread is not joinable", Logging::Red);
		return false;
	}

    DWORD threadState = WaitForSingleObject(this->tTask->native_handle(), 500);
    if (threadState != WAIT_TIMEOUT) {
        LOG("[ThreadLoop::checkIntegrity()] State of thread is not WAIT_TIMEOUT; " + threadState, Logging::Red); // Thread is not running
        return false;
    }

    return true;
}
