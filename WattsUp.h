#ifndef __WATTSUP_H_
#define __WATTSUP_H_

#define MAX_STRING_LEN 256
#define EXTERNAL_MODE 'E'
#define INTERNAL_MODE 'I'
#define TCPIP_MODE 'T'
#define FULLHANDLING 2

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
  FILE* serial_file;
  FILE* logfile;
  int interval;
  array_list *time;
  array_list *power;
  array_list *voltage;
  array_list *current;  
} WattsUp;

WattsUp* initialize_wattsup(bool sim, char *port, int interval);

void clean_wattsup(WattsUp *meter);

void mode(WattsUp *meter, bool sim, char runmode);

void fetch(WattsUp *meter, bool sim);

void logging(WattsUp *meter, bool sim, char *logfile);

#endif
