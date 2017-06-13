//https://gist.github.com/cirocosta/ab2b73b2e1bb34b83f52

/*
 * ORIGINAL HEADER
 *
 * sigev_thread.c
 *
 * Demonstrate use of the SIGEV_THREAD signal mode to handle
 * signals by creating a new thread.
 *
 * Special notes: This program will not compile on Solaris 2.5.
 * It will compile on Digital UNIX 4.0 but will not work.
 * Digital UNIX 4.0c fixes SIGEV_THREAD, and sources inform me
 * that Solaris 2.6 will also fix SIGEV_THREAD. To try this on
 * Solaris 2.5, remove the "#ifdef sun" conditionals in main.
 *
 * Original:
 * http://ptgmedia.pearsoncmg.com/images/0201633922/sourcecode/sigev_thread.c
 */

/**
 * CIRO's Header
 * This is a slightly modified version of the original one so that
 * i can create multiple timers and trigger then in a more
 * handy way for my project.
 */

#include <pthread.h>
//#include <sys/signal.h>
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>

#define MILLION 1000000

void err_abort(int status, char* message);

void errno_abort(char* message);

pthread_mutex_t mutex;
pthread_cond_t cond ;
int counter;

/*
 * Thread start routine to notify the application when the
 * timer expires. This routine is run "as if" it were a new
 * thread, each time the timer expires.
 *
 * When the timer has expired 5 times, the main thread will
 * be awakened, and will terminate the program.
 */
void timer_thread(union sigval arg);
void create_timer(unsigned i);
