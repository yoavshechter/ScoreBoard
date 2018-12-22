#include "Simulator.h"

int simulator(char** filesPaths) {
	FILE* filesFd[NUM_OF_FILES];
	/*Files verifictaion.*/
	if (!verifyFiles(filesFd, filesPaths)) {
		return 0;
	}
	/*Memory initialize.*/
	int memory[MEMORY_LENGTH] = { 0 };
	double regs[NUM_OF_REGISTERS] = { 0 };
	initializeRegs(regs);
	return 0;
}

void initializeRegs(double* regs) {
	for (int i = 0; i < NUM_OF_REGISTERS; i++) {
		regs[i] = decToSinglePrecision(i);
	}
}