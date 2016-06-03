#include "ArrayList.h"
#include <stdio.h>

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#define MAX_STRING_LEN 256
#define EXTERNAL_MODE 'E'
#define INTERNAL_MODE 'I'
#define TCPIP_MODE 'T'
#define FULLHANDLING 2

typedef enum {false, true} bool;

typedef struct flags {
  bool verbose;
  bool debug;
  bool simulation_mode;
  bool internal_mode;
  bool fetch;
  bool graphics_mode;
  bool logging;
  bool raw;
  struct outfile {
    bool outfile;
    char *outfile_path;
  } outfile;
  struct interval {
    bool active_interval;
    double interval;
  } interval;
  struct port {
    bool port;
    char *portDest;
  } port;
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
