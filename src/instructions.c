#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "instructions.h"

static const char *m_ValidRegisters[32] = {
	"$zero", "$at",
	"$v0", "$v1",
	"$a0", "$a1", "$a2", "$a3",
	"$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
	"$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
	"$t8", "$t9",
	"$k0", "$k1",
	"$gp", "$sp", "$fp", "$ra"
};

uint8_t resolve_register(char *reg) {
	uint8_t i;
	for (i = 0; i < 32; i++) {
		if (strncmp(reg, m_ValidRegisters[i], 5) == 0) {
			return i;
		}
	}
	fprintf(stderr, "ERROR: Unknown register %s\n", reg);
	return 0;
}

uint32_t instruction_ADDI(char *line) {
	char *tmp = NULL;
	uint8_t outputReg;
	uint8_t inputReg;
	uint16_t value = 0;
	uint32_t output = 0;

	tmp = strtok(NULL, "\t, ");
	if (tmp == NULL) {
		return 0;
	}
	outputReg = resolve_register(tmp);

	tmp = strtok(NULL, "\t, ");
	if (tmp == NULL) {
		return 0;
	}
	inputReg = resolve_register(tmp);

	tmp = strtok(NULL, "\t, ");
	if (tmp == NULL) {
		return 0;
	}
	sscanf(tmp, "%hu", &value);

	printf("ADDI %hhu, %hhu, %hu\n", outputReg, inputReg, value);

	// Assemble
	output |= 0x1;							// Opcode (6 bits)
	output |= ((outputReg & 0x1f) << 6);	// Output (5 bits)
	output |= ((inputReg & 0x1f) << 11);	// Input  (5 bits)
	output |= ((value & 0xffff) << 16);		// Value  (16 bits)
	return output;
}
