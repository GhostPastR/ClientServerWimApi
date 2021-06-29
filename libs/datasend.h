#ifndef DATASEND_H
#define DATASEND_H

#include <string>
#include <vector>
#include <windows.h>

enum class Command : unsigned int{
    NOT,
    CONNECT,
    DISCONNECT,
    DATA,
    CHECK_SERVER,
    CONNECT_OK,
    DISCONNECT_OK,
    DATA_OK,
    CHECK_SERVER_OK,
};


struct DataFile
{
    std::wstring name;
    std::wstring dataFile;
};

struct Header{
    unsigned int sender = {};
    unsigned int recipient = {};
    Command command = {};
    char dataComplete = {1};

    constexpr static unsigned int sizeHeader(){return 5;}

    std::wstring dataSend();
    void setByte(const std::wstring &data);
    void setByte(LPCTSTR buf);

};

struct DataSend
{
    Header header;
    std::vector<DataFile> dataFiles;

    std::wstring byte();
    bool setByte(const std::wstring &data);
};

#endif // DATASEND_H
