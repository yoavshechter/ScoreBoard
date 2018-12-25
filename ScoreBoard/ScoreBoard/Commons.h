#pragma once

#define MEMORY_LENGTH 4096
#define NUM_OF_REGISTERS 16
#define NUM_OF_CYCLES_TYPES 4
#define NUM_OF_FILES 6
#define NUM_OF_UNITS 6
#define MAX_NUM_OF_FUNCTIONAL_UNITS 5
#define MAX_LENGTH 10
#define MAX_LINE_LENGTH 100 // TODO - Need to decide correct paramter.
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

typedef enum {
	Integer,
	Add,
	Sub,
	Mult,
	Div,
}RegistersStatus;

typedef enum {
	UNIT_LD = 0,
	UNIT_ST,
	UNIT_ADD,
	UNIT_SUB,
	UNIT_MULT,
	UNIT_DIV,
}UnitType;

typedef enum {
	ISSUE,
	READ_OPERAND,
	EXECUTION,
	WRITE_RESILT,
}InstructionStatus;

static char* filesNames[NUM_OF_FILES] = { "cfg", "memin", "memout", "regout", "traceinst", "traceunit" };


static char* unitsTypeNames[NUM_OF_UNITS] = { "LD", "ST", "ADD", "SUB", "MULT", "DIV" };


static char* configUnitsTypes[2*NUM_OF_UNITS+1] = { "add_nr_units", "sub_nr_units", "mul_nr_units", "div_nr_units", "ld_nr_units", "st_nr_units",
"add_delay" , "sub_delay", "mul_delay", "div_delay", "ld_delay", "st_delay", "trace_unit" };