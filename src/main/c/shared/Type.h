#ifndef TYPE_HEADER
#define TYPE_HEADER

typedef enum {
	false = 0,
	true = 1
} boolean;

typedef enum{ INTEGER_T, BOOLEAN_T, STRING_T, HEXCOLOR_T, TREE_T, FOREST_T, WORLD_T } SymbolTypes;

typedef int Token;
typedef char* Id;
typedef char* Hexcolor;
typedef int Class;

#endif
