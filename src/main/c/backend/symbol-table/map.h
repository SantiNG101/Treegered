#ifndef MAP_H
#define MAP_H
#define ERROR_MAP -2

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

typedef enum {TRUE, FALSE} bool;

Map * mapInit(long capacity);
bool mapSet(Map * map, char * key, Value value);
bool mapDelete(Map * map, char * key);
Value mapGet(Map * map, char * key);
void mapFree(Map * map);
void _mapPrint(Map * map);

#endif