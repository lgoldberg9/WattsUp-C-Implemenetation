#include "WattsUp.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h> /* Text animation lib */
#include <sys/utsname.h> /* Sys information lib */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

WattsUp* initialize_wattsup(bool sim, char *port, int interval) {
  WattsUp *meter = (WattsUp*) malloc(sizeof(WattsUp));
  if (sim) {
    meter->serial_file = fopen(port, 'r');
  } else {
    if ((fd = open(port, O_RDONLY)) != -1) {
      meter->serial_file = NULL;
    } else {
      perror("initialize_wattsup: Unable to open %s - ", port);
      exit(EXIT_FAILURE);
    }
  }
  meter->logfile = NULL;
  meter->interval = interval;
  meter->time = (array_list*) malloc(sizeof(array_list));
  meter->power = (array_list*) malloc(sizeof(array_list));
  meter->voltage = (array_list*) malloc(sizeof(array_list));
  meter->current = (array_list*) malloc(sizeof(array_list));
  return *meter;
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

void mode(wattsUp *meter, bool sim, char runmode) {
  if (!sim) {
    char *str_runmode;
    if ((asprintf(str_runmode, "#L,W,3,%s,,%d", runmode, meter->interval)) != NULL) {
      fwrite(str_runmode, sizeof(char), sizeof(str_runmode), meter->serial_file);
      if (runmode == INTERNAL_MODE) {
	char *str_fullhandling;
	if ((asprintf(str_fullhandling, "#0,W,1,%d", FULLHANDLING)) != NULL) {
	  fwrite(str_fullhandling, sizeof(char), sizeof(str_fullhandling),
		 meter->serial_file);
	}
	free(str_fullhandling);
      }
    }
    free(str_runmode);
  }
}
