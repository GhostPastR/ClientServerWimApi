#include "clientsharedmemory.h"
#include <iostream>
#include <fstream>
#include <datasend.h>
#include <algorithm>
#include <controlsm.h>

ClientSharedMemory::ClientSharedMemory(unsigned int id) : IProcess(), id_(id)
{
    std::ifstream in("config.txt");
    if (in.is_open()){
        std::string line;
        while (getline(in, line)){
            nameFiles_.push_back( line );
        }
    }
    in.close();
}

ClientSharedMemory::~ClientSharedMemory()
{
    closeSharedMemory();
}

void ClientSharedMemory::openSharedMemory()
{
    IProcess::openSharedMemory("The client is running.","Failed to connect!");
}

void ClientSharedMemory::closeSharedMemory()
{
    IProcess::closeSharedMemory("The client is stopped.");
}


void ClientSharedMemory::connect()
{
    DataSend dataSend;
    dataSend.header.sender = id_;
    dataSend.header.recipient = 1;
    dataSend.header.command = Command::CONNECT;
    sendData(dataSend.byte());
}

void ClientSharedMemory::disconnect()
{
    DataSend dataSend;
    dataSend.header.sender = id_;
    dataSend.header.recipient = 1;
    dataSend.header.command = Command::DISCONNECT;
    sendData(dataSend.byte());

    connect_ = false;
    std::cout << "The connection to the server is broken\n";
}

void ClientSharedMemory::exec()
{
    ControlSm::createSemaphore();
    try {
        openSharedMemory();

        if(!waitingServerConnect()){
            std::cout << "The server is not responding\n";
            return;
        }

        bool flag = true;
        while(flag){
            if(!waitingServerFile()) {
                flag = waitingServerCheck();
            }
            else {
                break;
            }
        }

        waiting();

        flag = true;
        while(flag){
            if(!waitingServerDisconnect()) {
                flag = waitingServerCheck();
            }
            else {
                std::cout << "The server is not responding\n";
                break;
            }
        }
    }
    catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }
    ControlSm::closeSemaphore();
}

bool ClientSharedMemory::waitingServerConnect(){
    if( ControlSm::wait(1000) ){
        connect();
        ControlSm::release();
    }
    else {
        ControlSm::closeSemaphore();
        return false;
    }
    MSG msg;
    SetTimer(nullptr, 0, 50, nullptr);
    int i = 0;
    while(GetMessage(&msg, NULL, 0, 0) && (i < 100)) {
        DispatchMessage(&msg);
        if( ControlSm::wait(50) ){
            processing();
            if(isConnect() ) {
                ControlSm::release();
                KillTimer(nullptr, 0);
                return true;
            }
            ControlSm::release();
        }
        ++i;
    }
    KillTimer(nullptr, 0);
    return false;
}

bool ClientSharedMemory::waitingServerFile(){
    bool isDataFile = false;
    do{
        if( ControlSm::wait(1000) ){
            isDataFile = sendFile(isDataFile);
            ControlSm::release();
        }
        else {
            ControlSm::closeSemaphore();
            return false;
        }

        MSG msg;
        int i = 0;
        while(GetMessage(&msg, NULL, 0, 0) && (i < 100)) {
            DispatchMessage(&msg);
            if(ControlSm::wait(50)){
                processing();
                if(isData()) {
                    if(!isDataFile){
                        ControlSm::release();
                        KillTimer(nullptr, 0);
                        return true;
                    }
                    ControlSm::release();
                    break;
                }
                ControlSm::release();
            }
            ++i;
        }
        KillTimer(nullptr, 0);
        if(i==100){
            return false;
        }
    }
    while(isDataFile);
    return true;
}

bool ClientSharedMemory::waitingServerCheck(){
    if( ControlSm::wait(1000) ){
        checkServer();
        ControlSm::release();
    }
    else {
        ControlSm::closeSemaphore();
        return false;
    }

    MSG msg;
    int i = 0;
    while(GetMessage(&msg, NULL, 0, 0) && (i < 100)) {
        DispatchMessage(&msg);
        if( ControlSm::wait(50) ){
            processing();
            if(isData() ) {
                ControlSm::release();
                KillTimer(nullptr, 0);
                return true;
            }
            ControlSm::release();
        }
        ++i;
    }
    KillTimer(nullptr, 0);
    return false;
}

