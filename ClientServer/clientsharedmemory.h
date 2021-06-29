#ifndef CLIENTSHAREDMEMORY_H
#define CLIENTSHAREDMEMORY_H

#include <iprocess.h>
#include <vector>

class ClientSharedMemory final : public IProcess
{
public:
    ClientSharedMemory( unsigned int id );
    ~ClientSharedMemory() override;

    void exec() override;
    void processing() override;

    void openSharedMemory();
    void closeSharedMemory();
    bool sendFile(bool continuation = false);
    void connect();
    void disconnect();
    void checkServer();

    bool isConnect() const;
    bool isData() const;
    bool isCheckServer() const;

    bool waitingServerConnect();
    bool waitingServerFile();
    bool waitingServerCheck();
    bool waitingServerDisconnect();
    void waiting();

private:
    bool connect_ = {false};
    bool data_ = {false};
    bool checkServer_ = {false};
    unsigned int id_ = {};

    std::wstring heading_;
    std::wstring dataSend_;
    size_t indexData_ = {0};

    std::vector<std::string> nameFiles_;

    std::wstring readFile(const std::string &name) const;
};

#endif // CLIENTSHAREDMEMORY_H
