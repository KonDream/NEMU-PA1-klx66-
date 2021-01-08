#include "common.h"
#include "memory/cache.h"
#include "burst.h"
#include <string.h>
#include "nemu.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

extern uint8_t current_sreg;

/* Memory accessing interfaces */

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg_id){
	if(cpu.cr0.protect_enable == 0)
		return addr;
	else
		return cpu.sreg[sreg_id].base + addr;
}

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	int l1_1st_line = read_cache1(addr);
	uint32_t offset = addr & (Cache_Block_L1_Size - 1);
	uint8_t ret[BURST_LEN << 1];
	if (offset + len > Cache_Block_L1_Size){
		int l1_2nd_line = read_cache1(addr + Cache_Block_L1_Size - offset);
		memcpy(ret,cache1[l1_1st_line].data + offset,Cache_Block_L1_Size - offset);
		memcpy(ret + Cache_Block_L1_Size - offset,cache1[l1_2nd_line].data,len - (Cache_Block_L1_Size - offset));
	}else {
		memcpy(ret,cache1[l1_1st_line].data + offset,len);
	}

	int tmp = 0;
	uint32_t ans = unalign_rw(ret + tmp, 4) & (~0u >> ((4 - len) << 3));
	return ans;
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	write_cache1(addr,len,data);
	//dram_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	uint32_t now_offset = addr & 0xfff;
	if (now_offset + len -1 > 0xfff){
		// Assert(0,"Cross the page boundary");
		size_t l = 0xfff - now_offset + 1;
		uint32_t addr_r = lnaddr_read(addr,l);
		uint32_t addr_l = lnaddr_read(addr + l,len - l);
		uint32_t val = (addr_l << (l << 3)) | addr_r;
		return val;
	}else {
		//hwaddr_t hwaddr = page_translate(addr);
		return hwaddr_read(addr,len);
	}
	//return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	uint32_t now_offset = addr & 0xfff;
	if (now_offset + len - 1 > 0xfff){
		// Assert(0,"Cross the page boundary");
		size_t l = 0xfff - now_offset + 1;
		lnaddr_write(addr,l,data & ((1 << (l << 3)) - 1));
		lnaddr_write(addr + l,len - l,data >> (l << 3));
	}else {
		//hwaddr_t hwaddr = page_translate(addr);
		hwaddr_write(addr, len, data);
	}
	//hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, len, current_sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, len, current_sreg);
	lnaddr_write(lnaddr, len, data);
}

