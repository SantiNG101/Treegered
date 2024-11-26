#include "Generator.h"

/* MODULE INTERNAL STATE */
const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;
FILE * file = NULL;

/* MODULE INTERNAL STATE */

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
    file = fopen("treegered.html", "w");
	if (file == NULL) {
        printf("Error: Could not open treegered.html for writing.\n");
        return;
    }
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
    fclose(file);
}

/** PRIVATE FUNCTIONS */
static void _generatePrologue(void);
static void _generateEpilogue();
static void _generateProgram(Program * program);
static void _generateWorldPrologue(const int worldHeight, const int worldWidth, const char * worldMessage);
static void _generateTreeDraw(int x, boolean snowed, int density, int height, int depth, char* color, int bark);

void _generateGrowTrees(_TREENODE * trees);
void _generateGrowForests(_FORESTNODE * forests);

static char * _indentation(const unsigned int indentationLevel);
static void _output(FILE * file, const unsigned int indentationLevel, const char * const format, ...);

static void _generatePrologue(void) {
	_output(file, 0, "%s",
		"<!DOCTYPE html>\n<html>\n<head>\n<title>Tree Visualization</title>\n"
		"<style>\n"
		"body {\n  margin: 0;\n  overflow: hidden;\n}\n"
		".tree {\n  position: absolute;\n  display: flex;\n  flex-direction: column;\n  align-items: center;\n}\n"
		".trunk {\n background-color: brown; border: 2px solid #1B0000;\n}\n"
		".leaves {\n  border-radius: 50%;\n  text-align: center;\n  color: white;\n}\n"
		"</style>\n"
		"<script>\n"
		"window.onload = function() {\n"
		"  document.body.style.height = window.innerHeight + 'px';\n"
		"  document.body.style.width = window.innerWidth + 'px';\n"
	);
}

static void _generateWorldPrologue(const int worldHeight, const int worldWidth, const char * worldMessage){
	_output(file, 0, "  const worldHeight = %d;\n"
	, worldHeight
	);
	_output(file, 0, "  const worldWidth = %d;\n"
	, worldWidth
	);
	_output(file, 0, "%s",
		"  const scaleHeight = window.innerHeight / worldHeight;\n"
		"  const scaleWidth = window.innerWidth / worldWidth;\n"
		"  document.querySelectorAll('.tree').forEach(tree => {\n"
		"    tree.style.left = (parseFloat(tree.style.left) * scaleWidth) + 'px';\n"
		"    tree.style.bottom = (parseFloat(tree.style.bottom) * scaleHeight) + 'px';\n"
		"  });\n"
		"  document.querySelectorAll('.leaves, .trunk').forEach(part => {\n"
		"    part.style.width = (parseFloat(part.style.width) * scaleWidth) + 'px';\n"
		"    part.style.height = (parseFloat(part.style.height) * scaleHeight) + 'px';\n"
		"  });\n"
		"};\n"
		"</script>\n"
		"</head>\n<body style='background-color: black'>\n"
	);
	_output(file, 0, "<h1 style='position:absolute; top:10px; left:10px; color:white; background-color:black; z-index:99999' >%s</h1>\n"
	, worldMessage
	);
}

static void _generateTreeDraw(int x, boolean snowed, int density, int height, int depth, char* color, int bark){
	_output(file, 0, "<div class='tree' style='left: %d%; bottom: 0;'>\n"
	, x
	);
	if(snowed==TRUE){
		_output(file, 0, "<div class='leaves' style='width: %dpx; height: %dpx; background-color: white; z-index:%d; position:absolute; top: 5px; left:-5px'></div>\n"
		, (density+1), (height/2), ((depth*2)-1)
		);
	}
	_output(file, 0, "<div class='leaves' style='width: %dpx; height: %dpx; background-color: %s; z-index:%d; position:relative; top:10px'></div>\n"
	, (density), (height/2), (color), (depth*2)
	);
	_output(file, 0, "<div class='trunk' style='height: %dpx; width: %dpx; z-index:%d;'></div>\n"
	, (height/2), (bark), ((depth*2)-1)
	);
	_output(file, 0, "</div>\n");
}

static void _generateEpilogue() {
	_output(file, 0, "%s", "</body>\n</html>");
}

void _generateGrowTrees(_TREENODE * trees){
	if(trees == NULL) return;
	_generateTreeDraw(trees->x, trees->snowed, trees->density, trees->height, trees->depth, trees->color, trees->bark);
	return _generateGrowTrees(trees->next);
}

void _generateGrowForests(_FORESTNODE * forests){
	if(forests == NULL) return;
	_generateGrowTrees(forests->trees);
	return _generateGrowForests(forests->next);
}

/**
 * Generates the output of the program.
 */
static void _generateProgram(Program * program) {
    _WORLD * world = getWorld("world").value._world;
	_generateWorldPrologue(world->height, world->width, world->message);
    _GROWNODE * grow = getGrow("grow").value._grownode;
    if(grow->trees != NULL && grow->forests == NULL){
		_generateGrowTrees(grow->trees);
	}
    else if(grow->trees == NULL && grow->forests != NULL){
		_generateGrowForests(grow->forests);
	}
    else{
		_generateGrowTrees(grow->trees);
		_generateGrowForests(grow->forests);
	}
}

/**
 * Generates an indentation string for the specified level.
 */
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
static void _output(FILE * file, const unsigned int indentationLevel, const char * const format, ...) {
	if (file == NULL) {
        perror("Error opening file");
        return;
	}
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(file, effectiveFormat, arguments);
	fflush(stdout);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	_generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue();
	logDebugging(_logger, "Generation is done.");
}