#ifndef MAP_H
#define MAP_H
#define ERROR_MAP -2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../shared/Type.h"

typedef struct Value{
  int type;
  char * initialization;
} Value;

typedef struct Entry{
  char * key;
  Value value;
} Entry;

typedef struct Map{
  struct Entry * map;
  long capacity;
  long size;
} Map;

Map * mapInit(long capacity);
boolean mapSet(Map * map, char * key, Value value);
boolean mapDelete(Map * map, char * key);
Value mapGet(Map * map, char * key);
void mapFree(Map * map);
void _mapPrint(Map * map);

#endif