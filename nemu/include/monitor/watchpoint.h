#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  /* TODO: Add more members if necessary */
} WP;
void add_watch_point();
#endif
