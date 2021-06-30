#include <iostream>
#include <clientsharedmemory.h>
#include <controlsm.h>
#include <random>
#include <ctime>

DWORD WINAPI exec(LPVOID)
{
    std::mt19937 generator;
    generator.seed(time(0));
    std::uniform_int_distribution<unsigned int> dist(10, 1000);
    auto id = dist(generator);
    ClientSharedMemory client{id};
    client.exec();
    return 0;
}

void infoCommand(){
    std::string text;
    text += "Client management commands:\n";
    text += "0 - Exit.\n";
    text += "1 - Start the client.\n";
    text += "2 - Stop the client.\n";
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
                if(threadExec == nullptr){
                    threadExec = CreateThread(nullptr, 0, exec, nullptr, 0, nullptr);
                    if(threadExec == nullptr){
                        std::cerr << "The message processing thread could not be "
                                     "started!\n";
                    }
                    ControlSm::acommand = 1;
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
    return 0;
}
