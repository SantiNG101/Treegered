#include "map.h"

/* PRIVATE FUNCTIONS */
int _hashCode(char * key, long mod) {
  long h = 0;
    for (int i = 0; key[i]; i++) {
      h = 31 * h + key[i];
    }
    return h % mod;
}

boolean _mapSet(Map * map, char * key, Value value, int index, int startingIndex) {
  if (index == startingIndex) return false;
  if (map->map[index].key != NULL) 
    return _mapSet(map, key, value, index+1%map->capacity, startingIndex);
  map->map[index].key = key;
  map->map[index].value = value;
  map->size++;
  return true;
}

boolean _mapDelete(Map * map, char * key, int index, int startingIndex) {
  if (index == startingIndex || map->map[index].key == NULL) return false;
  if (strcmp(map->map[index].key, key) != 0)
    return _mapDelete(map, key, index+1%map->capacity, startingIndex);
  map->map[index].key = NULL;
  map->size--;
  return true;
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

boolean mapSet(Map * map, char * key, Value value) {
  if (key == NULL || *key == '\0' || map->size == map->capacity) return false;
  int hash = _hashCode(key, map->capacity);
  if (map->map[hash].key != NULL) {
    return _mapSet(map, key, value, hash+1%map->capacity, hash);
  }
  map->map[hash].key = key;
  map->map[hash].value = value;
  map->size++;
  return true;
}

boolean mapDelete(Map * map, char * key) {
  if (key == NULL || *key == '\0' || map->size == 0) return false;
  int hash = _hashCode(key, map->capacity);
  if (strcmp(map->map[hash].key, key) != 0)
    return _mapDelete(map, key, hash+1%map->capacity, hash);
  map->map[hash].key = NULL;
  map->size--;
  return true;
}

Value mapGet(Map * map, char * key) {
  if (key == NULL || *key == '\0' || map->size == 0) return (Value) {0, NULL};
  int hash = _hashCode(key, map->capacity);
  if (map->map[hash].key == NULL) return (Value) {0, NULL};
  if (strcmp(map->map[hash].key, key) != 0)
    return _mapGet(map, key, hash+1%map->capacity, hash);
  return map->map[hash].value;
}