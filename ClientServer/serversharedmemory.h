#ifndef SERVERSHAREDMEMORY_H
#define SERVERSHAREDMEMORY_H

#include <iprocess.h>
#include <unordered_map>
#include "storagedata.h"

class ServerSharedMemory final : IProcess
{
public:
    ServerSharedMemory();
    ~ServerSharedMemory() override;

    void openSharedMemory();
    void closeSharedMemory();

    void exec() override;
    void processing() override;

private:
    std::unordered_map<unsigned int, StorageData> client_;

    void response(unsigned int id, Command command);
    bool isConnectClient(unsigned int id) const;
};

#endif // SERVERSHAREDMEMORY_H
