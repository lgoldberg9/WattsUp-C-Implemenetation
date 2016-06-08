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
#include <sys/time.h>

//function used for stopwatch functionality
static size_t time_ms() {
  struct timeval tv;
  if(gettimeofday(&tv, NULL) == -1) {
    perror("gettimeofday");
    exit(2);
  }
  // Convert timeval values to milliseconds
  return tv.tv_sec*1000 + tv.tv_usec/1000;
}

/*
  Initializes the WattsUp meter variable and all appropriate variables
  so as to be able to collect data correctly
*/
WattsUp* initialize_wattsup(flags *options) {

  WattsUp *meter = (WattsUp*) malloc(sizeof(WattsUp));
  if (meter == NULL){
    perror("Could not allocate memory for new WattsUp");
  }

  int file_descr = 0;

  //Opens serial port
  if ((file_descr = open(options->port->port_dest, O_RDWR | O_NOCTTY | O_NDELAY))
      != -1) {
    meter->serial_descr = file_descr;
    //Reset BAUD rate
    struct termios serial_options;
    tcgetattr(meter->serial_descr, &serial_options);
    cfsetispeed(&serial_options, B115200);
    cfsetospeed(&serial_options, B115200);
    tcsetattr(file_descr, TCSANOW, &serial_options);
  } else {
    perror("initialize_wattsup - Unable to open port");
    exit(EXIT_FAILURE);
  }

  //Opens log file
  FILE *logfile_tmp;
  if (options->outfile->outfile_b) { 
    if ((logfile_tmp = fopen(options->outfile->outfile_path, "w")) == NULL) {
      perror("initialize_wattsup - Unable to open file");
      exit(EXIT_FAILURE);
    } 
  } else {
    logfile_tmp = stdout;
  }

  //Sets appropriate varibales
  meter->logfile = logfile_tmp;
  meter->sample_size = options->sample->sample_size;
  meter->interval = options->interval_d;
  meter->time = (double*) malloc(sizeof(double) * meter->sample_size);
  meter->power = (double*) malloc(sizeof(double) * meter->sample_size);
  meter->voltage = (double*) malloc(sizeof(double) * meter->sample_size);
  meter->current = (double*) malloc(sizeof(double) * meter->sample_size);
  meter->energy = (double*) malloc(sizeof(double) * meter->sample_size);
  
  return meter;
}//initialize_wattsup


//Frees all allocated memory
void clean_wattsup(WattsUp *meter) {
  if (meter != NULL) {
    close(meter->serial_descr);
    fclose(meter->logfile);
    free(meter->time);
    free(meter->power);
    free(meter->voltage);
    free(meter->current);
    free(meter->energy);
    free(meter);
  }
}//clean_wattsup

/*
  Function that logs all incomign data and processes it.
*/
void logging(WattsUp *meter) {
  fprintf(stdout, "Logging...\n");  
  
  char buf[MAX_STRING_LEN];
  char line[MAX_STRING_LEN];
  char *word;
  int sample = meter->sample_size;
  size_t time_start, time_end, run_time;

  memset(&buf[0], 0, sizeof(buf));

  //Necessary to get WattsUp meter to send data back to the computer
  write(meter->serial_descr, "#R,W,0;", strlen("#R,W,0;"));
  
  //Sampling
  for (int time = 0; time < sample; time++) {
    time_start = time_ms();
    //Read incoming data packet
    read(meter->serial_descr, buf, sizeof(buf));
    for (int index = 0; index < MAX_STRING_LEN; index++) {
      line[index] = buf[index];
    }

    //Delimit by commas (',')
    word = strtok(line, ",");
    if (word != NULL) {
      meter->time[time] = time * meter->interval;

      //Parse Data
      for (int word_parse = 0; word_parse < 6; word_parse++) {
	switch (word_parse) {
	case 3: //Current in Amps
	  meter->power[time] = atof(word) / 10.0;
	  break;
	case 4: //Volts
	  meter->voltage[time] = atof(word) / 10.0;
	  break;
	case 5: //Power
	  meter->current[time] = atof(word) / 1000.0;
	  break;
	case 6:
	  meter->energy[time] = atof(word) / 10.0;
	default:
	  break;
	}
	word = strtok(NULL, ",");
      }
    } 
    else { 
      time--;
    }
    //Sleep for given interval - maybe need to make more accurate?
    time_end = time_ms();
    run_time = time_end - time_start;
    usleep((1000000 * meter->interval)- run_time*1000);
  }//for-loop

  double summary_totals[4] = {0, 0, 0, 0};

  //printing to the output file (stdout default)
  fprintf(meter->logfile, "Time\t Amps\t Volts\t Watts\n");
  for (int index = 0; index < sample; index++) {
    fprintf(meter->logfile, "%.3lf\t%.3lf\t%.2lf\t%.3lf\t%.3lf\n",
	    meter->time[index], meter->current[index], meter->voltage[index],
	    meter->power[index], meter->energy[index]);
    summary_totals[0] += meter->current[index];
    summary_totals[1] += meter->voltage[index];
    summary_totals[2] += meter->power[index];
    summary_totals[3] += meter->energy[index];
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
	  summary_totals[3]);

  fprintf(stdout, "Logging complete.\n");

}//logging

//WattsUp.c
