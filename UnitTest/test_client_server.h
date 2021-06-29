#ifndef TEST_CLIENT_SERVER_H
#define TEST_CLIENT_SERVER_H

#include <iostream>
#include <cassert>
#include <clientsharedmemory.h>
#include <serversharedmemory.h>

class TestBuf : public IProcess{
public:
    TestBuf(){}
    ~TestBuf(){}

    void openSharedMemory(){
        IProcess::openSharedMemory("","");
    };
    void closeSharedMemory(){
        IProcess::closeSharedMemory("");
    }

    void processing() override{}
    void exec() override{}

    LPCTSTR getBuf() const{return buf_;}
};




void test_client_server_connect_disconnect(){
    TestBuf testBuf;
    testBuf.openSharedMemory();

    ClientSharedMemory client{100};
    ServerSharedMemory server;
    client.openSharedMemory();
    server.openSharedMemory();
    client.connect();

    std::wstring connect;
    connect += static_cast<wchar_t>(100);
    connect += static_cast<wchar_t>(1);
    connect += static_cast<wchar_t>(Command::CONNECT);
    connect += static_cast<wchar_t>(1);
    assert(connect==std::wstring{testBuf.getBuf()});

    server.processing();

    std::wstring connect_ok;
    connect_ok += static_cast<wchar_t>(1);
    connect_ok += static_cast<wchar_t>(100);
    connect_ok += static_cast<wchar_t>(Command::CONNECT_OK);
    connect_ok += static_cast<wchar_t>(1);
    assert(connect_ok==std::wstring{testBuf.getBuf()});

    client.processing();
    assert(client.isConnect());

    client.disconnect();

    std::wstring disconnect;
    disconnect += static_cast<wchar_t>(100);
    disconnect += static_cast<wchar_t>(1);
    disconnect += static_cast<wchar_t>(Command::DISCONNECT);
    disconnect += static_cast<wchar_t>(1);

    assert(disconnect==std::wstring{testBuf.getBuf()});

    server.processing();

    std::wstring disconnect_ok;
    disconnect_ok += static_cast<wchar_t>(1);
    disconnect_ok += static_cast<wchar_t>(100);
    disconnect_ok += static_cast<wchar_t>(Command::DISCONNECT_OK);
    disconnect_ok += static_cast<wchar_t>(1);
    assert(disconnect_ok==std::wstring{testBuf.getBuf()});

    client.processing();

    assert(!client.isConnect());

    client.closeSharedMemory();
    server.closeSharedMemory();
    testBuf.closeSharedMemory();

    std::cout << "Test 'test_client_server_connect_disconnect' - Pass!\n";
}

void test_client_server_check_server(){
    TestBuf testBuf;
    testBuf.openSharedMemory();

    ClientSharedMemory client{100};
    ServerSharedMemory server;
    client.openSharedMemory();
    server.openSharedMemory();
    client.connect();
    server.processing();
    client.processing();

    client.checkServer();
    std::wstring check_response;
    check_response += static_cast<wchar_t>(100);
    check_response += static_cast<wchar_t>(1);
    check_response += static_cast<wchar_t>(Command::CHECK_SERVER);
    check_response += static_cast<wchar_t>(1);
    assert(check_response==std::wstring{testBuf.getBuf()});

    server.processing();

    std::wstring server_response;
    server_response += static_cast<wchar_t>(1);
    server_response += static_cast<wchar_t>(100);
    server_response += static_cast<wchar_t>(Command::CHECK_SERVER_OK);
    server_response += static_cast<wchar_t>(1);
    assert(server_response==std::wstring{testBuf.getBuf()});

    client.processing();

    assert(client.isCheckServer());

    client.disconnect();
    server.processing();
    client.processing();

    client.closeSharedMemory();
    server.closeSharedMemory();
    testBuf.closeSharedMemory();

    std::cout << "Test 'test_client_server_check_server' - Pass!\n";
}

void test_client_server_file(){
    TestBuf testBuf;
    testBuf.openSharedMemory();
    ClientSharedMemory client{100};
    ServerSharedMemory server;
    client.openSharedMemory();
    server.openSharedMemory();
    client.sendFile();

    Header header;
    header.setByte(testBuf.getBuf());
    assert(header.sender == 100);
    assert(header.recipient == 1);
    assert(header.command == Command::DATA);
    server.processing();
    header.setByte(testBuf.getBuf());
    assert(header.sender == 1);
    assert(header.recipient == 100);
    assert(header.command == Command::DATA_OK);

    client.closeSharedMemory();
    server.closeSharedMemory();
}



//std::cout << connect_ok.size() << std::endl;
//std::cout << std::wstring{testBuf.getBuf()}.size() << std::endl;

#endif // TEST_CLIENT_SERVER_H
