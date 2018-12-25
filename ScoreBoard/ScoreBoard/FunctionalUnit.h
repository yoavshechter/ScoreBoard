#pragma once
#include <stdlib.h>
#include "Commons.h"
#include "Config.h"
#include "Instructions.h"


typedef struct unit {
	Instruction* instruction;
	UnitType type;
	int unitNum;
	int busy;
	opcode op;

	int f_i;
	int f_j;
	int f_k;

	struct Unit* q_j;
	int q_j_type;
	int q_k_type;
	int q_j_index;
	int q_k_index;
	struct Unit* q_k;

	int r_j;
	int r_k;
	int isEmpty;
}Unit;

typedef struct units {
	Unit* units[MAX_NUM_OF_FUNCTIONAL_UNITS];
	UnitType type;
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