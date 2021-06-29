#include "iprocess.h"
#include <iostream>

IProcess::IProcess()
{

}

IProcess::~IProcess()
{

}

void IProcess::openSharedMemory(const std::string &ok, const std::string &error)
{
    if(sharedMemory_ == nullptr) {
        sharedMemory_ = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr,
                                          PAGE_READWRITE, 0, bufSize,
                                          nameSharedMemory.c_str());
        if(sharedMemory_ == nullptr) {
            throw std::runtime_error(error);
        }

        buf_ = (LPTSTR)MapViewOfFile(sharedMemory_, FILE_MAP_ALL_ACCESS, 0, 0,
                                     bufSize);
        if(buf_ == nullptr) {
            CloseHandle(sharedMemory_);
            throw std::runtime_error(error);
        }
        std::cout << ok << std::endl;
    }
}

void IProcess::closeSharedMemory(const std::string &ok)
{
    if(sharedMemory_ != nullptr){
        UnmapViewOfFile(buf_);
        CloseHandle(sharedMemory_);
        std::cout << ok << std::endl;
    }
}

void IProcess::sendData(const std::wstring &data)
{
    if((sharedMemory_ != nullptr) && (buf_ != nullptr)){
        CopyMemory((PVOID)buf_, data.c_str(), data.size() * sizeof(wchar_t));
    }
    else {
        std::cout << "Failed to send data!" << std::endl;
    }
}

void IProcess::clearSharedMemory()
{
    const wchar_t data[] = {1,1,0};
    CopyMemory((PVOID)buf_, data, 3 * sizeof(wchar_t));
}
