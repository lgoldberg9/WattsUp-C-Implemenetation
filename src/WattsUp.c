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
  FILE *logfile_tmp;
  if (options->outfile->outfile_b) { 
    if ((logfile_tmp = fopen(options->outfile->outfile_path, "w")) == NULL) {
      perror("initialize_wattsup - Unable to open file");
      exit(EXIT_FAILURE);
    } 
  } else {
    logfile_tmp = stdout;
  }
  meter->logfile = logfile_tmp;
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
    fclose(meter->logfile);
    free(meter->time);
    free(meter->power);
    free(meter->voltage);
    free(meter->current);
    free(meter);
  }
}

void logging(WattsUp *meter) {
  fprintf(stdout, "Logging...\n");  
  
  char buf[MAX_STRING_LEN];
  char line[MAX_STRING_LEN];
  char *word;
  int sample = meter->sample_size;

  memset(&buf[0], 0, sizeof(buf));

  write(meter->serial_descr, "#R,W,0;", strlen("#R,W,0;"));
  
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
  
  fprintf(meter->logfile, "Time\t Amps\t Volts\t Watts\n");
  for (int index = 0; index < sample; index++) {
    fprintf(meter->logfile, "%.3lf\t%.3lf\t%.2lf\t%.3lf\n", meter->time[index],
	    meter->current[index], meter->voltage[index], meter->power[index]);
    summary_totals[0] += meter->current[index];
    summary_totals[1] += meter->voltage[index];
    summary_totals[2] += meter->power[index];
  }

  fprintf(meter->logfile, "\n======= Summary Statistics =======\n");
  fprintf(meter->logfile, "Total Time:\t %.3lf seconds\n",
	  sample * meter->interval);  
  fprintf(meter->logfile, "Average Amps:\t %.4lf A\n",
	  summary_totals[0] / sample);
  fprintf(meter->logfile, "Average Volts:\t %.4lf V\n",
	  summary_totals[1] / sample);
  fprintf(meter->logfile, "Average Watts:\t %.4lf W\n",
	  summary_totals[2] / sample);
  fprintf(meter->logfile, "Total Energy:\t %.4lf J\n",
	  summary_totals[2] * meter->interval * sample);

  fprintf(stdout, "Logging complete.\n");
  
}
