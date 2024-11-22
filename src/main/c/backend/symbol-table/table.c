#include "table.h"

#define _CHECK_INITIALIZATION_ if (!_isInitialized()) return ERROR_MAP

Map * symbolTable = NULL;
long capacity = 1024;

void initTable() {
  symbolTable = mapInit(capacity);
}

boolean _isInitialized() {
  return symbolTable != NULL;
}

boolean checkExistance(char * id) {
  _CHECK_INITIALIZATION_;
  Value value = mapGet(symbolTable, id);
  return value.type != 0;
}

int getType(char * id) {
  if (!checkExistance(id)) return ERROR_MAP;
  Value v = mapGet(symbolTable, id);
  return v.type;
}

boolean _addToTable(char * id, int type, char * init) {
  _CHECK_INITIALIZATION_;
  Value value = (Value) {type, init};
  return mapSet(symbolTable, id, value);
}

boolean updateToTable(char * id, int type, char * init) {
  if (!checkExistance(id)) return ERROR_MAP;
  return _addToTable(id, type, init);
}

boolean addToTable(char * id, int type, char * init) {
  if (checkExistance(id)) return ERROR_MAP;
  return _addToTable(id, type, init);
}

void freeTable() {
  mapFree(symbolTable);
}

void printTable() {
  _mapPrint(symbolTable);
}