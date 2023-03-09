#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  struct watchpoint *back;
  int val;
  char args[100];
  /* TODO: Add more members if necessary */
} WP;
void add_watch_point();
bool check_watch_points();
void delete_watch_point(int NO);
void show_watch_points();
#endif
