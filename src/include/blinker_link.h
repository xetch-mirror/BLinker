#ifndef BLINKER_LINK_H
#define BLINKER_LINK_H

#include "blinker_types.h"

int  link_objects(ObjectFile *objs, int obj_count, LinkedOutput *out);
void link_write(const char *path, LinkedOutput *out);

#endif