#define _GNU_SOURCE
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include "FunctionalUnit.h"

Unit* createUnit(UnitType type, int num) {
	Unit* src = (Unit*)malloc(sizeof(Unit));
	if (!src) {
		printf("Error! Failed to allocate memory for unit struct.\n");
		return 0;
	}
	src->type = type;
	src->unitNum = num;
	src->isEmpty = Yes;

	src->busy = No;
	src->op = -1;
	src->f_i = -1;
	src->f_j = -1;
	src->f_k = -1;
	src->r_j = -1;
	src->r_k = -1;
	return src;
}

void freeUnit(Unit* src) {
	if (!src) {
		return;
	}
	free(src);
}

Units* createUnits(int numOfUnits, int delay, UnitType type) {
	Units* src = (Units*)malloc(sizeof(Units));
	if (!src) {
		printf("Error! Failed to allocate memory for units struct.\n");
		return 0;
	}

	Unit** unitsArray = (Unit**)malloc(numOfUnits * sizeof(Unit*));
	if (!unitsArray) {
		printf("Error! Failed to allocate memory for unitsArray.\n");
		return 0;
	}

	src->units = unitsArray;

	for (int i = 0; i < numOfUnits; i++) {
		Unit* unit = createUnit(type, i);
		src->units[i] = unit;
		if (!src->units[i]) {
			printf("Error! Failed to allocate memory for units struct.\n");
			for (int j = i; j > -1; j--) {
				freeUnit(src->units[j]);
			}
			return 0;
		}
	}
	src->numOfTotalUnits = numOfUnits;
	src->numOfActiveUnits = 0;
	src->delay = delay;
	src->type = type;
	src->canInsert = Yes;
	return src;
}

void freeUnits(Units* src) {
	if (!src) {
		return;
	}
	for (int i = 0; i < MAX_NUM_OF_FUNCTIONAL_UNITS; i++) {
		freeUnit(src->units[i]);
	}
	free(src);
}

FunctionalUnit* createFunctionalUnit(Config* cfg) {
	FunctionalUnit* src = (FunctionalUnit*)malloc(sizeof(FunctionalUnit));
	if (!src) {
		printf("Error! Failed to allocate memory for functional units struct.\n");
		return 0;
	}
	char* name = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
	if (!name) {
		printf("Error! Failed to allocate memory for functional units struct.\n");
		free(src);
		return 0;
	}
	src->unitName = name;
	for (int i = 0; i < NUM_OF_UNITS; i++) {
		src->fu[i] = createUnits(cfg->units[i], cfg->delays[i], i);
		if (!src->fu[i]) {
			printf("Error! Failed to allocate memory for functional units in functional units struct.\n");
			for (int j = i; j > -1; j--) {
				freeUnits(src->fu[j]);
			}
			return 0;
		}
	}
	strcpy(src->unitName, cfg->unitName);
	src->unitNum = cfg->unitNum;
	return src;
}

void freeFunctionalUnit(FunctionalUnit* fus) {
	if (!fus) {
		return;
	}
	for (int i = 0; i < NUM_OF_UNITS; i++) {
		freeUnits(fus->fu[i]);
	}
	free(fus->unitName);
	free(fus);
}