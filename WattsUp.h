#ifndef __WATTSUP_H_
#define __WATTSUP_H_

typedef enum { false, true } bool;

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
