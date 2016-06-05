#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <termios.h>

#define MAX_STRING_LEN 65536
#define BAUDRATE B115200

const static char *LW = "#L,W,3,E,,1;";
const static char *RW = "#R,W,0;";
const static char *LR = "#L,R,0;";
const static char *dir = "/dev/ttyUSB0";

int main(int argc, char **argv) {

  int file_descr = 0;
  int intv = 20;

  file_descr = open(dir, O_RDWR);

  /* Setting baud rate to 115200. */
  struct termios options;
  tcgetattr(file_descr, &options);
  cfsetispeed(&options, BAUDRATE);
  cfsetospeed(&options, BAUDRATE);
  tcsetattr(file_descr, TCSANOW, &options);
  
  if (file_descr == -1) {
    perror("open failure");
    return errno;
  }

  printf("file_descr: %d\n", file_descr);
  char buf[MAX_STRING_LEN];
  char line[MAX_STRING_LEN];
  char *word;
  double amps = 0.0;
  double volts = 0.0;
  double watts = 0.0;
  double tot_amps = 0.0;
  double tot_volts = 0.0;
  double tot_watts = 0.0;
  int t = 0;
  
  memset(&buf[0], 0, sizeof(buf));

  write(file_descr, RW, strlen(RW));
  printf("wrote RW: %s\n", RW);
  usleep(2000000);
  
  for (t = 1; t <= intv; t++) {
    
    read(file_descr, buf, MAX_STRING_LEN);
    for (int i = 0; i < MAX_STRING_LEN; i++) {
      line[i] = buf[i];
    }

    word = strtok(line, ",");

    if (word != NULL) {
      
      printf("t = %d: ", t);
      for (int m = 0; m <= 5; m++) {
	switch(m) {
	case 3:
	  amps = atoi(word) / 1000.0;
	  printf("Amps = %.4lf, ", amps);
	  tot_amps += amps;
	  break;
	case 4:
	  volts = atoi(word) / 10.0;
	  printf("Volts = %.4lf, ", volts);
	  tot_volts += volts;
	  break;
	case 5:
	  watts = atoi(word) / 10.0;
	  tot_watts += watts;
	  printf("Watts = %.4lf, ", atoi(word) / 10.0);
	  printf("Energy = %.4lf", tot_watts);
	  break;
	default:
	  break;
	}
	word = strtok(NULL, ",");
      }
      
      putchar('\n');
    } else {
      t--;
    }
    usleep(1000000);
  }

  fprintf(stdout, "======= Summary Statistics =======\n");
  fprintf(stdout, "Total Time:\t %d seconds\n", t);  
  fprintf(stdout, "Average Amps:\t %.4lf A\n", tot_amps / t);
  fprintf(stdout, "Average Volts:\t %.4lf V\n", tot_volts / t);
  fprintf(stdout, "Average Watts:\t %.4lf W\n", tot_watts / t);
  fprintf(stdout, "Total Energy:\t %.4lf J\n", tot_watts);


  close(file_descr);
  return 0;

}
