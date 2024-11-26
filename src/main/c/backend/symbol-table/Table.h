#ifndef TABLE_H
#define TABLE_H

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Type.h"

typedef enum {
    INTEGER_TYPE = INTEGER_T,
    BOOL_TYPE = BOOLEAN_T,
    STRING_TYPE = STRING_T,
    HEXCOLOR_TYPE = HEXCOLOR_T,
    TREE_TYPE = TREE_T,
    FOREST_TYPE = FOREST_T,
    WORLD_TYPE = WORLD_T,
    EMPTY_TYPE = EMPTY_T,
    TREE_NODE_TYPE = TREE_NODE_T,
    FOREST_NODE_TYPE = FOREST_NODE_T,
    GROW_NODE_TYPE = GROW_NODE_T
} EntryType;

typedef union {
    _INTEGER * _integer;
    _BOOLEAN * _boolean;
    _STRING * _string;
    _HEXCOLOR * _hexcolor;
    _TREE * _tree;
    _FOREST * _forest;
    _WORLD * _world;
    _TREENODE * _treenode;
    _FORESTNODE * _forestnode;
    _GROWNODE * _grownode;
} EntryValue;  

typedef struct{
    EntryValue value;
    EntryType type;
    boolean found;
} EntryResult;

void initializeTable(void);

void destroyTable(void);

EntryType getType(char * identifier);

EntryResult getInteger(char * identifier);

EntryResult getBoolean(char * identifier);

EntryResult getString(char * identifier);

EntryResult getHexcolor(char * identifier);

EntryResult getTree(char * identifier);

EntryResult getForest(char * identifier);

EntryResult getWorld(char * identifier);

EntryResult getTreeNode(char * identifier);

EntryResult getForestNode(char * identifier);

EntryResult getGrow(char * identifier);

boolean exists(char * identifier );

boolean insertInteger(char * identifier, _INTEGER * value );

boolean insertBoolean(char * identifier, _BOOLEAN * value);

boolean insertString(char * identifier, _STRING * value);

boolean insertHexcolor(char * identifier, _HEXCOLOR * value);

boolean insertTree(char * identifier, _TREE * value);

boolean insertForest(char * identifier, _FOREST * value);

boolean insertWorld(char * identifier, _WORLD * value);

boolean insertTreeNode(char * identifier, _TREENODE * value);

boolean insertForestNode(char * identifier, _FORESTNODE * value);

boolean insertGrow(char * identifier, _GROWNODE * value);

#endif  // TABLE_H
