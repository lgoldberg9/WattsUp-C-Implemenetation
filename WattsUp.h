#include "Utilities.h"

#ifndef __WATTSUP_H_
#define __WATTSUP_H_

WattsUp* initialize_wattsup(bool sim, char *port, int interval);

void clean_wattsup(WattsUp *meter);

void mode(WattsUp *meter, bool sim, char runmode);

void fetch(WattsUp *meter, bool sim);

void logging(WattsUp *meter, bool sim, bool raw, char *logfile);

#endif
