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
	int canWriteResult;

	int busy;
	int op;
	int f_i;
	int f_j;
	double f_j_value;
	int f_k;
	double f_k_value;

	int q_j_type;
	int q_k_type;
	int q_j_index;
	int q_k_index;

	int r_j;
	int r_k;
}Unit;

typedef struct units {
	Unit* units[NUM_OF_UNITS];
	int type;
	int numOfTotalUnits;
	int numOfActiveUnits;
	int delay;
	int canInsert;
}Units;

typedef struct functionalUnit {
	Units* fu[NUM_OF_UNITS];
	int unitName;
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