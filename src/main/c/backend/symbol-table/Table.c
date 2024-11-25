#include "Table.h"
#include "khash.h"

typedef struct {
    EntryValue value;
    EntryType type;
} Entry;

static Logger * _logger = NULL;

KHASH_MAP_INIT_STR(myhash, Entry)  // Usa "myhash" como nombre del hash.

khash_t(myhash) * table;


void initializeTable(void){
   table = kh_init(myhash);
   _logger = createLogger("Table");
}

void destroyTable(void){
    kh_destroy(myhash, table);
    destroyLogger(_logger);
}

EntryType getType(char * identifier){
    logInformation(_logger, "Looking up type with the identifier: %s...", identifier);

    // Buscar el identificador en el hash.
    khiter_t k = kh_get(myhash, table, identifier);
    if (k == kh_end(table)) {
        logWarning(_logger, "Identifier %s not found", identifier);
        return EMPTY_TYPE;  // Si no se encuentra, devolver EMPTY_TYPE.
    }

    // Si se encuentra, obtener la entrada y devolver el tipo.
    Entry entry = kh_value(table, k);
    logInformation(_logger, "Identifier %s found with type: %d", identifier, entry.type);
    return entry.type;
}

static EntryResult getEntry(char * identifier, EntryType type){
    logInformation(_logger, "Looking up value with the identifier: %s...", identifier);
    EntryResult result = { .found=false};  // Inicializar como no encontrado.

    // Buscar el identificador en el hash.
    khiter_t k = kh_get(myhash, table, identifier);
    if (k == kh_end(table)) {
        logWarning(_logger, "Identifier %s not found", identifier);
        return result;
    }

    // Si se encuentra, verificar si el tipo coincide.
    Entry entry = kh_value(table, k);
    if (type == entry.type) {
        result.found = true;
        result.value = entry.value;  // Asignar el valor.
        logInformation(_logger, "Found value for identifier %s of type %d", identifier, type);
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

boolean exists(char * identifier) {
    khiter_t k = kh_get(myhash, table, identifier);
    if (k == kh_end(table)) {
        logWarning(_logger, "Identifier %s does not exist", identifier);
        return false;
    }
    return true;  // Si el identificador existe, devolver true.
}

static boolean insert(char * identifier, EntryType type, EntryValue value) {
    int ret;
    logWarning(_logger, "Placing the identifier: %s...", identifier);

    // Intentar insertar el identificador en el hash.
    khiter_t k = kh_put(myhash, table, identifier, &ret);
    if (ret <= 0) {
        logError(_logger, "Failed to insert identifier: %s", identifier);
        return false;  // Si no se pudo insertar, devolver false.
    }

    // Crear la entrada y asignarla al identificador.
    Entry entry = { .type = type, .value = value };
    kh_value(table, k) = entry;

    logInformation(_logger, "Identifier %s inserted successfully", identifier);
    return true;
}

boolean insertInteger(char * identifier, _INTEGER * value) {
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
