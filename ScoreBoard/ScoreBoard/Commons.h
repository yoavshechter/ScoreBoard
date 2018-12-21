#pragma once
#define MAX_REGISTERS = 16
#define NUM_OF_CYCLES_TYPES = 4

typedef struct opcodeCommand {
	char* cmdName;
}Command;

typedef struct fetchUnit {
	char* unitName;
}Unit;