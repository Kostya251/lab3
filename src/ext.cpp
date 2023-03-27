#include "ext.h"
Ext::Ext() : FS()
{
}
Ext::~Ext()
{
}
int Ext::Open(const WCHAR *filename, __int64 start_pos) { return 0; };
int Ext::Close() { return 0; };
int Ext::ReadClusters(__int64 first_cluster, __int64 number_of_clusters, uint8_t *buf) { return 0; };