#include <stdio.h>
#include "parser.h"

int main(int argc, char *argv[]) {
	// Check if have arg
	if (argc < 2) {
		fprintf(stderr, "%s <file>\n", argv[0]);
		return 1;
	}

	// Compile from read data
	return parser_Compile(argv[1], "out.bepexe");
}