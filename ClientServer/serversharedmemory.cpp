#include "serversharedmemory.h"
#include <iostream>
#include <datasend.h>
#include <memory>
#include <controlsm.h>

ServerSharedMemory::ServerSharedMemory() : IProcess()
{

}

ServerSharedMemory::~ServerSharedMemory()
{
    closeSharedMemory();
}

void ServerSharedMemory::openSharedMemory()
{
    IProcess::openSharedMemory("Start server.", "Failed to start the server!");
}

void ServerSharedMemory::closeSharedMemory()
{
    IProcess::closeSharedMemory("The server is stopped.");
}

void ServerSharedMemory::exec()
{
    ControlSm::createSemaphore();
    try {
        openSharedMemory();
        MSG msg;
        SetTimer(nullptr, 0, 50, nullptr );
        while(GetMessage(&msg, NULL, 0, 0) && (ControlSm::acommand != 0)) {
            DispatchMessage(&msg);
            if( ControlSm::wait(50) ) {
                processing();
                ControlSm::release();
            }
        }
        KillTimer(nullptr, 0);
    }
    catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }
    ControlSm::closeSemaphore();
}

void ServerSharedMemory::processing()
{
    Header header;
    header.setByte(buf_);
    if((header.sender != 1) && (header.recipient == 1)){
        if(header.command == Command::CONNECT){
            if(!isConnectClient(header.sender)){
                client_.insert(std::make_pair(header.sender, StorageData()));
                response(header.sender, Command::CONNECT_OK);
                std::cout << "A connection has been established with a new "
                               "client, id = " << header.sender << std::endl;
            }
        }
        else if(header.command == Command::DATA){
            if(isConnectClient(header.sender)){
                auto &client = client_.find(header.sender)->second;
                client.appendByte(buf_, header.dataComplete);
                response(header.sender, Command::DATA_OK);
                std::cout << "The data was received from the client, id = "
                          << header.sender << std::endl;
            }
        }
        else if(header.command == Command::DISCONNECT){
            if(isConnectClient(header.sender)){
                client_.erase(header.sender);
                response(header.sender, Command::DISCONNECT_OK);
                std::cout << "The client broke the connection, id = "
                          << header.sender << std::endl;
            }

        }
        else if(header.command == Command::CHECK_SERVER){
            if(isConnectClient(header.sender)){
                response(header.sender, Command::CHECK_SERVER_OK);
            }
        }
    }
}

void ServerSharedMemory::response(unsigned int id, Command command)
{
    DataSend dataSend;
    dataSend.header.sender = 1;
    dataSend.header.recipient = id;
    dataSend.header.command = command;
    sendData(dataSend.byte());
}

bool ServerSharedMemory::isConnectClient(unsigned int id) const
{
    return client_.find(id) != client_.end();
}
