#ifndef __CACHE_H__
#define __CACHE_H__

#define Cache_Block_L1_Size 64 // cache block 存储空间的大小为 64B
#define Cache_L1_Size 64 * 1024   // cache 存储空间的大小为 64KB
#define Cache_L1_Way_Bit 3  // 8-way set associative
#define Cache_L1_Group_Bit 7
#define Cache_L1_Block_Bit 6
#define Cache_L1_Group_Size (1 << Cache_L1_Group_Bit)
#define Cache_L1_Way_Size (1 << Cache_L1_Way_Bit)

/*
替换算法采用随机方式
write through
not write allocate
*/

#define Test

typedef struct{
    uint8_t data[Cache_Block_L1_Size]; // 64B
    uint32_t tag;
    bool valid;
}Cache_L1;

Cache_L1 cache1[Cache_L1_Size/Cache_Block_L1_Size];

#ifdef Test
int test_time;
#endif

void init_cache();
int read_cache1(hwaddr_t);
void write_cache1(hwaddr_t, size_t, uint32_t);
#endif