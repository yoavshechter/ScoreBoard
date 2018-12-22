#define _CRT_SECURE_NO_WARNINGS
#include "Simulator.h"


int simulator(char** filesPaths) {
	FILE* filesFd[NUM_OF_FILES];
	Config* cfg;
	int memory[MEMORY_LENGTH] = { 0 };
	double regs[NUM_OF_REGISTERS] = { 0 };
	int numOfLines = 0;

	char* line = calloc(1, sizeof(char) * MAX_LINE_LENGTH);
	if (!line) {
		printf("Error! couldnt allocated memory.\n");
		return 0;
	}
	/*Files verifictaion.*/
	if (!verifyFiles(filesFd, filesPaths)) {
		return 0;
	}
	/*Memory initialize.*/
	if ((numOfLines = initializeMemory(filesFd[MEMIN], line, memory)) == 0) {
		printf("Error! memin.txt file is empty.\n");
		freeSolution(filesFd, line, 0, 0);
		return 0;
	}
	/*Registers initialize.*/
	initializeRegs(regs);
	if ((cfg = parseConfig(filesFd[CONFIG], line)) == 0) {
		printf("Error! failed to create config obg from cfg.txt.\n");
		freeSolution(filesFd, line, cfg, 0);
	}
	return 0;
}

int initializeMemory(FILE* meminFd, char* line, int* memory) {
	int numOfLines = 0;
	int maxLine = 0;
	char currentLine[MAX_LINE_LENGTH];
	while (fgets(line, MAX_LINE_LENGTH, meminFd) != 0) {
		sscanf(line, "%s", currentLine);
		if (strcmp(currentLine, "0x00000000") != 0) {
			maxLine = numOfLines;
		}
		sscanf(currentLine, "%x", &memory[numOfLines]);
		numOfLines++;
	}
	return maxLine;
}

void initializeRegs(double* regs) {
	for (int i = 0; i < NUM_OF_REGISTERS; i++) {
		regs[i] = decToSinglePrecision(i);
	}
}

void freeSolution(FILE** fds, char* line, Config* cfg, Command* cmd) {
	for (int i = 0; i < NUM_OF_FILES; i++) {
		fclose(fds[i]);
	}
	if (line) { free(line); }
	freeCmd(cmd);
	freeConfig(cfg);
}

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

FunctionalUnits*  createFunctionalUnits() {
	FunctionalUnits* fus = (FunctionalUnits*)malloc(sizeof(FunctionalUnits));
	if (!fus) {
		return 0;
	}
	for (int i = 0; i < NUM_OF_FUNCTIONAL_UNITS; i++) {
		fus->fu[i] = createFunctionalUnit();
	}
	return fus;
}

void freeFunctionalUnits(FunctionalUnits* fus) {
	if (!fus) {
		return;
	}
	for (int i = 0; i < NUM_OF_FUNCTIONAL_UNITS; i++) {
		freeFunctionalUnit(fus->fu[i]);
	}
}
