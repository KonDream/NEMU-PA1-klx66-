#include "common.h"
#include "memory/cache.h"
#include <stdio.h>
#include <stdlib.h>
#include "burst.h"
#include <time.h>

void ddr3_read_replace(hwaddr_t addr, void *data);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);
void ddr3_write_replace(hwaddr_t addr, void *data, uint8_t *mask);

void init_cache(){
    int i;
    for(i = 0; i < Cache_L1_Size / Cache_Block_L1_Size;i ++)
        cache1[i].valid = 0;

    test_time = 0;
}

int read_cache1(hwaddr_t addr){
    uint32_t group_idx = (addr >> Cache_L1_Block_Bit) & (Cache_L1_Group_Size - 1);
    uint32_t tag = (addr >> (Cache_L1_Group_Bit + Cache_L1_Block_Bit));
    uint32_t block_start = (addr >> Cache_L1_Block_Bit) << Cache_L1_Block_Bit;
    int i, group = group_idx * Cache_L1_Way_Size;
    for(i = group + 0; i < group + Cache_L1_Way_Size; i ++){
        if (cache1[i].valid == 1 && cache1[i].tag == tag){
            #ifdef Test
                test_time += 2;//HIT in Cache1
            #endif
            return i;
        }
    }

    #ifdef Test
        test_time += 200;
    #endif
    srand(time(0));
    i = group + rand() % Cache_L1_Way_Size;
        /*new content*/
    int j;
    for (j = 0;j < Cache_Block_L1_Size / BURST_LEN;j ++)
        ddr3_read_replace(block_start + BURST_LEN * j, cache1[i].data + BURST_LEN * j);
    
    cache1[i].valid = 1;
    cache1[i].tag = tag;
    //printf("%d\n", test_time);
    return i;
}

void write_cache1(hwaddr_t addr, size_t len, uint32_t data){
    uint32_t group_idx = (addr >> Cache_L1_Block_Bit) & (Cache_L1_Group_Size - 1);
    uint32_t tag = (addr >> (Cache_L1_Group_Bit + Cache_L1_Block_Bit));
    uint32_t offset = addr & (Cache_Block_L1_Size - 1);

    int i,group = group_idx * Cache_L1_Way_Size;
    for (i = group + 0;i < group + Cache_L1_Way_Size;i ++){
        if (cache1[i].valid == 1 && cache1[i].tag == tag){// WRITE HIT
            /*write through*/
            if (offset + len > Cache_Block_L1_Size)
            {
                dram_write(addr,Cache_Block_L1_Size - offset,data);
                memcpy(cache1[i].data + offset, &data, Cache_Block_L1_Size - offset);
                write_cache1(addr + Cache_Block_L1_Size - offset, len -  (Cache_Block_L1_Size - offset), data >>  (Cache_Block_L1_Size - offset));
            }
            else
            {
                dram_write(addr,len,data);
                memcpy(cache1[i].data + offset, &data, len);
                
            }
            return;
        }
    }
}