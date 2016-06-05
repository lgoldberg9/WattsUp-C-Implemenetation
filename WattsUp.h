#include "Utilities.h"

#ifndef __WATTSUP_H_
#define __WATTSUP_H_

WattsUp* initialize_wattsup(flags *options);

void clean_wattsup(WattsUp *meter);

void mode(WattsUp *meter, char runmode);

void fetch(WattsUp *meter);

void logging(WattsUp *meter);

#endif
