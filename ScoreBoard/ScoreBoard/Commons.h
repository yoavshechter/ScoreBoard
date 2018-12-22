#pragma once
#define MAX_REGISTERS 16
#define NUM_OF_CYCLES_TYPES 4
#define NUM_OF_FILES 6

typedef enum {
	CONFIG = 0,
	MEMIN,
	MEMOUT,
	REGOUT,
	TRACEINST,
	TRACEUNIT,
}files;


static char* filesNames[NUM_OF_FILES] = { "cfg", "memin", "memout", "regout", "traceinst", "traceunit" };

typedef struct opcodeCommand {
	char* cmdName;
}Command;

typedef struct fetchUnit {
	char* unitName;
}Unit;