#include "common.h"
#include "memory/cache.h"
#include <stdio.h>
#include <stdlib.h>
#include "burst.h"

void init_cache(){
    int i;
    for(i = 0; i < Cache_L1_Size / Cache_Block_L1_Size;i ++)
        cache1[i].valid = 0;

    test_time = 0;
}

int read_cache1(hwaddr_t addr){
    uint32_t group_idx = (addr >> Cache_L1_Block_Bit) & (Cache_L1_Group_Size - 1);
    uint32_t tag = (addr >> (Cache_L1_Group_Bit + Cache_L1_Block_Bit));
    int i, group = group_idx * Cache_L1_Way_Size;
    for(i = group + 0; i < group + Cache_L1_Way_Size; i ++){
        if (cache1[i].valid == 1 && cache1[i].tag == tag){
            #ifdef Test
                test_time += 2;//HIT in Cache1
            #endif
            return i;
        }
    }
    cache1[i].valid = 1;
    cache1[i].tag = tag;
    return i;
}