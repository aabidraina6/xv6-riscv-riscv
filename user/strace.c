#include "../kernel/types.h"
#include "user.h"

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
    panic("Expected format: strace mask command [args]");
  }
  
  trace(atoi(argv[1]));
  exec(argv[2],&argv[2]);
  exit(0);
}
