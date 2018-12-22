#pragma once
#include <stdlib.h>
#include "Commons.h"

typedef struct functionalUnit {
	int busy;
	opcode op;
	int f[3];
	struct FunctionalUnit* q[2];
	int r[2];
}FunctionalUnit;

typedef struct functionalUnits {
	FunctionalUnit** fu;
}FunctionalUnits;


FunctionalUnit*  createFunctionalUnit();

void freeFunctionalUnit(FunctionalUnit* fu);

FunctionalUnits*  createFunctionalUnits(int numOfUnits);

void freeFunctionalUnits(FunctionalUnits* fus);