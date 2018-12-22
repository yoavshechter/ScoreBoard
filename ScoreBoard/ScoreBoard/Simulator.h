#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "Utils.h"

enum functionalUnitNames {
	INTEGER = 0,
	MULT1,
	MULT2,
	Add,
	DIVIDE,
};

typedef struct functionalUnit {
	int busy;
	opcode op;
	int f[3];
	int r[2];
}FunctionalUnit;

typedef struct functionalUnits {
	FunctionalUnit* fu[NUM_OF_FUNCTIONAL_UNITS];
}FunctionalUnits;


FunctionalUnit*  createFunctionalUnit();

void freeFunctionalUnit(FunctionalUnit* fu);

FunctionalUnits*  createFunctionalUnits();

void freeFunctionalUnits(FunctionalUnits* fus);

int simulator(char** filesPaths);

int initializeMemory(FILE* meminFd, char* line, int* memory);

void initializeRegs(double* regs);

void freeSolution(FILE** fds, char* line, Config* cfg, Command* cmd);