// threadtest.c
#include "types.h"
#include "stat.h"
#include "user.h"
#define NUM_THREADS 2
#define PGSIZE 4096      // 4 KB

struct targs {
  int id;
  int loops;
};

void
worker(void *arg)
{
  struct targs *a = (struct targs*)arg;// Unpack the argument block
  for(int i = 0; i < a->loops; i++){
    printf(1, "thread %d loop %d\n", a->id, i);
    sleep(1);
  }
  /* Inform the console we’re about to stop, then *exit the thread*.
   * In xv6 a thread calls the same exit() that a process would.
   * The kernel knows it’s a thread by noticing that it shares its
   * page directory (address space) with its parent.
   */
  printf(1, "thread %d done\n", a->id);
  exit();                     // thread exits
}

int
main(void)
{
  struct targs args[NUM_THREADS];
  void *stacks[NUM_THREADS];

  // --- create two threads ---

  for(int i = 0; i < NUM_THREADS; i++){
    args[i].id    = i + 1;
    args[i].loops = 3;
    /* stacks[i] = sbrk(PGSIZE); grabs one clean 4 KB page from the heap to serve as 
    the private scratch-pad (stack) for thread i, 
    ensuring threads don’t step on each other’s data.*/

    /* Allocate a brand-new stack with sbrk().
        sbrk(PGSIZE) bumps the user heap up by 4096 bytes and
        returns the *starting address* of that fresh page.
        Because we always allocate a full page per thread we
        avoid stack collisions between threads.
     */
    stacks[i]     = sbrk(PGSIZE);           

    /* Call clone():
     *  clone(start_fn, arg, stack)
     *  -----------------------------------
     *  • Returns 0 in the CHILD, >0 in the PARENT, -1 on error.
     *  • We care about the error case; otherwise we just keep
     *    looping to launch the next thread.
     */
    if(clone(worker, &args[i], stacks[i]) < 0){
        printf(1, "clone failed for thread %d\n", i + 1);
        exit();
  }
}


  // --- join each thread ---
  for(int i = 0; i < NUM_THREADS; i++){
    int pid = join();                     // waits for ONE finished thread
    printf(1, "parent joined thread (pid %d)\n", pid);
  }

  printf(1, "all threads successfully joined; parent exiting\n");
  sleep(1000);
  exit();
}
