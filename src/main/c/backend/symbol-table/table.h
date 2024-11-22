#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include "map.h"

void initTable();
boolean checkExistance(char * id);
boolean addToTable(char * id, int type, char * init);
boolean updateToTable(char * id, int type, char * init);
int getType(char * id);
void freeTable();
void printTable();

#endif