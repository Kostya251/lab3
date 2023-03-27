#pragma once
#include "resources.h"
class FS
{
public:
    FS(){};
    virtual ~FS();
    virtual int Open(const WCHAR *filename, __int64 start_pos);
    virtual int Close();
    virtual int ReadClusters(__int64 first_cluster, __int64 number_of_clusters, uint8_t *buf);
    DWORD GetLastError_() { return m_dwLastError; };

private:
protected:
    HANDLE m_hDisk{INVALID_HANDLE_VALUE};
    DWORD m_dwLastError{0};
};