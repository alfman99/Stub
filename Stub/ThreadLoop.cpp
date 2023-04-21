#include "pch.h"
#include "ThreadLoop.hpp"
#include "AntiDebugging.hpp"

void ThreadLoop::loop(atomic<bool>& running, unsigned int sleep) {
#ifndef _DEBUG
    RunImp* dImp = RunImp::GetInstance();
    // Hide this thread
    AntiDebugging::HideThread(dImp->dGetCurrentThread());
#else
    cout << "Sleep threadloop 6000" << endl;
    this_thread::sleep_for(chrono::seconds(6000));
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
#ifdef _DEBUG // IF DEBUG
    if (!this->tTask) cout << "[ThreadLoop::checkIntegrity()] Thread pointer is NULL" << endl;
    if (!this->isRunning) cout << "[ThreadLoop::checkIntegrity()] Thread is not running" << endl;
    if (!this->tTask->joinable()) cout << "[ThreadLoop::checkIntegrity()] Thread is not joinable" << endl;

    DWORD threadState = WaitForSingleObject(this->tTask->native_handle(), 500);
    if (threadState != WAIT_TIMEOUT) cout << "[ThreadLoop::checkIntegrity()] State of thread is not WAIT_TIMEOUT; " << threadState << endl; // Thread is not running
#else // IF RELEASE
    if (!this->tTask) return false;
    if (!this->isRunning) return false;
    if (!this->tTask->joinable()) return false;

    DWORD threadState = WaitForSingleObject(this->tTask->native_handle(), 500);
    if (threadState != WAIT_TIMEOUT) return false; // Thread is not running
#endif

    return true;
}
