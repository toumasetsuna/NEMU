#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include <stdint.h>


enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */


typedef struct {
  union{
  union GPR {
    uint32_t _32;
    uint16_t _16;
    uint8_t _8[2];
  } gpr[8];

  /* Do NOT change the order of the GPRs' definitions. */

  /* In NEMU, rtlreg_t is exactly uint32_t. This makes RTL instructions
   * in PA2 able to directly access these registers.
   */
  struct{rtlreg_t eax, ecx, edx, ebx, esp, ebp, esi, edi;};
  };
  vaddr_t eip;
  struct Eflag{
    unsigned int CF: 1;
    unsigned int   : 5;
    unsigned int ZF: 1;
    unsigned int SF: 1;
    unsigned int   : 1;
    unsigned int IF: 1;
    unsigned int   : 1;
    unsigned int OF: 1;
  } eflag;
  struct IDTR{
    uint16_t limit;
    uint32_t base;
  } idtr;
  uint32_t CS;
  union{
  struct {
    unsigned int :31;
    unsigned int PG:1;
  };
  uint32_t val; 
  }cr0;
  union{
  struct {
    unsigned int PDBR:20;
  };
  uint32_t val; 
  }cr3;
} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
  assert(index >= 0 && index < 8);
  return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

static inline const char* reg_name(int index, int width) {
  assert(index >= 0 && index < 8);
  switch (width) {
    case 4: return regsl[index];
    case 1: return regsb[index];
    case 2: return regsw[index];
    default: assert(0);
  }
}
static void print_reg_info(){
  for(int j=0;j<8;j++){
    printf("%s: 0x%08x\t",reg_name(j,4),reg_l(j));
    printf("%s: 0x%08x\t",reg_name(j,2),reg_w(j));
    printf("%s: 0x%08x\n\n",reg_name(j,1),reg_b(j));
  }
  printf("eip: 0x%08x\n",cpu.eip);    
  printf("eflag: 0x%08x\n",cpu.eflag);
  printf("idtr: 0x%08x\n",cpu.idtr.base);

}
#endif
