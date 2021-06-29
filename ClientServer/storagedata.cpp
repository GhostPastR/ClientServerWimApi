#include "storagedata.h"
#include <windows.h>
#include <iostream>
#include <fstream>

DWORD WINAPI save(LPVOID data)
{
    auto dataSend = static_cast<DataSend*>(data);
    for(const auto &i : dataSend->dataFiles ){
        std::wofstream out;
        std::string name = std::to_string(dataSend->header.sender) + "-";
        name += {i.name.begin(), i.name.end()};
        out.open(name);
        if (out.is_open()){
            out << i.dataFile;
        }
        else{
            std::cout << "Could not write to a file!\n";
        }
        out.close();
    }
    return 0;
}

StorageData::StorageData()
{
}

void StorageData::appendByte(const std::wstring &byte, char completeData)
{
    if(byte_.empty()){
        byte_ += byte;
    }
    else{
        std::copy(byte.cbegin() + Header::sizeHeader(), byte.cend(),
                  std::back_inserter(byte_));
    }
    if(completeData == 1){
        auto data = std::make_shared<DataSend>();
        data->setByte(byte_);
        byte_.clear();
        saveData(data);
    }
}

void StorageData::saveData(const std::shared_ptr<DataSend> &data)
{
    HANDLE threadExec = CreateThread(nullptr, 0, save,
                                     static_cast<LPVOID>(data.get()), 0,
                                     nullptr);
    PostThreadMessage(0, WM_QUIT, 0, 0);
    WaitForSingleObject(threadExec, INFINITE);
    CloseHandle( threadExec );
}
