#ifndef CONTROLSM_H
#define CONTROLSM_H

#include <atomic>
#include <windows.h>

struct ControlSm
{    
    static std::atomic<int> acommand;

    static void createSemaphore();
    static void closeSemaphore();

    static bool wait(DWORD dwMilliseconds = INFINITE);
    static bool release();

private:
    static HANDLE semaphore;
};

#endif // CONTROLSM_H
