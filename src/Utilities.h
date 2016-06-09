#include <stdio.h>

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#define MAX_STRING_LEN 1024
#define BAUDRATE B115200

/*const static char *LW = "#L,W,3,E,,1;";
  const static char *RW = "#R,W,0;";
  const static char *LR = "#L,R,0;"; 
*/

typedef enum {false, true} bool;

typedef struct outfile_def {
  bool outfile_b;
  char *outfile_path;
} outfile_t;

typedef struct port_def {
  bool port_b;
  char *port_dest;
} port_t;

typedef struct sample_def {
  bool sample_b;
  int sample_size;
} sample_t;

typedef struct flags {
  bool debug;
  bool graphics_mode;
  bool logging;
  sample_t *sample;
  outfile_t *outfile;
  double interval_d;
  port_t *port;
} flags;

typedef struct WattsUp {
  int serial_descr;
  FILE *logfile;
  int sample_size;
  double interval;
  double *time;
  double *power;
  double *voltage;
  double *current;
} WattsUp;

/* 

 */
void debug(const char *fmt, ...);

#endif
