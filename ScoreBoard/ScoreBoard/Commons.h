#pragma once

#define MEMORY_LENGTH 4096
#define NUM_OF_REGISTERS 16
#define NUM_OF_CYCLES_TYPES 4
#define NUM_OF_FILES 6
#define NUM_OF_UNITS 6
#define MAX_NUM_OF_FUNCTIONAL_UNITS 5
#define MAX_LENGTH 10
#define MAX_LINE_LENGTH 20 // TODO - Need to decide correct paramter.
#define NUM_OF_INSTRUCTION_IN_QUEUE 16

#define No 0
#define Yes 1

typedef enum {
	CONFIG = 0,
	MEMIN,
	MEMOUT,
	REGOUT,
	TRACEINST,
	TRACEUNIT,
}file;

typedef enum {
	OP_LD = 0,
	OP_ST,
	OP_ADD,
	OP_SUB,
	OP_MULT,
	OP_DIV,
	OP_HALT,
}opcode;

static char* filesNames[NUM_OF_FILES] = { "cfg", "memin", "memout", "regout", "traceinst", "traceunit" };