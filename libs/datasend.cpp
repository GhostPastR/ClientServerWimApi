#include "datasend.h"
#include <iostream>

union Size{
    unsigned int size;
    wchar_t byte[sizeof(unsigned int) / sizeof(wchar_t)];
};

std::wstring Header::dataSend()
{
    std::wstring data;
    data += static_cast<wchar_t>(sender);
    data += static_cast<wchar_t>(recipient);
    data += static_cast<wchar_t>(command);
    data += static_cast<wchar_t>(dataComplete);
    return data;
}

void Header::setByte(const std::wstring &data)
{
    sender = static_cast<unsigned int>(data[0]);
    recipient = static_cast<unsigned int>(data[1]);
    command = static_cast<Command>(data[2]);
    dataComplete = static_cast<char>(data[3]);
}

void Header::setByte(LPCTSTR buf)
{
    sender = static_cast<unsigned int>(buf[0]);
    recipient = static_cast<unsigned int>(buf[1]);
    command = static_cast<Command>(buf[2]);
    dataComplete = static_cast<char>(buf[3]);
}

std::wstring DataSend::byte()
{
    std::wstring data;
    data += header.dataSend();
    data += static_cast<wchar_t>(dataFiles.size());

    for(const auto &i : dataFiles){
        data += static_cast<wchar_t>(i.name.size());
        data += i.name;
        if(i.dataFile.size() > std::numeric_limits<wchar_t>::max()){
            data += 2;
            Size size;
            size.size = i.dataFile.size();
            data += size.byte;
        }
        else{
            data += 1;
            data += i.dataFile.size();
        }
        data += i.dataFile;
    }
    return data;

}

bool DataSend::setByte(const std::wstring &data)
{
    if(data.empty())
        return false;
    header.setByte( data );
    auto countFile = static_cast<unsigned int>(data[4]);
    dataFiles.reserve(countFile);
    auto index = data.begin() + Header::sizeHeader();
    for( size_t i = 0; i < countFile; ++i){
        DataFile dataFile;
        auto sizeName = static_cast<unsigned int>(*index);
        auto begin = ++index;
        auto end = begin + sizeName;
        dataFile.name.reserve(sizeName);
        std::copy(begin, end, std::back_inserter(dataFile.name));
        size_t sizeFile = 0;
        index = end;
        if(*index == 1){
            index++;
            sizeFile = *index;
            index++;
        }
        else{
            index++;
            Size size;
            size.byte[0] = *index;
            index++;
            size.byte[1] = *index;
            index++;
            sizeFile = size.size;
        }
        begin = index;
        end = begin + sizeFile;
        dataFile.dataFile.reserve(sizeFile);
        std::copy(begin, end, std::back_inserter(dataFile.dataFile));
        dataFiles.push_back(std::move(dataFile));
        index = end;
    }
    return true;
}
