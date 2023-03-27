#include "ntfs.h"
#include "my_utils.h"
NTFS::NTFS() : FS()
{
}
int NTFS::Open(const WCHAR *filename, __int64 start_pos)
{
    // открывает файл filename, NTFS начинается со смещения start_pos от начала файла
    m_hDisk = CreateFileW(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (m_hDisk == INVALID_HANDLE_VALUE)
    {
        m_dwLastError = GetLastError();
        return -1;
    }

    DWORD bytes_read;
    __int64 offset{start_pos};
    __int64 cur_offset;
    cur_offset = set_file_seek(m_hDisk, offset, FILE_BEGIN);
    if (cur_offset == -1)
    {
        m_dwLastError = GetLastError();
        return -1;
    }
    // if (ReadFile(m_hDisk, buf, SECTOR_SIZE, &bytes_read, NULL) == FALSE)
    // {
    //     m_dwLastError = GetLastError();
    //     return -1;
    // }

    // заполнение структуры BOOT_NTFS по указателю boot_ntfs

    return 0;
}
int NTFS::Close()
{
    return 0;
}
int NTFS::ReadClusters(__int64 first_cluster, __int64 number_of_clusters, uint8_t *buf)
{
    // Читает в buf number_of_clusters кластеров начиная с first_cluster.
    // За память buf отвечает пользователь функции.
    if (buf == NULL)
    {
        // return number_of_clusters*<размер_кластера>
    }

    return 0;
}