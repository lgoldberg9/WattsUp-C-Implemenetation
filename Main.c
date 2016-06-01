#include "WattsUp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h> /* Text animation lib */
#include <sys/utsname.h> /* Sys information lib */

void initialize_options(flags *options) {
  options->verbose = false;
  options->debug = false;
  options->simulation_mode = false;
  options->internal_mode = false;
  options->fetch = false;
  options->graphics_mode = false;
  options->logging = false;
  options->raw = false;
  options->outfile.outfile = false;
  options->outfile.outfile_path = "log.out";
  options->interval.active_interval = false;
  options->interval.interval = 1;
  options->port.port = false;
}

void option_descr(void) {
  fprintf(stdout, "usage: wattsup [-h] [-v] [-d] [-m] [-i] [-f] [-g] [-l] [-r] ");
  fprintf(stdout, "[-o OUTFILE]\n [-s INTERVAL] [-p PORT]\n\n");
  fprintf(stdout, "Get data from Watts Up power meter.\n\n");
  fprintf(stdout, "optional arguments:\n");
  fprintf(stdout, "  -h,\t\t show this help message and exit\n");
  fprintf(stdout, "  -v,\t\t verbose\n");
  fprintf(stdout, "  -d,\t\t debugging output\n");
  fprintf(stdout, "  -m,\t\t simulate logging by reading serial data from disk ");
  fprintf(stdout, " with\n\t\t\t delay of sample interval between lines\n");
  fprintf(stdout, "  -i,\t\t Set meter to internal logging mode\n");
  fprintf(stdout, "  -f,\t\t Fetch data stored on the meter (NOT YET WORKING!)\n");
  fprintf(stdout, "  -g,\t\t Graphical output: plot the data in real time\n");
  fprintf(stdout, "  -l,\t\t log data in real time\n");
  fprintf(stdout, "  -r,\t\t output raw file\n");
  fprintf(stdout, "  -o OUTFILE,\t Output file\n");
  fprintf(stdout, "  -s INTERVAL,\t Sample interval\n");
  fprintf(stdout, "  -p PORT,\t USB serial port\n");
}

void parse_inputs(flags *options, int argc, char **argv) {

  for (int index = 1; index <= argc; index++) {
    switch (argv[index][1]) {
    case 'v':
      options->verbose = true;
      break;
    case 'd':
      options->debug = true;
      break;
    case 'm':
      options->simulation_mode = true;
      break;
    case 'i':
      options->internal_mode = true;
      break;
    case 'f':
      options->fetch = true;
      break;
    case 'g':
      options->graphics_mode = true;
      break;
    case 'l':
      options->logging = true;
      break;
    case 'r':
      options->raw = true;
      break;
    case 'o':
      options->outfile.outfile = true;
      options->outfile.outfile_path = (char*) malloc(sizeof(argv[++index]));
      options->outfile.outfile_path = argv[index];
      break;
    case 's':
      options->interval.active_interval = true;
      options->interval.interval = atoi(argv[++index]);
      break;
    case 'p':
      options->port.port = true;
      options->port.portDest = argv[++index];
      break;
    case 'h':
      option_descr();
      exit(EXIT_SUCCESS);
    default:
      fprintf(stdout, "Incorrect option provided!\n");
      exit(EXIT_FAILURE);
    }
  }
}

int main(int argc, char **argv) {

  flags *options;
  utsname *buf;
  
  if ((options = malloc(sizeof(flags))) != 0) {
    fprintf(stderr, "Failed to allocate memory for flags.\n");
    return EXIT_FAILURE;
  }

  initialize_options(options);
  parse_inputs(options, argc, argv);

  if (!options->port.port) {
    options->port.port = true;
    if ((utsname *buf = (utsname*) malloc(sizeof(utsname))) != 0) {
      fprintf(stderr, "Failed to allocate memory for UNAME.\n");
      return EXIT_FAILURE;
    }
    char *system = buf->sysname;
    if (strcmp(system, "Darwin") == 0) {
      strcpy(options->port.portDest, "/dev/tty.usb-serial-A1000wT3");
    } else if (strcmp(system, "Linux") == 0) {
      strcpy(options->port.portDest, "/dev/ttyUSB0");
    }
  }
  
}
