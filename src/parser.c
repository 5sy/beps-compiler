#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "parser.h"
#include "instructions.h"

static uint64_t m_LabelNames[PARSER_MAX_LABELS];
static uint32_t m_LabelOffsets[PARSER_MAX_LABELS];
static uint32_t instructions = 0;

// djb2 http://www.cse.yorku.ca/~oz/hash.html
uint64_t hash_s(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
    	// Stop if char hits :
    	if (c == ':') {
    		break;
    	}
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

uint32_t make_instruction(char *line) {
	uint32_t value = 0;
	char *ins = strtok(line, "\t ");
	uint64_t hash_value = hash_s((unsigned char*)ins);
	int success = 1;

	switch (hash_value) {
		// ADDI
		case 0x17C81DE77:
			value = instruction_ADDI(ins);
			break;
		// SYSCALL
		case 0xD0ACCE803DC0:
			value = instruction_SYSCALL(ins);
			break;
		// Ignored
		case 0x2B5E0:	// Comment (;)
		case 0x2B5AF:	// newline
			success = 0;
			break;
		default:
			fprintf(stderr, "ERROR: Unknown instruction '%s' (hash 0x%llX)\n", ins, hash_value);
			break;
	}
	// +1 if success, else dont incr
	instructions += (success == 1);
	return value;
}

void parser_WriteHeader(FILE *file) {
	fwrite("BEPS", 1, 4, file);
	fwrite(&PARSER_VERSION, 1, 1, file);
}

int parser_Compile(char *filePath, char *outPath) {
	FILE *file = fopen(filePath, "r");
	FILE *outFile = NULL;
	char buffer[PARSER_LINE_BUFFER];
	char *ins_buffer = NULL;
	uint32_t labels = 0;
	uint32_t cur_ins = 0;
	instructions = 0;

	// File exist check
	if (file == NULL) {
		fprintf(stderr, "Invalid file.\n");
		return 1;
	}

	outFile = fopen(outPath, "wb");
	if (outFile == NULL) {
		fprintf(stderr, "Unable to create output file.\n");
		fclose(file);
		return 1;
	}
	parser_WriteHeader(outFile);

	// Read file line by line
	while (fgets(buffer, PARSER_LINE_BUFFER, file) != NULL) {
		// Read instruction
		ins_buffer = malloc(strlen(buffer) + 1);	// safe to use length of line
		sscanf(buffer, "%s ", ins_buffer);

		// Check if label and store offset
		if (ins_buffer[strlen(ins_buffer) - 1] == ':') {
			m_LabelNames[labels] = hash_s((unsigned char*)ins_buffer);
			m_LabelOffsets[labels] = instructions;
			labels += 1;
		} else {
			// Compile instruction
			cur_ins = make_instruction(buffer);
			// Make sure valid instruction
			if (cur_ins != 0) {
				fwrite(&cur_ins, 1, sizeof(uint32_t), outFile);
				printf("0x%X\n", cur_ins);
			}
		}

		// Free buffer
		free(ins_buffer);
	}

	// Close file
	fclose(file);
	fclose(outFile);
	return 0;
}
