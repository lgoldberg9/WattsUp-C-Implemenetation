#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int foo = 0;

long fib(long x) {
  if (x == 0)
    return 0;
  else if (x == 1)
    return 1;
  else
    return fib(x-1) + fib(x-2);
}

void *WattsUp() {
  int noob = system("bin/WattsUp -n 20 -l -s 1");
  printf("noob: %d\n", noob);
  foo = 1;
}

int main(void) {
  pthread_t thread;

  if (pthread_create(&thread, NULL, WattsUp, NULL)) {
    fprintf(stderr, "Error creating thread\n");
    exit(EXIT_FAILURE);
  }
  
  fib(45);

  pthread_join(thread, NULL);
  
  exit(EXIT_SUCCESS);
}
