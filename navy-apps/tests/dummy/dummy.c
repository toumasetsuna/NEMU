#include <stdint.h>

#define SYS_none 0
extern int _syscall_(int, intptr_t, intptr_t, intptr_t);

int main() {
  // this system call will trap into OS but do nothing
  char a[20]="hello how are you";
  int r = _syscall_(3, 1, a, 10);
  return 0;
}
