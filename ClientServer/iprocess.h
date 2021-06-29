#ifndef IPROCESS_H
#define IPROCESS_H

#include <windows.h>
#include <string>

class IProcess
{
public:
    IProcess();
    virtual ~IProcess();
    IProcess(const IProcess &) = delete;
    IProcess &operator=(const IProcess &) = delete;

protected:
    const std::wstring nameSharedMemory = L"GUID";
    const unsigned long bufSize = 256 * 1024;

    HANDLE sharedMemory_ = {};
    LPCTSTR buf_ = {};

    void openSharedMemory( const std::string &ok, const std::string &error);
    void closeSharedMemory(const std::string &ok);
    void sendData( const std::wstring &data );
    void clearSharedMemory();

    virtual void exec() = 0;
    virtual void processing() = 0;
};

#endif // IPROCESS_H
