#include "resources.h"
#include "gpt.h"
#include "my_utils.h"
#include <wchar.h>
int main()
{
    DWORD error;
    uint8_t err_buf[ERR_MSG_BUF];
    GPT gpt;
    // int result_code = gpt.open(L"D:\\Projects\\FS\\archTest.vmdk");
    int result_code = gpt.open(L"\\\\.\\PhysicalDrive0");
    char buf[ERR_MSG_BUF];
    printf("result_code: %d\n", result_code);
    if (result_code != 0)
    {
        printf("err_code: %d\n", result_code);
        if (result_code == -1)
        {
            get_error_string(gpt.GetLastError_(), buf);
            printf("%s", buf);
        }
        exit(1);
    }
    printf("Parsing done.\n");
    printf("Primary GPT Header:\n");
    gpt.PrintGuidTableHeader();
    printf("\n");
    const GPTPartition *p;
    for (int i = 0; i < gpt.GetNumberOfEntries(); ++i)
    {
        p = gpt.GetEntryByIndex(i);
        printf("Entry%d:\n", i);
        printf("GUID: ");
        print_guid((uint8_t *)&p->entry.PartitionTypeGUID);
        printf("\n");
        printf("Partition Name: ");
        for (int j = 0; j < GPT_PART_NAME_LEN; ++j)
        {
            printf("%c", p->entry.PartitionName[j]);
        }
        printf("\n");
        printf("First LBA: %lld\n", p->entry.FirstLBA);
        printf("Last LBA: %lld\n", p->entry.LastLBA);
        printf("offset to first sector: %lld bytes\n", p->GetPos());
        printf("offset to last sector: %lld bytes\n", p->GetEnd());
        printf("size: %lld bytes\n", p->GetSize());
        printf("\n");
    }

    // printf("\n");
    // uint32_t linux_data = 0x0FC63DAF;
    // int index = gpt.FindEntryIndexByGUID(linux_data);
    // printf("index: %d\n", index);
    // const GPTPartition *p = gpt.GetEntryByIndex(index);
    // printf("++\n");
    // print_guid((uint8_t *)&p->entry.PartitionTypeGUID);
    // printf("\n--\n");
    // for (int i = 0; i < wcslen((wchar_t *)p->entry.PartitionName); ++i)
    //     wprintf(L"%c", p->entry.PartitionName[i]);
    // printf("\n");
    // printf("First LBA: %lld\n", p->entry.FirstLBA);
    // printf("Last LBA: %lld\n", p->entry.LastLBA);
    // printf("offset to first sector: %d bytes\n", p->GetPos());
    // printf("offset to last sector: %d bytes\n", p->GetEnd());
    // printf("size: %d bytes\n", p->GetSize());
    return 0;
}