bool ClientSharedMemory::waitingServerDisconnect(){
    if( ControlSm::wait(1000) ){
        disconnect();
        ControlSm::release();
    }
    else {
        ControlSm::closeSemaphore();
        return false;
    }

    MSG msg;
    int i = 0;
    while(GetMessage(&msg, NULL, 0, 0) && (i < 100)) {
        DispatchMessage(&msg);
        if( ControlSm::wait(50) ){
            processing();
            if(isData() ) {
                ControlSm::release();
                KillTimer(nullptr, 0);
                return true;
            }
            ControlSm::release();
        }
        ++i;
    }
    KillTimer(nullptr, 0);
    return false;
}

void ClientSharedMemory::waiting()
{
    MSG msg;
    int i = 0;
    while(GetMessage(&msg, NULL, 0, 0) && (ControlSm::acommand != 0) ) {
        DispatchMessage(&msg);
        if( ControlSm::wait(50) ){
            processing();
            ControlSm::release();
        }
        if(i == 100){
            if(!waitingServerCheck()){
                break;
            }
            i = 0;
        }
        i++;
    }
    KillTimer(nullptr, 0);
}

void ClientSharedMemory::processing()
{
    Header header;
    header.setByte(buf_);
    if((header.sender == 1) && (header.recipient == id_)){
        if(header.command == Command::CONNECT_OK ){
            connect_ = true;
            std::cout << "The connection to the server is established!\n";
            clearSharedMemory();
        }
        else if(header.command == Command::DATA_OK ){
            data_ = true;
            std::cout << "The server has received the data!\n";
            clearSharedMemory();
        }
        else if(header.command == Command::CHECK_SERVER_OK ){
            checkServer_ = true;
            std::cout << "The server is running!\n";
            clearSharedMemory();
        }
    }
}


void ClientSharedMemory::checkServer()
{
    checkServer_ = false;
    DataSend dataSend;
    dataSend.header.sender = id_;
    dataSend.header.recipient = 1;
    dataSend.header.command = Command::CHECK_SERVER;
    sendData(dataSend.byte());
}

bool ClientSharedMemory::isConnect() const
{
    return connect_;
}

bool ClientSharedMemory::isData() const
{
    return data_;
}

bool ClientSharedMemory::isCheckServer() const
{
    return checkServer_;
}

bool ClientSharedMemory::sendFile(bool continuation)
{
    auto sBuf = bufSize - sizeof(wchar_t);
    if(continuation){
        auto size = ( dataSend_.size() - indexData_ ) * sizeof(wchar_t);
        auto sizeHeader = heading_.size() * sizeof(wchar_t);
        size += sizeHeader;
        auto byte = heading_;
        if(size <= sBuf){
            byte[3] = static_cast<wchar_t>(1);
            std::copy(dataSend_.cbegin() + indexData_,
                      dataSend_.cend(), std::back_inserter(byte));
            byte.push_back('\0');
            sendData(byte);
            return false;
        }
        else {
            auto begin = dataSend_.cbegin() + indexData_;
            std::copy(begin, begin + sBuf/sizeof(wchar_t) - heading_.size(),
                      std::back_inserter(byte));
            indexData_ += sBuf/sizeof(wchar_t) - heading_.size();
            sendData(byte);
            return true;
        }
    }
    else{
        data_ = false;
        DataSend dataSend;
        dataSend.header.sender = id_;
        dataSend.header.recipient = 1;
        dataSend.header.command = Command::DATA;

        for(const auto &i : nameFiles_){
            DataFile file;
            auto it = std::find( i.crbegin(), i.crend(), '/' );
            if(it != i.crend()){
                file.name = {i.crbegin(), it};
                std::reverse(file.name.begin(), file.name.end());
            }
            else{
                file.name = {i.cbegin(), i.cend()};
            }
            file.dataFile = readFile(i);
            dataSend.dataFiles.push_back(std::move(file));
        }

        dataSend_ = dataSend.byte();
        if(dataSend_.size() * sizeof(wchar_t) <= sBuf){
            dataSend_.push_back('\0');
            sendData(dataSend_);
            return false;
        }
        else {
            std::wstring byte;
            dataSend_[3] = static_cast<wchar_t>(2);
            heading_.clear();
            std::copy(dataSend_.cbegin(), dataSend_.cbegin() +
                      Header::sizeHeader(), std::back_inserter(heading_));
            std::copy(dataSend_.cbegin(),
                      dataSend_.cbegin() + sBuf/sizeof(wchar_t),
                      std::back_inserter(byte));
            indexData_ += sBuf/sizeof(wchar_t);
            sendData(byte);
            return true;
        }
    }
}

std::wstring ClientSharedMemory::readFile(const std::string &name) const
{   
    std::wstring text;
    std::wifstream in(name);
    if (in.is_open()){
        std::wstring line;
        while (getline(in, line)){
            text += line;
        }
    }
    in.close();
    return text;
}
