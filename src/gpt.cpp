#include "gpt.h"
#include "my_utils.h"
// #include <winsock.h>
// static const CPartType kPartTypes[] =
//     {

//         {0x21686148, "BIOS Boot"},

//         {0xC12A7328,  "EFI System"},
//         {0x024DEE41, "MBR"},

//         {0xE3C9E316, "Windows MSR"},
//         {0xEBD0A0A2, "Windows BDP"},
//         {0x5808C8AA, "Windows LDM Metadata"},
//         {0xAF9B60A0, "Windows LDM Data"},
//         {0xDE94BBA4, "Windows Recovery"},

//         {0x0FC63DAF, "Linux Data"},
//         {0x0657FD6D, "Linux Swap"},

//         {0x83BD6B9D, "FreeBSD Boot"},
//         {0x516E7CB4, "FreeBSD Data"},
//         {0x516E7CB5, "FreeBSD Swap"},
//         {0x516E7CB6, "FreeBSD UFS"},
//         {0x516E7CB8, "FreeBSD Vinum"},
//         {0x516E7CB8, "FreeBSD ZFS"},

//         {0x48465300, "HFS+"},
//         {0x7C3457EF, "APFS"},
// };
GPT::GPT()
{
    m_guidTableHeader = new PrimaryGptHeader;
}
GPT::~GPT()
{
    delete[] m_gptEntries;
    delete m_guidTableHeader;
    close();
}
int GPT::open(const WCHAR *filename)
{
    DWORD error_code;
    m_hDisk = CreateFileW(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (m_hDisk == INVALID_HANDLE_VALUE)
    {
        m_dwLastError = GetLastError();
        return -1;
    }
    DWORD bytes_read;
    // Read Rrotective MBR
    uint8_t buf[SECTOR_SIZE];
    __int64 offset{0};
    __int64 cur_offset;
    cur_offset = set_file_seek(m_hDisk, offset, FILE_BEGIN);
    if (cur_offset == -1)
    {
        m_dwLastError = GetLastError();
        return -1;
    }
    if (ReadFile(m_hDisk, buf, SECTOR_SIZE, &bytes_read, NULL) == FALSE)
    {
        m_dwLastError = GetLastError();
        return -1;
    }
    if (buf[0x1FE] != 0x55 || buf[0x1FF] != 0xAA)
    {
        return -2;
    }

    // Read Primary GPT Header
    cur_offset = set_file_seek(m_hDisk, SECTOR_SIZE, FILE_BEGIN);
    if (cur_offset == -1)
    {
        m_dwLastError = GetLastError();
        return -1;
    }
    if (ReadFile(m_hDisk, m_guidTableHeader, SECTOR_SIZE, &bytes_read, NULL) == FALSE)
    {
        printf("Err\n");
        m_dwLastError = GetLastError();
        return -1;
    }

    // Check gpt header signature
    // print_hex_chars((uint8_t *)&m_guidTableHeader.Signature, 8);
    // printf("\n");
    // print_hex_chars((uint8_t *)GPT_HEADER_SIGNATURE, 8);
    // printf("\n");
    if (m_guidTableHeader->Signature != (uint64_t)GPT_HEADER_SIGNATURE)
        return -3;

    // Check header size
    if (m_guidTableHeader->HeaderSize != (uint64_t)0x5c)
        return -4;

    // check header crc

    // if ()
    // {
    //     return -5;
    // }

    // Read partition entries

    cur_offset = set_file_seek(m_hDisk, SECTOR_SIZE * 2, FILE_BEGIN);
    // printf("cur offset: %d\n", cur_offset);
    // Чтение 4 элементов раздела в буфер buf
    if (ReadFile(m_hDisk, buf, SECTOR_SIZE, &bytes_read, NULL) == FALSE)
    {
        m_dwLastError = GetLastError();
        return -1;
    }
    // print_hex(buf, 512);
    // Подсчет количества непустых элементов
    for (int i = 0; i < 4; ++i)
    {
        if (buf[i * 128] == 0)
            break;
        // printf("%0.2X\n", buf[i * 128]);
        m_numberOfEntries++;
    }
    // printf("Number of GPT entries: %d\n", m_numberOfEntries);
    m_gptEntries = new GPTPartition[m_numberOfEntries];

    for (int i = 0; i < m_numberOfEntries; ++i)
    {
        // printf("%d\n", i);
        memcpy(&m_gptEntries[i].entry, &buf[i * 128], sizeof(m_gptEntries[i].entry));
        // printf("%0.2X\n", buf[i * 128]);
    }

    return 0;
}
int GPT::close()
{
    CloseHandle(m_hDisk);
    return 0;
}
DWORD GPT::GetLastError_()
{
    return m_dwLastError;
}
int GPT::FindEntryIndexById(const uint32_t id)
{
    for (int i = 0; i < m_numberOfEntries; ++i)
    {
        // uint32_t id_ = htonl(m_gptEntries[i].entry.PartitionTypeGUID.time_low);
        // printf("0x%0.4X\n", m_gptEntries[i].entry.PartitionTypeGUID.time_low);
        if (id == m_gptEntries[i].entry.PartitionTypeGUID.time_low)
            return i;
    }
    return -1;
}
const GPTPartition *GPT::GetEntryByIndex(uint8_t index) const
{
    if (index >= m_numberOfEntries || index < 0)
        return NULL;
    return &m_gptEntries[index];
}
void GPT::PrintGuidTableHeader()
{
    printf("%-17s", "Signature: ");
    print_hex_chars((uint8_t *)&m_guidTableHeader->Signature, sizeof(m_guidTableHeader->Signature));
    printf("\n");
    printf("%-17s", "Revision: ");
    print_hex_chars((uint8_t *)&m_guidTableHeader->Revision, sizeof(m_guidTableHeader->Revision));
    printf("\n");
    printf("%-17s", "HeaderSize: ");
    print_hex_chars((uint8_t *)&m_guidTableHeader->HeaderSize, sizeof(m_guidTableHeader->HeaderSize));
    printf("\n");
    printf("%-17s", "HeaderCRC32: ");
    print_hex_chars((uint8_t *)&m_guidTableHeader->HeaderCRC32, sizeof(m_guidTableHeader->HeaderCRC32));
    printf("\n");
    printf("%-17s", "PrimaryLBA: ");
    print_hex_chars((uint8_t *)&m_guidTableHeader->PrimaryLBA, sizeof(m_guidTableHeader->PrimaryLBA));
    printf("\n");
    printf("%-17s", "BackupLBA: ");
    print_hex_chars((uint8_t *)&m_guidTableHeader->BackupLBA, sizeof(m_guidTableHeader->BackupLBA));
    printf("\n");
    printf("%-17s", "FirstUsableLBA: ");
    print_hex_chars((uint8_t *)&m_guidTableHeader->FirstUsableLBA, sizeof(m_guidTableHeader->FirstUsableLBA));
    printf("\n");
    printf("%-17s", "LastUsableLBA: ");
    print_hex_chars((uint8_t *)&m_guidTableHeader->LastUsableLBA, sizeof(m_guidTableHeader->LastUsableLBA));
    printf("\n");
    printf("%-17s", "UniqueDiskGUID: ");
    print_guid((uint8_t *)&m_guidTableHeader->UniqueDiskGUID);
    printf("\n");
    printf("%-17s", "FirstEntryLBA: ");
    print_hex_chars((uint8_t *)&m_guidTableHeader->FirstEntryLBA, sizeof(m_guidTableHeader->FirstEntryLBA));
    printf("\n");
    printf("%-17s", "NumberOfEntries: ");
    print_hex_chars((uint8_t *)&m_guidTableHeader->NumberOfEntries, sizeof(m_guidTableHeader->NumberOfEntries));
    printf("\n");
    printf("%-17s", "SizeOfEntry: ");
    print_hex_chars((uint8_t *)&m_guidTableHeader->SizeOfEntry, sizeof(m_guidTableHeader->SizeOfEntry));
    printf("\n");
    printf("%-17s", "EntriesCRC2: ");
    print_hex_chars((uint8_t *)&m_guidTableHeader->EntriesCRC2, sizeof(m_guidTableHeader->EntriesCRC2));
    printf("\n");
}