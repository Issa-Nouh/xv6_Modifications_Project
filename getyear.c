#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  printf(1, "Unix V6 was released in %d\n", getyear());
  exit();
}
