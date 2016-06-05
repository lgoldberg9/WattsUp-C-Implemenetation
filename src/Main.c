#include "WattsUp.h"
#include "Utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h> /* Sys information lib */
#include <unistd.h>

/*

 */
void option_descr(void) {
  fprintf(stdout, "usage: wattsup [-h] [-d] [-g] [-l] [-n SAMPLE] ");
  fprintf(stdout, "[-o OUTFILE] [-s INTERVAL]\n\t\t [-p PORT]\n\n");
  fprintf(stdout, "Get data from Watts Up power meter.\n\n");
  fprintf(stdout, "optional arguments:\n");
  fprintf(stdout, "  -h,\t\t show this help message and exit\n");
  fprintf(stdout, "  -d,\t\t debugging output\n");
  fprintf(stdout, "  -g,\t\t Graphical output: plot the data in real time\n");
  fprintf(stdout, "  -l,\t\t log data in real time\n");
  fprintf(stdout, "  -n SAMPLE,\t provide sample size\n");
  fprintf(stdout, "  -o OUTFILE,\t Output file\n");
  fprintf(stdout, "  -s INTERVAL,\t Sample interval\n");
  fprintf(stdout, "  -p PORT,\t USB serial port\n");
}

/* 

 */
flags* initialize_options() {

  flags *options;

  if ((options = malloc(sizeof(flags))) == NULL) {
    fprintf(stderr, "Failed to allocate memory for flags.\n");
    exit(EXIT_FAILURE);
  }
  
  options->debug = false;
  options->graphics_mode = false;
  options->logging = false;
  
  if ((options->sample = (sample_t*) malloc(sizeof(sample_t))) == NULL) {
    fprintf(stderr, "Failed to allocate memory for options->sample.\n");
    exit(EXIT_FAILURE);
  }
  options->sample->sample_b = false;
  options->sample->sample_size = 1;

  if ((options->outfile = (outfile_t*) malloc(sizeof(outfile_t))) == NULL) {
    fprintf(stderr, "Failed to allocate memory for options->outfile.\n");
    exit(EXIT_FAILURE);
  }
  options->outfile->outfile_b = false;
  options->outfile->outfile_path = "log.out";

  options->interval_d = 1;

  if ((options->port = (port_t*) malloc(sizeof(port_t))) == NULL) {
    fprintf(stderr, "Failed to allocate memory for options->port.\n");
    exit(EXIT_FAILURE);
  }
  options->port->port_b = false;
  options->port->port_dest = "/dev/ttyUSB0";

  return options;
}

/*

 */
void clean_options(flags *options) {
  if (!options->port->port_b) {
    free(options->port->port_dest);
  }
  free(options->sample);
  free(options->outfile);
  free(options->port);
  free(options);
}

/*

 */
void parse_inputs(flags *options, int argc, char **argv) {

  for (int index = 1; index < argc; index++) {
    switch (argv[index][1]) {
    case 'd':
      options->debug = true;
      break;
    case 'g':
      options->graphics_mode = true;
      break;
    case 'l':
      options->logging = true;
      break;
    case 'n':
      options->sample->sample_b = true;
      if (index++ < argc && argv[index][0] != '-') {
	options->sample->sample_size = atoi(argv[index]);
      } else {
	fprintf(stderr, "Not enough inputs provided to -n.\n");
      }
      break;
    case 'o':
      options->outfile->outfile_b = true;
      if (index++ < argc && argv[index][0] != '-') {
	options->outfile->outfile_path = argv[index]; 
      } else {
	fprintf(stderr, "Not enough inputs provided to -o.\n");
      }
      break;
    case 's':
      if (index++ < argc && argv[index][0] != '-') {
	options->interval_d = atof(argv[index]);
      } else {
	fprintf(stderr, "Not enough inputs provided to -s.\n");
      }
      break;
    case 'p':
      options->port->port_b = true;
      if (index++ < argc && argv[index][0] != '-') {
	options->port->port_dest = argv[index];
      } else {
	fprintf(stderr, "Not enough inputs provided to -p.\n");
      }
      break;
    case 'h':
      option_descr();
      exit(EXIT_SUCCESS);
      break;
    default:
      fprintf(stdout, "Incorrect option provided!\n");
      exit(EXIT_FAILURE);
      break;
    }
  }
}

/*

 */
void create_port(flags *options) {

  struct utsname* buf;
  
  if ((buf = (struct utsname*) malloc(sizeof(struct utsname))) == NULL) {
    fprintf(stderr, "Failed to allocate memory for UNAME.\n");
    exit(EXIT_FAILURE);
  }
  
  uname(buf);
  char *system = (char*) buf->sysname;
  
  options->port->port_dest = (char*) malloc(sizeof(char) * MAX_STRING_LEN);
  if (strcmp(system, "Darwin") == 0) { /* Max OS X */
    strcpy(options->port->port_dest, "/dev/tty.usb-serial-A1000wT3");
  } else if (strcmp(system, "Linux") == 0) {
    strcpy(options->port->port_dest, "/dev/ttyUSB0");
  }
  free(buf);
}

int main(int argc, char **argv) {
  
  flags *options = initialize_options();
  parse_inputs(options, argc, argv);
  
  if (!options->port->port_b) {
    create_port(options);
  }

  if (access(options->port->port_dest, F_OK) == -1) {
    fprintf(stderr, "\nSerial port %s does not exist.\n", options->port->port_dest);
    fprintf(stderr, "Please make sure FDTI drivers are installed\n");
    fprintf(stderr, " (http://www.ftdichip.com/Drivers/VCP.htm)\n");
    fprintf(stderr, "Default ports are /dev/ttyUSB0 for Linux\n");
    fprintf(stderr, " and /dev/tty.usbserial-A1000wT3 for Mac OS X\n");
    return EXIT_FAILURE;
  }

  WattsUp *meter = initialize_wattsup(options);

  if (options->logging) {
    logging(meter);
  }
  
  clean_wattsup(meter);
  clean_options(options);
  
  return EXIT_SUCCESS;
  
}
