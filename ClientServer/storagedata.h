#ifndef STORAGEDATA_H
#define STORAGEDATA_H

#include <memory>
#include <datasend.h>

class StorageData final
{
public:
    StorageData();

    void appendByte(const std::wstring &byte, char completeData);
private:
    std::wstring byte_;

    void saveData(const std::shared_ptr<DataSend> &data);
};

#endif // STORAGEDATA_H
