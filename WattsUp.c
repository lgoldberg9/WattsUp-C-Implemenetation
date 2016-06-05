#define _GNU_SOURCE
#include "Utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h> /* Text animation lib */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <time.h>

WattsUp* initialize_wattsup(flags *options) {

  WattsUp *meter = (WattsUp*) malloc(sizeof(WattsUp));
  int file_descr = 0;

  if ((file_descr = open(options->port->port_dest, O_RDWR | O_NOCTTY | O_NDELAY))
      != -1) {
    meter->serial_descr = file_descr;
    struct termios serial_options;
    tcgetattr(meter->serial_descr, &serial_options);
    cfsetispeed(&serial_options, B115200);
    cfsetospeed(&serial_options, B115200);
    tcsetattr(file_descr, TCSANOW, &serial_options);
  } else {
    perror("initialize_wattsup - Unable to open port");
    exit(EXIT_FAILURE);
  }
  /*FILE *logfile_tmp = NULL;
  if ((logfile_tmp = freopen(logfile, "w", stdout)) == NULL) {
    perror("initialize_wattsup - Unable to open file");
    exit(EXIT_FAILURE);
    } */
  meter->sample_size = options->sample->sample_size;
  meter->interval = options->interval_d;
  meter->time = (double*) malloc(sizeof(double) * meter->sample_size);
  meter->power = (double*) malloc(sizeof(double) * meter->sample_size);
  meter->voltage = (double*) malloc(sizeof(double) * meter->sample_size);
  meter->current = (double*) malloc(sizeof(double) * meter->sample_size);
  
  return meter;
}

void clean_wattsup(WattsUp *meter) {
  if (meter != NULL) {
    close(meter->serial_descr);
    fclose(stdout);
    free(meter->time);
    free(meter->power);
    free(meter->voltage);
    free(meter->current);
    free(meter);
  }
}

/*void mode(WattsUp *meter, bool sim, char runmode) {
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
  }*/
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
void logging(WattsUp *meter) {
  fprintf(stdout, "Logging...\n");  
  
  char buf[MAX_STRING_LEN];
  char line[MAX_STRING_LEN];
  char *word;
  int sample = meter->sample_size;

  memset(&buf[0], 0, sizeof(buf));

  write(meter->serial_descr, RW, strlen(RW));
  
  for (int time = 0; time < sample; time++) {

    read(meter->serial_descr, buf, sizeof(buf));
    for (int index = 0; index < MAX_STRING_LEN; index++) {
      line[index] = buf[index];
    }

    word = strtok(line, ",");
    
    if (word != NULL) {
      meter->time[time] = time * meter->interval;
      for (int word_parse = 0; word_parse < 6; word_parse++) {
	switch (word_parse) {
	case 3:
	  meter->power[time] = atof(word) / 10.0;
	  break;
	case 4:
	  meter->voltage[time] = atof(word) / 10.0;
	  break;
	case 5:
	  meter->current[time] = atof(word) / 1000.0;
	  break;
	default:
	  break;
	}
	word = strtok(NULL, ",");
      }
    } else {
      time--;
    }
    
    usleep(1000000 * meter->interval);
  }
  
  double summary_totals[3] = {0, 0, 0};
  
  fprintf(stdout, "Time\t Amps\t Volts\t Watts\n");
  for (int index = 0; index < sample; index++) {
    fprintf(stdout, "%.3lf\t%.3lf\t%.2lf\t%.3lf\n", meter->time[index],
	    meter->current[index], meter->voltage[index], meter->power[index]);
    summary_totals[0] += meter->current[index];
    summary_totals[1] += meter->voltage[index];
    summary_totals[2] += meter->power[index];
  }

  fprintf(stdout, "\n======= Summary Statistics =======\n");
  fprintf(stdout, "Total Time:\t %.3lf seconds\n", sample * meter->interval);  
  fprintf(stdout, "Average Amps:\t %.4lf A\n",
	  summary_totals[0] / sample);
  fprintf(stdout, "Average Volts:\t %.4lf V\n",
	  summary_totals[1] / sample);
  fprintf(stdout, "Average Watts:\t %.4lf W\n",
	  summary_totals[2] / sample);
  fprintf(stdout, "Total Energy:\t %.4lf J\n", summary_totals[2] * meter->interval);
  
}
