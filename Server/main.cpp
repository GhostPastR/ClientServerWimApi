#include <iostream>
#include <serversharedmemory.h>
#include <controlsm.h>

DWORD WINAPI exec(LPVOID)
{
    ServerSharedMemory server;
    server.exec();
    return 0;
}

void infoCommand(){
    std::string text;
    text += "Server management commands:\n";
    text += "0 - Exit.\n";
    text += "1 - Start the server.\n";
    text += "2 - Stop the server.\n";
    text += "Enter the command:\n";
    std::cout << text;
}

int main()
{
    HANDLE threadExec = {};
    while(true){
        infoCommand();
        int command;
        std::cin >> command;
        switch (command) {
            case 0:
                ControlSm::acommand = 0;
                PostThreadMessage(0, WM_QUIT, 0, 0);
                WaitForSingleObject(threadExec, INFINITE);
                CloseHandle( threadExec );
                return 0;
            case 1:
                if(threadExec == nullptr) {
                    ControlSm::acommand = 1;
                    threadExec = CreateThread(nullptr, 0, exec, nullptr, 0,
                                              nullptr);
                    if(threadExec == nullptr){
                        std::cerr << "The message processing thread could not "
                                     "be started!\n";
                    }
                }
                else {
                    std::cout << "The client is already running\n";
                }
                break;
            case 2:
                ControlSm::acommand = 0;
                break;
        default:
            std::cout << "Wrong command" << std::endl;
            break;
        }
    }
}
