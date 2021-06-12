/*
   File: ut.c
   Created on: 19/04/18
   Written by: Eli Kessem
   ID:  066127549 
 
   Description: this file implements a simple library for creating & scheduling 
                user-level threads. 

 */

#ifndef _UT_H
#define _UT_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "ut.h"



static ut_slot thread_table; // the threads table
static int table_size; // threads table size
static ucontext_t curr_context; // temporary context for more comfortable work

static tid_t curr_thread_num; // contains the current thread number, for swapcontext
static tid_t prev_thread_num; // contains the previous thread number, for swapcontext
static tid_t tid = 0; // a thread ID


/*  Initialize the library data structures. Create the threads table  */
int ut_init(int tab_size){
  if (tab_size < MIN_TAB_SIZE || tab_size > MAX_TAB_SIZE)
    tab_size = MAX_TAB_SIZE;
  table_size = tab_size; // saving table size
  thread_table = (ut_slot) malloc ((table_size) * sizeof(ut_slot_t));
  if (thread_table == NULL)
    return SYS_ERR;
  return 0;
}


/*  Add a new thread to the threads table  */
tid_t ut_spawn_thread(void (*func)(int), int arg){
  ut_slot curr_slot;
  int stack_size = STACKSIZE;
  if (tid >= table_size)
    return TAB_FULL;
  curr_slot = &thread_table[tid]; // creating a pointer to the current thread, for more comfortable work
  curr_slot->arg = arg;
  curr_slot->func = func;
  curr_slot->vtime = 0;

  if (getcontext(&curr_slot->uc) == -1)
      return SYS_ERR;

  curr_slot->uc.uc_link = &curr_context;
  curr_slot->uc.uc_stack.ss_sp = (void *)malloc(stack_size);
  if (curr_slot->uc.uc_stack.ss_sp == NULL)
    return SYS_ERR;
  curr_slot->uc.uc_stack.ss_size = stack_size;

  makecontext(&curr_slot->uc, (void(*)(void)) func, 1, arg);

  tid++;
  return tid-1;
}


/* This is the signal handler which swaps between the threads. */
void handler(int signal) {

  if (signal == SIGVTALRM)
  {
    // update the vtime statistics
    thread_table[curr_thread_num].vtime += 100;
  }

  if (signal == SIGALRM)
  {
    // operates swapcontext every second
    alarm(1);
    prev_thread_num = curr_thread_num;
    curr_thread_num = (curr_thread_num +1 ) % table_size;
    if (swapcontext(&thread_table[prev_thread_num].uc, &thread_table[curr_thread_num].uc) == -1)
    {
      perror ("swapcontext in handler failed");
      exit(1);
    }
  }
}


/*  Starts running the threads, previously created by ut_spawn_thread  */
int ut_start(void){
  struct sigaction sa;
  struct itimerval itv;

  /* set up vtimer for accounting */
  itv.it_interval.tv_sec = 0;
  itv.it_interval.tv_usec = 100000;
  itv.it_value = itv.it_interval;

  /* Initialize the data structures for SIGALRM handling. */
  sa.sa_flags = SA_RESTART;
  sigfillset(&sa.sa_mask);
  sa.sa_handler = handler;

  if (sigaction(SIGALRM, &sa, NULL) < 0)
    return SYS_ERR;

  if (sigaction(SIGVTALRM, &sa, NULL) < 0)
    return SYS_ERR;

  if (setitimer(ITIMER_VIRTUAL, &itv, NULL) < 0)
    return SYS_ERR;

  alarm(1);
  curr_thread_num = 0; // program starts with thread 0
  if (swapcontext(&curr_context, &thread_table[curr_thread_num].uc) == -1)
    return SYS_ERR;

  return 0;
}

/*  Returns the CPU-time consumed by the given thread  */
unsigned long ut_get_vtime(tid_t tid){
  return thread_table[tid].vtime;
}
