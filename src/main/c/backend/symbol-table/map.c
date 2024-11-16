#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

/* PRIVATE FUNCTIONS */
int _hashCode(char * key, long mod) {
  long h = 0;
    for (int i = 0; key[i]; i++) {
      h = 31 * h + key[i];
    }
    return h % mod;
}

bool _mapSet(Map * map, char * key, Value value, int index, int startingIndex) {
  if (index == startingIndex) return FALSE;
  if (map->map[index].key != NULL) 
    return _mapSet(map, key, value, index+1%map->capacity, startingIndex);
  map->map[index].key = key;
  map->map[index].value = value;
  map->size++;
  return TRUE;
}

void _mapPrint(Map * map) {
  printf("Map:\n");
  for(int i=0;i<map->capacity;i++) {
    printf("{Key: %s; Value: {%d,%s}}\n", map->map[i].key, map->map[i].value.type, map->map[i].value.initialization);
  }
}

bool _mapDelete(Map * map, char * key, int index, int startingIndex) {
  if (index == startingIndex || map->map[index].key == NULL) return FALSE;
  if (strcmp(map->map[index].key, key) != 0)
    return _mapDelete(map, key, index+1%map->capacity, startingIndex);
  map->map[index].key = NULL;
  map->size--;
  return TRUE;
}

Value _mapGet(Map * map, char * key, int index, int startingIndex) {
  if (index == startingIndex || map->map[index].key == NULL) return (Value) {0, NULL};
  if (strcmp(map->map[index].key, key) != 0)
    return _mapGet(map, key, index+1%map->capacity, startingIndex);
  return map->map[index].value;
}

/* PUBLIC FUNCTIONS */

Map * mapInit(long capacity) {
  Map * map = malloc(sizeof(Map));
  map->capacity = capacity;
  map->size = 0;
  Entry * entry = calloc(capacity, sizeof(entry));
  map->map = entry;

  return map;
}

void mapFree(Map * map) {
  free(map->map);
  free(map);
}

bool mapSet(Map * map, char * key, Value value) {
  if (key == NULL || *key == '\0' || map->size == map->capacity) return FALSE;
  int hash = _hashCode(key, map->capacity);
  if (map->map[hash].key != NULL) {
    return _mapSet(map, key, value, hash+1%map->capacity, hash);
  }
  map->map[hash].key = key;
  map->map[hash].value = value;
  map->size++;
  return TRUE;
}

bool mapDelete(Map * map, char * key) {
  if (key == NULL || *key == '\0' || map->size == 0) return FALSE;
  int hash = _hashCode(key, map->capacity);
  if (strcmp(map->map[hash].key, key) != 0)
    return _mapDelete(map, key, hash+1%map->capacity, hash);
  map->map[hash].key = NULL;
  map->size--;
  return TRUE;
}

Value mapGet(Map * map, char * key) {
  if (key == NULL || *key == '\0' || map->size == 0) return (Value) {0, NULL};
  int hash = _hashCode(key, map->capacity);
  if (map->map[hash].key == NULL) return (Value) {0, NULL};
  if (strcmp(map->map[hash].key, key) != 0)
    return _mapGet(map, key, hash+1%map->capacity, hash);
  return map->map[hash].value;
}