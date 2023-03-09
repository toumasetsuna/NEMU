#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include <assert.h>
#define NR_WP 32
#define INF 0xfffffff;
static WP wp_pool[NR_WP];
static WP *head, *free_;
void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i++) {
    wp_pool[i].NO = i;
    wp_pool[i].val=INF;
    wp_pool[i].next = &wp_pool[i + 1];
    wp_pool[i+1].back=&wp_pool[i];
  }
  wp_pool[NR_WP - 1].next = NULL;
  wp_pool[0].back=NULL;
  head = NULL;
  free_ = wp_pool;
}
void add_watch_point(char *args) {
  if(free_==NULL) assert(0);
  WP *t = head;
  WP* t1=free_->next;
  head = free_;
  head->next = t;
  if(t!=NULL)t->back=head;
  free_ = t1;
  free_->back=NULL;
  
}
void delete_watch_point(int NO) {
  WP *t=head;
  while(t!=NULL){
    if(t->NO==NO){
      t->val=INF;
      WP* t1=free_;
      free_=t;
      free_->next=t1;
      if(t->back!=NULL) t->back->next=t->next;
      if(t->next!=NULL) t->next->back=t->back;
      break;
    }  else t=t->next;
  }
}
bool check_watch_points() {
  WP* t=head;
  bool change=false;
  while(t!=NULL){
    //printf("watch point %d: %s\tvalue: %d\n",t->NO,t->args,t->val);
    bool success;
    int val=expr(t->args,&success);
    if(success) {
      if(val!=t->val) change=true;
      t->val=val;
    }
    t=t->next;
  }
  return change;
}
/* TODO: Implement the functionality of watchpoint */
