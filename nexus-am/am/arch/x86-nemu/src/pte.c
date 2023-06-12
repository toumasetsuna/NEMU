#include <x86.h>
#include "common.h"
#define PG_ALIGN __attribute((aligned(PGSIZE)))

static PDE kpdirs[NR_PDE] PG_ALIGN;
static PTE kptabs[PMEM_SIZE / PGSIZE] PG_ALIGN;
static void* (*palloc_f)();
static void (*pfree_f)(void*);

_Area segments[] = {      // Kernel memory mappings
  {.start = (void*)0,          .end = (void*)PMEM_SIZE}
};

#define NR_KSEG_MAP (sizeof(segments) / sizeof(segments[0]))

void _pte_init(void* (*palloc)(), void (*pfree)(void*)) {
  palloc_f = palloc;
  pfree_f = pfree;

  int i;

  // make all PDEs invalid
  for (i = 0; i < NR_PDE; i ++) {
    kpdirs[i] = 0;
  }

  PTE *ptab = kptabs;
  for (i = 0; i < NR_KSEG_MAP; i ++) {
    uint32_t pdir_idx = (uintptr_t)segments[i].start / (PGSIZE * NR_PTE);
    uint32_t pdir_idx_end = (uintptr_t)segments[i].end / (PGSIZE * NR_PTE);
    for (; pdir_idx < pdir_idx_end; pdir_idx ++) {
      // fill PDE
      kpdirs[pdir_idx] = (uintptr_t)ptab | PTE_P;
      // fill PTE
      PTE pte = PGADDR(pdir_idx, 0, 0) | PTE_P;
      PTE pte_end = PGADDR(pdir_idx + 1, 0, 0) | PTE_P;
      for (; pte < pte_end; pte += PGSIZE) {
        *ptab = pte;
        ptab ++;
      }
    }
  }
  
  //printf("kpdirs: 0x%08x\n",kpdirs);
  set_cr3(kpdirs);
  set_cr0(get_cr0() | CR0_PG);
}

void _protect(_Protect *p) {
  PDE *updir = (PDE*)(palloc_f());
  p->ptr = updir;
  // map kernel space
  for (int i = 0; i < NR_PDE; i ++) {
    updir[i] = kpdirs[i];
  }

  p->area.start = (void*)0x8000000;
  p->area.end = (void*)0xc0000000;
}

void _release(_Protect *p) {
}

void _switch(_Protect *p) {
  set_cr3(p->ptr);
}

void _map(_Protect *p, void *va, void *pa) {
 
  uint32_t* cr3=(uint32_t*)p->ptr;
  uint32_t vaddr=(uint32_t) va;
  uint32_t paddr=(uint32_t) pa;
  uint32_t t0=vaddr>>22;
  uint32_t t1=(vaddr<<10)>>22;
  //uint32_t t2=(vaddr<<20)>>20;
  //uint32_t u0=paddr>>22;
  uint32_t u1=(paddr>>12)<<12;
  if(!(cr3[t0]&PTE_P)){
    PTE* uptabs= (PTE*)(palloc_f());
    cr3[t0]=(uint32_t)uptabs;
     cr3[t0]=cr3[t0]|PTE_P;
  }
  cr3=(uint32_t*)(cr3[t0]&~0xfff);
  assert(!(cr3[t1]&1));
  cr3[t1]=u1;
  cr3[t1]=cr3[t1]|PTE_P;
}

void _unmap(_Protect *p, void *va) {
}

_RegSet *_umake(_Protect *p, _Area ustack, _Area kstack, void *entry, char *const argv[], char *const envp[]) {
  uint32_t* t=(uintptr_t*) ustack.end;
  t[0]=0;//start arg
  t[-1]=0;//cs
  t[-2]=8;//eflag
  t[-3]=(uint32_t)entry;//entry
  t[-4]=0;//error code
  t[-5]=0x81;//irq_id
  for(int i=-6;i>-14;i--) t[i]=0;//general purpose register
  return (_RegSet*)&t[-13];
}
