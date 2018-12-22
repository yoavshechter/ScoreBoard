#include "FunctionalUnit.h"

FunctionalUnit*  createFunctionalUnit() {
	FunctionalUnit* fu = (FunctionalUnit*)malloc(sizeof(FunctionalUnit));
	if (!fu) {
		return 0;
	}
	return fu;
}

void freeFunctionalUnit(FunctionalUnit* fu) {
	if (!fu) {
		return;
	}
	free(fu);
}

FunctionalUnits*  createFunctionalUnits(int numOfUnits) {
	FunctionalUnits* fus = (FunctionalUnits*)malloc(sizeof(FunctionalUnits));
	if (!fus) {
		return 0;
	}
	for (int i = 0; i < numOfUnits; i++) {
		fus->fu[i] = createFunctionalUnit();
	}
	return fus;
}

void freeFunctionalUnits(FunctionalUnits* fus) {
	if (!fus) {
		return;
	}
	for (int i = 0; i < MAX_NUM_OF_FUNCTIONAL_UNITS; i++) {
		freeFunctionalUnit(fus->fu[i]);
	}
}