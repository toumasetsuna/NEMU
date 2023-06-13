#include "proc.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC];
static int nr_proc = 0;
PCB *current = NULL;
PCB *current_game=&pcb[1];
uintptr_t loader(_Protect *as, const char *filename);

void load_prog(const char *filename) {
  int i = nr_proc ++;
  _protect(&pcb[i].as);

  uintptr_t entry = loader(&pcb[i].as, filename);

  // TODO: remove the following three lines after you have implemented _umake()
  //_switch(&pcb[i].as);
 // current = &pcb[i];
 // ((void (*)(void))entry)();

  _Area stack;
  stack.start = pcb[i].stack;
  stack.end = stack.start + sizeof(pcb[i].stack);

  pcb[i].tf = _umake(&pcb[i].as, stack, stack, (void *)entry, NULL, NULL);

}
_RegSet* schedule(_RegSet *prev) {
  Log("schedule");
  current->tf= prev;
  //current=(current==current_game?&pcb[0]:current_game);
  current->tf=&pcb[1];
  _switch(&current->as);
  return current->tf;
}
void change_game(){
  /*Log("change game");
  PCB* old=current_game;
  if(current_game==&pcb[1]) current_game=&pcb[2];
  else current_game=&pcb[1]; */
}
