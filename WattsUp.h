#ifndef __WATTSUP_H_
#define __WATTSUP_H_

#define MAX_STRING_LEN 256

//typedef enum {false, true} bool;

typedef struct {
  bool outfile_b;
  char *outfile_path;
} outfile_t;

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
  struct interval{
    bool active_interval;
    double interval;
  } interval;
  struct port {
    bool port;
    char *portDest;
  } port;
} flags;



#endif
