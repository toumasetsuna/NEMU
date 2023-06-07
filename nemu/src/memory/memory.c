#include "device/mmio.h"
#include "nemu.h"

#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];
paddr_t page_translate(vaddr_t addr){
  if(~cpu.cr0.val&0x80000000)
    return addr;
  uint32_t t0=addr>>22;
  uint32_t t1=(addr<<10)>>22;
  uint32_t t2=(addr<<20)>>20;
  uint32_t pde=paddr_read((cpu.cr3.val&~0xfff)+4*t0,4);
  if(!(pde&1)) Log("invalid vaddr: 0x%08x",addr);
  assert(pde&1);
  uint32_t pte=paddr_read((pde&~0xfff)+4*t1, 4);
  if(!(pte&1)) {
    Log("invalid vaddr: 0x%08x",addr);
    Log("invalid pde: 0x%08x",pde );
    Log("invalid pte: 0x%08x",pte);
  }
  assert(pte&1);
  uint32_t ans=(pte&~0xfff)+t2;
  //if(addr!=ans) Log("vaddr 0x%08x to paddr 0x%08x",addr,ans);
  //assert(addr==ans);
  return ans;
}
/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len) {
  if(is_mmio(addr)==-1) return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
  else return mmio_read(addr, len, is_mmio(addr));
}

void paddr_write(paddr_t addr, int len, uint32_t data) {
  if(is_mmio(addr)==-1)  memcpy(guest_to_host(addr), &data, len);
  else mmio_write(addr, len,data, is_mmio(addr));
}

uint32_t vaddr_read(vaddr_t addr, int len) {
  paddr_t addr0=page_translate(addr);
  //assert(addr0==addr);
  paddr_t addr1=page_translate(addr+len-1);
  assert(addr1-addr0==len-1);
  return paddr_read(addr0, len);
}

void vaddr_write(vaddr_t addr, int len, uint32_t data) {
  paddr_t addr0=page_translate(addr);
  //assert(addr0==addr);
  paddr_t addr1=page_translate(addr+len-1);
  assert(addr1-addr0==len-1);
  paddr_write(addr0, len, data);
}
