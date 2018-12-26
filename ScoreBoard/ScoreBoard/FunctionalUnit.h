#pragma once
#include <stdlib.h>
#include "Commons.h"
#include "Config.h"
#include "Instructions.h"


typedef struct unit {
	Instruction* instruction;
	int type;
	int unitNum;
	int isEmpty;

	int busy;
	int op;
	int f_i;
	int f_j;
	int f_k;
	struct Unit* q_j;
	struct Unit* q_k;
	int r_j;
	int r_k;
}Unit;

typedef struct units {
	Unit** units;
	int type;
	int numOfTotalUnits;
	int numOfActiveUnits;
	int delay;
	int canInsert;
}Units;

typedef struct functionalUnit {
	Units* fu[NUM_OF_UNITS];
	char* unitName;
	int unitNum;
}FunctionalUnit;


/*Create unit object.*/
Unit*  createUnit(UnitType type, int num);

/*Free unit object.*/
void freeUnit(Unit* src);

/*Create units object.*/
Units* createUnits(int numOfUnits, int delay, UnitType type);

/*Free units object.*/
void freeUnits(Units* src);

/*Create functional unis object.*/
FunctionalUnit* createFunctionalUnit(Config* cfg);

/*Free functional units object.*/
void freeFunctionalUnit(FunctionalUnit* src);