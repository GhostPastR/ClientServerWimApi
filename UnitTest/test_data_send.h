#ifndef TEST_DATA_SEND_H
#define TEST_DATA_SEND_H

#include <iostream>
#include <cassert>
#include <datasend.h>

void test_data_send_not_file(){
    DataSend data;
    data.header.sender = 1;
    data.header.recipient = 0;
    data.header.command = Command::CONNECT;

    std::wstring wstr;
    wstr += static_cast<wchar_t>(data.header.sender);
    wstr += static_cast<wchar_t>(data.header.recipient);
    wstr += static_cast<wchar_t>(data.header.command);
    wstr += static_cast<wchar_t>(data.header.dataComplete);
    wstr += static_cast<wchar_t>(0);

    auto buf = data.byte();
    assert(buf == wstr);

    DataSend result;
    result.setByte( buf );
    assert(data.header.sender == 1);
    assert(data.header.recipient == 0);
    assert(data.header.command == Command::CONNECT);

    std::cout << "Test 'test_data_send_not_file' - Pass!\n";
}


void test_data_send_1_file(){
    DataSend data;
    data.header.sender = 1;
    data.header.recipient = 0;
    data.header.command = Command::DATA;

    DataFile dataFile;
    dataFile.name = L"name_file";
    dataFile.dataFile = L"1234567890";

    data.dataFiles.push_back(dataFile);

    std::wstring wstr;
    wstr += static_cast<wchar_t>(data.header.sender);
    wstr += static_cast<wchar_t>(data.header.recipient);
    wstr += static_cast<wchar_t>(data.header.command);
    wstr += static_cast<wchar_t>(data.header.dataComplete);
    wstr += static_cast<wchar_t>(1);

    wstr += static_cast<wchar_t>(dataFile.name.size());
    wstr += dataFile.name;
    wstr += 1;
    wstr += static_cast<wchar_t>(dataFile.dataFile.size());
    wstr += dataFile.dataFile;

    auto buf = data.byte();
    assert(buf == wstr);

    DataSend result;
    result.setByte( buf );
    assert(data.header.sender == 1);
    assert(data.header.recipient == 0);
    assert(data.header.command == Command::DATA);
    assert(data.dataFiles.size() == 1);

    assert(data.dataFiles[0].name.size() == dataFile.name.size());
    assert(data.dataFiles[0].name == dataFile.name);
    assert(data.dataFiles[0].dataFile.size() == dataFile.dataFile.size());
    assert(data.dataFiles[0].dataFile == dataFile.dataFile);

    std::cout << "Test 'test_data_send_one_file' - Pass!\n";
}

void test_data_send_2_file(){
    DataSend data;
    data.header.sender = 1;
    data.header.recipient = 0;
    data.header.command = Command::DATA;

    DataFile dataFile;
    dataFile.name = L"name_file_1";
    dataFile.dataFile = L"1234567890";
    data.dataFiles.push_back(dataFile);

    DataFile dataFile2;
    dataFile2.name = L"name_file_2";
    dataFile2.dataFile = L"9876543210";
    data.dataFiles.push_back(dataFile2);

    std::wstring wstr;
    wstr += static_cast<wchar_t>(data.header.sender);
    wstr += static_cast<wchar_t>(data.header.recipient);
    wstr += static_cast<wchar_t>(data.header.command);
    wstr += static_cast<wchar_t>(data.header.dataComplete);
    wstr += static_cast<wchar_t>(2);

    wstr += static_cast<wchar_t>(dataFile.name.size());
    wstr += dataFile.name;
    wstr += 1;
    wstr += static_cast<wchar_t>(dataFile.dataFile.size());
    wstr += dataFile.dataFile;

    wstr += static_cast<wchar_t>(dataFile2.name.size());
    wstr += dataFile2.name;
    wstr += 1;
    wstr += static_cast<wchar_t>(dataFile2.dataFile.size());
    wstr += dataFile2.dataFile;

    auto buf = data.byte();
    assert(buf == wstr);

    DataSend result;
    result.setByte( buf );
    assert(data.header.sender == 1);
    assert(data.header.recipient == 0);
    assert(data.header.command == Command::DATA);
    assert(data.dataFiles.size() == 2);

    assert(data.dataFiles[0].name.size() == dataFile.name.size());
    assert(data.dataFiles[0].name == dataFile.name);
    assert(data.dataFiles[0].dataFile.size() == dataFile.dataFile.size());
    assert(data.dataFiles[0].dataFile == dataFile.dataFile);

    assert(data.dataFiles[1].name.size() == dataFile2.name.size());
    assert(data.dataFiles[1].name == dataFile2.name);
    assert(data.dataFiles[1].dataFile.size() == dataFile2.dataFile.size());
    assert(data.dataFiles[1].dataFile == dataFile2.dataFile);

    std::cout << "Test 'test_data_send_one_file' - Pass!\n";
}

#endif // TEST_DATA_SEND_H
