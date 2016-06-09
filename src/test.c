#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

long fib(long x) {
  if (x == 0)
    return 0;
  else if
    (x == 1)
    return 1;
  else
    return fib(x-1) + fib(x-2);
}

void *WattsUp()
{
  system("bin/WattsUp -n 20 -l -o data/outfile.log -s 0.5");
}

int main(void) {
  pthread_t thread;
  
  if (pthread_create(&thread, NULL, WattsUp, NULL)) {
    fprintf(stderr, "Error creating thread\n");
    return 1;
  }
  printf("%ld\n", fib(45));
  return 0;
}
