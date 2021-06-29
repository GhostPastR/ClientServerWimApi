#include "controlsm.h"

std::atomic<int> ControlSm::acommand = {};
HANDLE ControlSm::semaphore = {};

void ControlSm::createSemaphore()
{
    if(ControlSm::semaphore == nullptr){
        ControlSm::semaphore = CreateSemaphore( nullptr, 1, 1,
                                                  TEXT("SemaphoreSm") );
    }
}

void ControlSm::closeSemaphore()
{
    CloseHandle(ControlSm::semaphore);
}

bool ControlSm::wait(DWORD dwMilliseconds)
{
    return WaitForSingleObject(ControlSm::semaphore, dwMilliseconds)
            == WAIT_OBJECT_0;
}

bool ControlSm::release()
{
    return ReleaseSemaphore(ControlSm::semaphore, 1, nullptr);
}
