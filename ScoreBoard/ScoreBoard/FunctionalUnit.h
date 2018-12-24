#pragma once
#include <stdlib.h>
#include "Commons.h"
#include "Config.h"

typedef enum {
	UNIT_ADD = 0,
	UNIT_SUB, 
	UNIT_MULT,
	UNIT_DIV,
	UNIT_LD,
	UNIT_ST,
}UnitType;

typedef struct unit {
	UnitType name;
	int unitNum;
	int busy;
	opcode op;
	double f[3];
	struct Unit* q[2];
	int r[2];
}Unit;

typedef struct units {
	Unit* units[MAX_NUM_OF_FUNCTIONAL_UNITS];
	int numOfUnits;
	int delay;
}Units;

typedef struct functionalUnit {
	Units* fu[NUM_OF_UNITS];
	char* unitName;
	int unitNum;
}FunctionalUnit;


static char* unitsTypeNames[6] = { "ADD", "SUB", "MULT", "DIV", "LD", "ST" };


/*Create unit object.*/
Unit*  createUnit();

/*Free unit object.*/
void freeUnit(Unit* src);

/*Create units object.*/
Units*  createUnits(int numOfUnits, int delay);

/*Free units object.*/
void freeUnits(Units* src);

/*Create functional unis object.*/
FunctionalUnit* createFunctionalUnit(Config* cfg);

/*Free functional units object.*/
void freeFunctionalUnit(FunctionalUnit* src);