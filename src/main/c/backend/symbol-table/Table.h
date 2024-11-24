#ifndef TABLE_H
#define TABLE_H

typedef struct {
    int value;
} INTCLASS;

typedef struct {
    char value[512];
} STRCLASS;

typedef struct {
    char value[10];
} HEXCOLORCLASS;

typedef struct {
    bool value;
} BOOLCLASS;

typedef struct {
    int height;
    char species[50];
} TREECLASS;

typedef struct {
    TREECLASS trees[100];
    int numTrees;
} FORESTCLASS;

typedef struct {
    char name[100];
    FORESTCLASS forest;
    int population;
} WORLDCLASS;


// Define the symbol table entry structure
typedef struct SymbolTableEntry {
    char varName[100];  // Variable name (identifier)
    enum { INT, STR, HEXCOLOR, BOOL, TREE, FOREST, WORLD } varType; // Type of the variable
    union { 
        INTCLASS intValue;
        STRCLASS strValue;
        HEXCOLORCLASS hexColorValue;
        BOOLCLASS boolValue;
        TREECLASS treeValue;
        FORESTCLASS forestValue;
        WORLDCLASS worldValue;
    } value;  // The value of the variable
    struct SymbolTableEntry* next;  // Pointer to the next entry
} SymbolTableEntry;


// Define the symbol table structure
typedef struct {
    SymbolTableEntry* head;
} SymbolTable;

// Function Declarations

// Function to create a new empty symbol table
SymbolTable* createSymbolTable();

// Function to add a symbol to the symbol table
void addSymbol(SymbolTable* table, const char* varName, void* value, int varType);

// Function to find a symbol by its name
SymbolTableEntry* findSymbol(SymbolTable* table, const char* varName);

// Function to print the contents of the symbol table
void printSymbolTable(SymbolTable* table);

#endif  // TABLE_H
