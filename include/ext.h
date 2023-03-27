#pragma once
#include "resources.h"
#include "fs.h"
class Ext : public FS
{
public:
    Ext();
    ~Ext();
    int Open(const WCHAR *filename, __int64 start_pos) override;
    int Close() override;
    int ReadClusters(__int64 first_cluster, __int64 number_of_clusters, uint8_t *buf) override;

private:
};