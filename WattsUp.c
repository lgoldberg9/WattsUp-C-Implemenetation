#define _GNU_SOURCE
#include "ArrayList.h"
#include "Utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h> /* Text animation lib */
#include <sys/utsname.h> /* Sys information lib */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <time.h>

WattsUp* initialize_wattsup(bool sim, char *port, int interval) {
  WattsUp *meter = (WattsUp*) malloc(sizeof(WattsUp));
  if (sim) {
    meter->serial_descr = open(port, O_RDWR);
  } else {
    int file_descr;
    if ((file_descr = open(port, O_RDWR)) != -1) {
      meter->serial_descr = file_descr;
      struct termios serial_options;
      tcgetattr(meter->serial_descr, &serial_options);
      cfsetispeed(&serial_options, B115200);
      cfsetospeed(&serial_options, B115200);
      tcsetattr(file_descr, TCSANOW, &serial_options);
    } else {
      char *error = "initialize_wattsup: Unable to open %s";
      asprintf(&error, port);
      perror(error);
      free(error); 
      exit(EXIT_FAILURE);
    }
  }
  meter->logfile = NULL;
  meter->interval = interval;
  meter->time = (array_list*) malloc(sizeof(array_list));
  meter->power = (array_list*) malloc(sizeof(array_list));
  meter->voltage = (array_list*) malloc(sizeof(array_list));
  meter->current = (array_list*) malloc(sizeof(array_list));
  return meter;
}

void clean_wattsup(WattsUp *meter) {
  if (meter != NULL) {
    free(meter->time);
    free(meter->power);
    free(meter->voltage);
    free(meter->current);
    free(meter);
  }
}

void mode(WattsUp *meter, bool sim, char runmode) {
  if (!sim) {
    char *str_runmode;
    if ((asprintf(&str_runmode, "#L,W,3,%d,,%d", runmode, meter->interval)) != -1) {
      fprintf(stdout, "got here1\n");
      write(meter->serial_descr, str_runmode, sizeof(str_runmode));
      fprintf(stdout, "got here2\n");
      if (runmode == INTERNAL_MODE) {
	fprintf(stdout, "got here3\n");
	char *str_fullhandling;
	if ((asprintf(&str_fullhandling, "#0,W,1,%d", FULLHANDLING)) != -1) {
	  fprintf(stdout, "got here4\n");
	  write(meter->serial_descr, str_fullhandling, sizeof(str_fullhandling));
	  free(str_fullhandling);
	} else {
	  fprintf(stdout, "Failed to malloc space in mode.\n");
	  exit(EXIT_FAILURE);
	}
      }
      free(str_runmode);
    } else {
      fprintf(stdout, "Failed to malloc space in mode.\n");
      exit(EXIT_FAILURE);
    }
  }
}
/*
void fetch(WattsUp *meter, bool sim) {
  if (!sim) {
    char *serial_line = NULL;
    size_t line_len = 0;
    ssize_t read;
    char initial_str[2];
    char *measure_strs[6];
    double measures[3];
    time_t rawtime;
    struct tm *info;
    
    while ((read = getline(&serial_line, &line_len, meter->serial_descr)) != -1) {
      memcpy(initial_str, serial_line, 2);
      if (strncmp(initial_str, "#d", 2) == 0) {
	asprintf(&measure_strs[0], strtok(serial_line, ","));
	for (int index = 1; index < 6; index++) {
	  asprintf(&measure_strs[index], strtok(NULL, ","));
	}
	measures[0] = atof(measure_strs[3]) / 10.0;
	measures[1] = atof(measure_strs[4]) / 10.0;
	measures[2] = atof(measure_strs[5]) / 10.0;

	time(&rawtime);
	info = localtime(&rawtime);
	
	fprintf(stdout, "%s, %.3lf, %.3lf, %.3lf", asctime(info), measures[0],
		measures[1], measures[2]);
	for (int index = 0; index < 6; index++) {
	  free(measure_strs[index]);
	}
      }
      free(serial_line);
    }
  }
}
*/
void logging(WattsUp *meter, bool sim, bool raw, char *logfile) {
  fprintf(stdout, "Logging...\n");
  if (!sim) {
    mode(meter, sim, EXTERNAL_MODE);
  }
  if (logfile != NULL) {
    meter->logfile = fopen(logfile, "w");
  }
  if (raw) {
    // do something involving try catch
  }
  
  char *serial_line = NULL;
  size_t line_len = 0;
  getline(&serial_line, &line_len, meter->serial_file);
  int n = 0;

  /* Begin ncurses mode. */
  /*
  initsrc();
  noecho();
  cbreak();
  */
  
}
