#include "device/port-io.h"
#include "monitor/monitor.h"
#include <sys/time.h>

#define RTC_PORT 0x48   // Note that this is not the standard
extern void dev_raise_intr(void);
void rtc_io_handler(ioaddr_t addr, int len, bool is_write);
static uint32_t *rtc_port_base;
void timer_intr() {
  static int num=0;
  num++;
  if(num!=10) return;
  num=0;
  static uint32_t old=5;
  static uint32_t mytime=5;
  if (nemu_state == NEMU_RUNNING) {
    struct timeval now;
    gettimeofday(&now, NULL);
    uint32_t seconds = now.tv_sec;
    uint32_t useconds = now.tv_usec;
    
    mytime= seconds * 1000 + (useconds + 500) / 1000;
    Log("mytime: %ud",mytime);
    Log("old: %ud",old);
    if(mytime-old>1){
       old=mytime;
       dev_raise_intr();
       Log("sent clock");
    }
  }
}



void rtc_io_handler(ioaddr_t addr, int len, bool is_write) {
  if (!is_write) {
    struct timeval now;
    gettimeofday(&now, NULL);
    uint32_t seconds = now.tv_sec;
    uint32_t useconds = now.tv_usec;
    rtc_port_base[0] = seconds * 1000 + (useconds + 500) / 1000;
  }
}

void init_timer() {
  rtc_port_base = add_pio_map(RTC_PORT, 4, rtc_io_handler);
}
