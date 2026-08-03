#ifndef BLINKER_OBJECTS_H
#define BLINKER_OBJECTS_H

#include "blinker_types.h"

int  object_load(const char *path, ObjectFile *obj);
void object_dump(ObjectFile *obj);

#endif