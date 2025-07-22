// clone_endable_test.c
#include "types.h"
#include "stat.h"
#include "user.h"

volatile int counter = 0;   // proves shared memory
volatile int done    = 0;   // tells worker when to stop

void
worker(void *arg)
{
  int id = *(int *)arg;
  printf(1, "thread %d says hello!\n", id);

  // Busy-work: bump counter until the parent says we're done
  while(!done) {
    counter++;
    // yield so the parent actually runs
    sleep(1);
  }

  // Parent will exit the whole process; just fall through
  // (DON'T call exit() here!)
  for(;;) ;           // should never reach, but keeps GCC happy
}

int
main(void)
{
  int id = 42;

  // page-aligned 4 kB stack
  void *stack = malloc(4096);
  if(stack == 0 || ((uint)stack & 4095)) {
    printf(1, "stack alloc/alignment failed\n");
    exit();
  }

  int tid = clone(worker, &id, stack);
  if(tid < 0){
    printf(1, "clone failed\n");
    exit();
  }

  printf(1, "parent spawned tid %d (pid %d)\n", tid, getpid());

  // Show the counter for ~5 s (≈ 500 ticks)
  for(int i = 0; i < 25; i++){
    printf(1, "counter = %d\n", counter);
    sleep(20);        // ≈ 200 ms per loop
  }

  // Tell worker to stop, give it one tick to notice
  done = 1;
  sleep(1);

  printf(1, "parent exiting – test finished\n");
  exit();             // kills entire process, including worker
}
