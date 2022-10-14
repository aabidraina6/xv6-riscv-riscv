#include "kernel/types.h"
#include "user/user.h"

void
panic(char *s)
{
  fprintf(2, "%s\n", s);
  exit(1);
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    panic("Expected format: setpriority priority pid");
  }

  setpriority(atoi(argv[1]), atoi(argv[2]));
  exit(0);
}