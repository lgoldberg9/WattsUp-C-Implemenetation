#include "ArrayList.h"
#include <stdio.h>

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#define MAX_STRING_LEN 256
#define EXTERNAL_MODE 'E'
#define INTERNAL_MODE 'I'
#define TCPIP_MODE 'T'
#define FULLHANDLING 2

//typedef enum {false, true} bool;
typedef struct outfile_def {
  bool outfile_b;
  char *outfile_path;
} outfile_t;


typedef struct port_def {
    bool port_b;
    char *portDest;
  } port_t;


typedef struct interval_def {
    bool active_interval;
    double interval_d;
  } interval_t;

typedef struct flags {
  bool verbose;
  bool debug;
  bool simulation_mode;
  bool internal_mode;
  bool fetch;
  bool graphics_mode;
  bool logging;
  bool raw;
  outfile_t outfile;
  interval_t interval;
  port_t port;
} flags;

typedef struct WattsUp {
  int serial_descr;
  FILE* logfile;
  int interval;
  array_list *time;
  array_list *power;
  array_list *voltage;
  array_list *current;  
} WattsUp;

#endif
