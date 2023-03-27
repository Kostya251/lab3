#pragma once
#include "resources.h"
#include "fs.h"

#pragma pack(push, 1)
typedef struct
{
    BYTE jump[3];
    BYTE name[8];
    UINT16 sec_size;
    BYTE secs_cluster;
    BYTE reserved_0[7];
    BYTE media_desc;
    UINT16 reserved_2;
    UINT16 secs_track;
    UINT16 num_heads;
    BYTE reserved_3[8];
    UINT16 reserved_4;
    UINT16 reserved_5;
    UINT64 num_secs;
    UINT64 mft_clus;
    UINT64 mft2_clus;
    UINT32 mft_rec_size;
    UINT32 buf_size;
    UINT64 volume_id;
} BOOT_NTFS;
#pragma pack(pop)
class NTFS : public FS
{
public:
    NTFS();
    ~NTFS();
    int Open(const WCHAR *filename, __int64 start_pos) override;
    int Close() override;
    int ReadClusters(__int64 first_cluster, __int64 number_of_clusters, uint8_t *buf) override;

private:
    BOOT_NTFS *boot_ntfs;
};