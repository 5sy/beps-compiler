#ifndef BEPS_PARSER
#define BEPS_PARSER

#define PARSER_LINE_BUFFER	1024	// Buffer used for reading each line into
#define PARSER_MAX_LABELS	256		// Max amount of labels to store

static const char PARSER_VERSION = 0x1;

// Writes header to a file stream
void parser_WriteHeader(FILE *file);

// Compiles with given input path and output path
int parser_Compile(char *filePath, char *outPath);

#endif