#pragma once
#include "resources.h"
// static const uint8_t gpt_header_signature = {0x4546492050415254};
#define GPT_HEADER_SIGNATURE 0x5452415020494645
#define GPT_PART_NAME_LEN (72 / sizeof(uint16_t))
typedef struct
{
    uint32_t time_low;
    uint16_t time_mid;
    uint16_t time_hi_and_version;
    uint8_t clock_seq_hi;
    uint8_t clock_seq_low;
    uint8_t node[6];
} Guid;
typedef struct
{
    uint64_t Signature;          /* header identification */
    uint32_t Revision;           /* header version */
    uint32_t HeaderSize;         /* in bytes */
    uint32_t HeaderCRC32;        /* header CRC checksum */
    uint32_t Reserved1;          /* must be 0 */
    uint64_t PrimaryLBA;         /* LBA of block that contains this struct (LBA 1) */
    uint64_t BackupLBA;          /* backup GPT header */
    uint64_t FirstUsableLBA;     /* first usable logical block for partitions */
    uint64_t LastUsableLBA;      /* last usable logical block for partitions */
    Guid UniqueDiskGUID;         /* unique disk identifier */
    uint64_t FirstEntryLBA;      /* LBA of start of partition entries array */
    uint32_t NumberOfEntries;    /* total partition entries - normally 128 */
    uint32_t SizeOfEntry;        /* bytes for each GUID pt */
    uint32_t EntriesCRC2;        /* partition CRC checksum */
    uint8_t Reserved2[512 - 92]; /* must all be 0 */
} PrimaryGptHeader;              // Primary GPT Header - LBA1
typedef struct
{
    Guid PartitionTypeGUID; /* purpose and type of the partition */
    Guid UniquePartitionGUID;
    uint64_t FirstLBA;
    uint64_t LastLBA;
    uint64_t Attr;
    uint16_t PartitionName[GPT_PART_NAME_LEN];
} GptEntry;
typedef struct
{
    GptEntry entry;
    uint64_t GetSize() const { return (entry.LastLBA - entry.FirstLBA + 1) * SECTOR_SIZE; }
    uint64_t GetPos() const { return entry.FirstLBA * SECTOR_SIZE; }
    uint64_t GetEnd() const { return (entry.LastLBA + 1) * SECTOR_SIZE; }
} GPTPartition;
class GPT
{
public:
    GPT();
    ~GPT();
    int open(const WCHAR *filename);
    int close();
    DWORD GetLastError_();
    inline const PrimaryGptHeader *GetGuidTableHeader() { return m_guidTableHeader; };
    void PrintGuidTableHeader();
    int FindEntryIndexById(const uint32_t id);
    const GPTPartition *GetEntryByIndex(uint8_t index) const;
    inline uint32_t GetNumberOfEntries() const { return m_numberOfEntries; };

private:
    HANDLE m_hDisk{INVALID_HANDLE_VALUE};
    DWORD m_dwLastError;
    PrimaryGptHeader *m_guidTableHeader{nullptr};
    GPTPartition *m_gptEntries{nullptr};
    uint32_t m_numberOfEntries{0};
};