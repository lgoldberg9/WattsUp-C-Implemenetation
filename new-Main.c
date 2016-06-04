#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MAX_STRING_LEN 65536

const static char *LW = "#L,W,3,E,,1;";
const static char *RW = "#R,W,0;";
const static char *LR = "#L,R,0;";
const static char *dir = "/dev/ttyUSB0";

int main(int argc, char **argv) {

  int fd = 0;
  int ret = 0;
  int intv = 10;

  fd = open(dir, O_RDWR);

  if (fd == -1) {
    perror("failure: ");
  }

  printf("fd: %d\n", fd);
  char buf[MAX_STRING_LEN];
  char line[MAX_STRING_LEN];
  char* word;
  memset(&buf[0], 0, sizeof(buf));

  // write(fd, LR, strlen(LR));
  // printf("wrote LR\n");
  
  // for (int j = 0; j < intv; j++) {
  //   read(fd, buf, MAX_STRING_LEN);
  //   printf("j = %d: ", j);
  //   for (int i = 0; i < MAX_STRING_LEN; i++) {
  //     fprintf(stdout, "%c", buf[i]);
  //   }
  //   printf("\n");
  // }

  write(fd, RW, strlen(RW));
  printf("wrote RW\n");
  
  for (int j = 0; j < intv; j++) {
    
    read(fd, buf, MAX_STRING_LEN);
    printf("j = %d: ", j);
    for (int i = 0; i < MAX_STRING_LEN; i++) {
      //fprintf(stdout, "%c", buf[i]);
      line[i] = buf[i];
    }

    word = strtok(line, ",");
    for (int m=0; m<=5 || word != NULL; m++){
      switch(m){
        case 3: printf("Amps = %s\t", word);
        break;
        case 4: printf("Volts = %s\t", word);
        break;
        case 5: printf("Power = %s\t", word);
        break;
        default : break;
      }
      word = strtok(NULL, ",");
    }
    
    printf("\n");
    usleep(1000000);
  }

  // write(fd, LW, strlen(LW));
  // printf("wrote LW\n");

  // for (int j = 0; j < intv; j++) {
  //   read(fd, buf, MAX_STRING_LEN);
  //   printf("j = %d: ", j);
  //   for (int i = 0; i < MAX_STRING_LEN; i++) {
  //     fprintf(stdout, "%c", buf[i]);
  //   }
  //   printf("\n");
  // }
  
  return 0;

}
