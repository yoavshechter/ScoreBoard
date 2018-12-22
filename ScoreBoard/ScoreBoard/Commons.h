#pragma once

#define MEMORY_LENGTH 4096
#define NUM_OF_REGISTERS 16
#define NUM_OF_CYCLES_TYPES 4
#define NUM_OF_FILES 6
#define NUM_OF_UNITS 6

typedef enum {
	CONFIG = 0,
	MEMIN,
	MEMOUT,
	REGOUT,
	TRACEINST,
	TRACEUNIT,
}file;

typedef enum {
	LD = 0,
	ST,
	ADD,
	SUB,
	MULT,
	DIV,
	HALT,
}opcode;

static char* filesNames[NUM_OF_FILES] = { "cfg", "memin", "memout", "regout", "traceinst", "traceunit" };

typedef struct fetchUnit {
	char* unitName;
}Unit;