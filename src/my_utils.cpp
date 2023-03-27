#include "my_utils.h"
void *memmem(const void *l, size_t l_len, const void *s, size_t s_len)
{
    const register char *cur, *last;
    const char *cl = (const char *)l;
    const char *cs = (const char *)s;

    /* we need something to compare */
    if (l_len == 0 || s_len == 0)
    {
        return NULL;
    }

    /* "s" must be smaller or equal to "l" */
    if (l_len < s_len)
    {
        return NULL;
    }

    /* special case where s_len == 1 */
    if (s_len == 1)
    {
        return memchr(l, (int)*cs, l_len);
    }

    /* the last position where its possible to find "s" in "l" */
    last = cl + l_len - s_len;

    for (cur = cl; cur <= last; cur++)
    {
        if (cur[0] == cs[0] && memcmp(cur, cs, s_len) == 0)
        {
            return (void *)(unsigned int *)cur;
        }
    }

    return NULL;
}

void print_hex(const unsigned char *data, int size)
{
    int pos = 0;
    int offset = 0;
    while (pos < size)
    {
        printf("%6d| ", offset);
        for (int j = 0; j < 16; ++j)
        {
            printf("%.2X ", data[pos]);
            ++pos;
        }
        printf("\n");
        offset += 16;
    }
}

void get_error_string(DWORD err, CHAR msgbuf[])
{
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // flags
                   NULL,                                                       // lpsource
                   err,                                                        // message id
                   MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),                  // languageid
                   msgbuf,                                                     // output buffer
                   ERR_MSG_BUF,                                                // size of msgbuf, bytes
                   NULL);
}
void print_guid(const uint8_t *uuid)
{
    printf("%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
           uuid[0], uuid[1], uuid[2], uuid[3],
           uuid[4], uuid[5],
           uuid[6], uuid[7],
           uuid[8], uuid[9],
           uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);
}
void print_hex_chars(const uint8_t *data, int data_l)
{
    for (int i = 0; i < data_l; ++i)
    {
        printf("0x%0.2X ", data[i]);
    }
}

__int64 set_file_seek(HANDLE hf, __int64 distance, DWORD MoveMethod)
{
    LARGE_INTEGER li;

    li.QuadPart = distance;

    li.LowPart = SetFilePointer(hf, li.LowPart, &li.HighPart,
                                MoveMethod);
    DWORD error = GetLastError();
    if (li.LowPart == INVALID_SET_FILE_POINTER && error != NO_ERROR)
    {
        li.QuadPart = -1;
    }

    return li.QuadPart;
}
void print_gpt_header(const uint8_t *data)
{
    printf("%-17s", "Signature: ");
    print_hex_chars(data + 0, 8);
    printf("\n");
    printf("%-17s", "Revision: ");
    print_hex_chars(data + 8, 4);
    printf("\n");
    printf("%-17s", "HeaderSize: ");
    print_hex_chars(data + 12, 4);
    printf("\n");
    printf("%-17s", "HeaderCRC32: ");
    print_hex_chars(data + 16, 4);
    printf("\n");
    printf("%-17s", "PrimaryLBA: ");
    print_hex_chars(data + 24, 8);
    printf("\n");
    printf("%-17s", "BackupLBA: ");
    print_hex_chars(data + 32, 8);
    printf("\n");
    printf("%-17s", "FirstUsableLBA: ");
    print_hex_chars(data + 40, 8);
    printf("\n");
    printf("%-17s", "LastUsableLBA: ");
    print_hex_chars(data + 48, 8);
    printf("\n");
    printf("%-17s", "UniqueDiskGUID: ");
    print_guid(data + 56);
    printf("\n");
    printf("%-17s", "FirstEntryLBA: ");
    print_hex_chars(data + 72, 8);
    printf("\n");
    printf("%-17s", "NumberOfEntries: ");
    print_hex_chars(data + 80, 4);
    printf("\n");
    printf("%-17s", "SizeOfEntry: ");
    print_hex_chars(data + 84, 4);
    printf("\n");
    printf("%-17s", "EntriesCRC2: ");
    print_hex_chars(data + 88, 4);
    printf("\n");
}