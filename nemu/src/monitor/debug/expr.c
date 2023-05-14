#include "memory/memory.h"
#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
enum {
  TK_NOTYPE = 256, TK_EQ, TK_INT, TK_REG
  /* TODO: Add more token types */
};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"\\-", '-'},         // 
  {"\\*", '*'},
  {"\\/", '/'},
  {"==", TK_EQ},         // equal
  {"[1-9][0-9]*|0|0x[0-9a-f]+", TK_INT},
  {"\\$[a-z]+", TK_REG},
  {"\\(",'('},
  {"\\)",')'}
  };

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;
  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  int val;
  bool if_unary;
  int level;
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;
  nr_token = 0;
  printf("begin make tokens\n");
  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        printf("1 token\n");
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;
        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          case '+':
          case '-':
            tokens[nr_token].type=rules[i].token_type;
            tokens[nr_token++].level=1;
            break;
          case '*':
          case '/':
            tokens[nr_token].type=rules[i].token_type;
            tokens[nr_token++].level=1;
            break;
          case TK_EQ:
          case '(':
          case ')':
            tokens[nr_token].type=rules[i].token_type;
            tokens[nr_token++].level=2;
            break;
          case TK_INT:
            tokens[nr_token].type=rules[i].token_type;
            if(e[position]==0)sscanf(&e[position],"%x",&tokens[nr_token++].val);
            else sscanf(&e[position],"%d",&tokens[nr_token++].val);
            break;
          case TK_REG:
            tokens[nr_token].type=rules[i].token_type;
            for(int i=0;i<8;i++){
              if(strncmp(&e[position+1],regsl[i],3)==0){
                tokens[nr_token].val=reg_l(i);
                break;
              }
              if(strncmp(&e[position+1],regsb[i],3)==0){
                tokens[nr_token].val=reg_b(i);
                break;
              }
              if(strncmp(&e[position+1],regsw[i],2)==0){
                tokens[nr_token].val=reg_w(i);
                break;
              }
            }
            /*if(strncmp(&e[position+1],"eip",3)==0){
              tokens[nr_token].val=cpu.eip;
            }*/
            nr_token++;
            break;
          default:
            assert(0);
        }
        position += substr_len;
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
    
  }

  return true;
}
typedef struct {
  Token stack[50];
  int height;
} STACK;
STACK stack,op_stack,num_stack;
bool stack_empty(){
  return (stack.height==0);
}
bool op_empty(){
  return (op_stack.height==0);
}

void push(STACK* stack,Token tk){
  stack->stack[stack->height++]=tk;
}
Token pop(STACK*  stack){
  if(stack->height==0) assert(0);
  return stack->stack[--stack->height];
}
Token top(STACK* stack){
  if(stack->height==0) assert(0);
  return stack->stack[stack->height-1];
}
uint32_t expr(char *e, bool *success) {
  stack.height=op_stack.height=num_stack.height=0;
  if (!make_token(e)) {
    *success = false;
    printf("token not right\n");
    return 0;
  }
  printf("token right\n");
  bool if_unary=true;
  for(int i=0;i<nr_token;i++){
    if(tokens[i].type==TK_REG||tokens[i].type==TK_INT){
      push(&stack,tokens[i]);
      if_unary=false;
    }
    if(tokens[i].type=='('){
       push(&op_stack,tokens[i]);
       if_unary=true;
    }
    if(tokens[i].type==')'){
      while(!op_empty()){
        Token t=top(&op_stack);
        pop(&op_stack);
        if(t.type=='(') break;
        push(&stack,t); 
      }
      if_unary=false;
    }
    if(tokens[i].type=='+'||tokens[i].type=='-'||tokens[i].type=='*'||tokens[i].type=='/'||tokens[i].type==TK_EQ){
       Token cur_op=tokens[i];
       cur_op.if_unary=if_unary;
      while(!op_empty()
        &&(!cur_op.if_unary&&cur_op.level>=top(&op_stack).level)
        &&(cur_op.if_unary&&cur_op.level>top(&op_stack).level)
      ){
        Token t=top(&op_stack);
        push(&stack,t);
        pop(&op_stack);
      }
      push(&op_stack,cur_op);
      if_unary=true;
    }
  }
  while(!op_empty()){
    Token t=top(&op_stack);
    if(t.type=='(') break;
    push(&stack,t);
    pop(&op_stack);
  }
  /* TODO: Insert codes to evaluate the expression. */
  for(int i=0;i<stack.height;i++){
    Token cur=stack.stack[i];
    if(cur.type==TK_REG||cur.type==TK_INT){
      push(&num_stack,cur);
    }else {
      if(cur.if_unary){
        Token t=top(&num_stack);
        pop(&num_stack);
        if(cur.type=='-') t.val=-t.val;
        if(cur.type=='+') t.val=t.val;
        if(cur.type=='*') t.val=vaddr_read(t.val,4);
        push(&num_stack,t);

      }else{
        Token t1=top(&num_stack);
        pop(&num_stack);
        Token t2=top(&num_stack);
        pop(&num_stack);
        if(cur.type=='-') t1.val=t2.val-t1.val;
        if(cur.type=='+') t1.val=t2.val+t1.val;
        if(cur.type=='*') t1.val=t2.val*t1.val;
        if(cur.type=='/') {
          if(t1.val==0) {
            *success=false;
            return 0;
          }
          t1.val=t2.val/t1.val;
        }
        if(cur.type==TK_EQ) t1.val=(t2.val==t1.val);
        push(&num_stack,t1);
      }
    }
  }
  Token t=top(&num_stack);
  pop(&num_stack);
  if(num_stack.height==0)*success=true;
  printf("%d\n",t.val);
  return t.val;
}
