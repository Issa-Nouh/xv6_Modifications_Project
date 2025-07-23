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
  struct targs *a = (struct targs*)arg;
  for(int i = 0; i < a->loops; i++){
    printf(1, "thread %d loop %d\n", a->id, i);
    sleep(1);
  }
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
    stacks[i]     = sbrk(PGSIZE);           // <-- change here
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
  sleep(10000);
  exit();
}
