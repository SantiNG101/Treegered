#include "Table.h"
#include "khash.h"


typedef struct {
	EntryValue value;
    EntryType type;
} Entry;

static Logger * _logger = NULL;

KHASH_MAP_INIT_STR(hash, Entry)

khash_t(hash) * table;


void initializeTable(void){
   table = kh_init(hash);
   _logger = createLogger("Table");
}

void destroyTable(void){
    kh_destroy(hash, table);
    destroyLogger(_logger);
}

EntryType getType(char * identifier){
    logInformation(_logger, "Looking up type with the identifier: %s...", identifier);
    khiter_t k = kh_get(hash, table, identifier);
    if( k == kh_end(table) ){
        return EMPTY_TYPE;
    }
    Entry entry = kh_value(table, k);
    return entry.type;
}

static EntryResult getEntry(char * identifier, EntryType type){
    logInformation(_logger, "Looking up value with the identifier: %s...", identifier);
    EntryResult result = { .found=false} ;
    khiter_t k = kh_get(hash, table, identifier);
    if ( k==kh_end(table) )
        return result;
    Entry entry = kh_value(table,k);
    if ( type == entry.type ) {
        result.found=true, 
        result.value=entry.value;
    }
    return result;
}

EntryResult getInteger(char * identifier){
    return getEntry(identifier, INTEGER_TYPE);
}

EntryResult getBoolean(char * identifier){
    return getEntry(identifier, BOOL_TYPE);
}

EntryResult getString(char * identifier){
    return getEntry(identifier, STRING_TYPE);
}

EntryResult getHexcolor(char * identifier){
    return getEntry(identifier, HEXCOLOR_TYPE);
}

EntryResult getTree(char * identifier){
    return getEntry(identifier, TREE_TYPE);
}

EntryResult getForest(char * identifier){
    return getEntry(identifier, FOREST_TYPE);
}

EntryResult getWorld(char * identifier){
    return getEntry(identifier, WORLD_TYPE);
}

boolean exists(char * identifier ) {
    khiter_t k = kh_get(myhash, table, identifier);
    if ( k==kh_end(table) )
        return false;
    return true; 
}


static boolean insert(char * identifier,  EntryType type, EntryValue value ){
    int ret;
    logWarning(_logger, "Placing the identifier: %s...", identifier);

    khiter_t k = kh_put(myhash, table, identifier, &ret);
    if ( ret <= 0)
        return false;
    logCritical(_logger, "Load key");
    Entry entry = {
        .type = type,
        .value = value
    };

    kh_value(table,k) = entry;
    return true;
}

boolean insertInteger(char * identifier, _INTEGER * value ){
    EntryValue entryValue;
    entryValue._integer = value;
    return insert(identifier, INTEGER_TYPE, entryValue);
}


boolean insertBoolean(char * identifier, _BOOLEAN * value){
    EntryValue entryValue;
    entryValue._boolean = value;
    return insert(identifier, BOOL_TYPE, entryValue);
}

boolean insertString(char * identifier, _STRING * value){
    EntryValue entryValue;
    entryValue._string = value;
    return insert(identifier, STRING_TYPE, entryValue);
}

boolean insertHexcolor(char * identifier, _HEXCOLOR * value){
    EntryValue entryValue;
    entryValue._hexcolor = value;
    return insert(identifier, HEXCOLOR_TYPE, entryValue);
}

boolean insertTree(char * identifier, _TREE * value){
    EntryValue entryValue;
    entryValue._tree = value;
    return insert(identifier, TREE_TYPE, entryValue);
}

boolean insertForest(char * identifier, _FOREST * value){
    EntryValue entryValue;
    entryValue._forest = value;
    return insert(identifier, FOREST_TYPE, entryValue);
}

boolean insertWorld(char * identifier, _WORLD * value){
    EntryValue entryValue;
    entryValue._world = value;
    return insert(identifier, WORLD_TYPE, entryValue);
}