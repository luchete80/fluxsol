//https://stackoverflow.com/questions/15463621/executing-a-function-at-specific-intervals

#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h> // for sleep() and usleep()

void *timerthread(void *timer_parms) {  

  struct itimerspec new_value;
  int max_exp, fd;
  struct timespec now;
  uint64_t exp;
  ssize_t s;

  struct timer_params *p =(struct timer_params*)timer_parms;

  printf("starttimer Start\n");
  /* Create a CLOCK_REALTIME absolute timer with initial
     expiration and interval as specified in command line */
  if (clock_gettime(CLOCK_REALTIME, &now) == -1)
    handle_error("clock_gettime");

  new_value.it_value.tv_sec = now.tv_sec;
  new_value.it_value.tv_nsec = now.tv_nsec + p->tv_nsec;
  new_value.it_interval.tv_sec = p->tv_sec;
  new_value.it_interval.tv_nsec = p->tv_nsec;
  //max_exp = 5; //No of times

  fd = timerfd_create( CLOCK_REALTIME , 0);
  if (fd == -1)
    handle_error("timerfd_create");

  if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL) == -1)
    handle_error("timerfd_settime");

  printf("timer started\n");

  while(1) // keep checking
  {
    s = read(fd, &exp, sizeof(uint64_t));
    if (s != sizeof(uint64_t))
      handle_error("read");
    Processfunction(); // Say after X seconds call this function
  }
  return NULL;
}

// int main() {

  // struct timer_params timer_params_obj;
  // int res;void *thread_result;
  // timer_params_obj.tv_sec = 10; 
  // //timer_params_obj.tv_nsec = 10000000  ; //10ms
  // timer_params_obj.tv_nsec = 0  ; 

  // pthread_t pt;
  // pthread_create(&pt, NULL, timerthread, &timer_params_obj);
  // // thread is running and will call Processfunction() every 10 sec
// }