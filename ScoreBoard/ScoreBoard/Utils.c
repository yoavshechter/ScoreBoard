#include "Utils.h"


int verifyFiles(FILE** filesFd, char ** filesPaths) {
	for (int i = 0; i < NUM_OF_FILES; i++) {
		FILE* fd;
		if (i > 1) {
			fd = fopen(filesPaths[i], "w");
		}
		else {
			fd = fopen(filesPaths[i], "r");
		}
		if (!fd) {
			printf("Error! coulsnt open %s/.txt file.\n", filesNames[i]);
			for (int j = 0; j < i; j++) {
				fclose(filesFd[j]);
			}
			return 0;
		}
		filesFd[i] = fd;
	}
	return 1;
}

double decToSinglePrecision(int dec) {
	//TODO
	return 1.0;
}

void printMemoutFile(FILE* fd, int* memory, int maxLines) {
	for (int i = 0; i < maxLines; i++) {
		fprintf(fd, "%d.%.8x\n", i, memory[i]);
	}
}

void printRegoutFile(FILE* fd, double* regs) {
	int i;
	for (i = 0; i < NUM_OF_REGISTERS; i++) {
		fprintf(fd, "%f", regs[i]);
	}
}

void printTracinstFile(FILE* fd, Instruction* instruction, int pc, Unit* unit, int* cycles) {
	int i = 0;
	fprintf(fd, "%.8x ", cmdToHex(instruction));
	fprintf(fd, "%d ", pc);
	fprintf(fd, "%s ", unitsTypeNames[unit->type]);
	for (i = 0; i < NUM_OF_CYCLES_TYPES - 1; i++) {
		fprintf(fd, "%d ", cycles[i]);
	}
	fprintf(fd, "%d\n", cycles[NUM_OF_CYCLES_TYPES - 1]);
}

void printTraceunitFile(FILE* fd, int cycle, Unit* unit, int* regsIndices, Unit** qUnits, int* r) {
	int i = 0;
	fprintf(fd, "%d ", cycle);
	fprintf(fd, "%s ", unitsTypeNames[unit->type]);
	for (i = 0; i < 3; i++) {
		fprintf(fd, "F%d ", regsIndices[i]);
	}
	for (i = 0; i < 2; i++) {
		if (!qUnits[i]) {
			fprintf(fd, "%s ", unitsTypeNames[qUnits[i]->type]);
		}
		else {
			fprintf(fd, "- ");
		}
	}
	fprintf(fd, (r[0]) ? "Yes " : "No ");
	fprintf(fd, (r[1]) ? "Yes" : "No");
}

int cmdToHex(Instruction* instruction) {
	unsigned int hex = 0;
	hex += instruction->opcode << 24;
	hex += instruction->regDst << 20;
	hex += instruction->regSrc0 << 16;
	hex += instruction->regSrc1 << 12;
	hex += 0xFFFFF & instruction->immidiate;
	return hex;
}

int issueInstructionToUnit(FunctionalUnit* fus, Instruction* instruction, int* regsStatus, int cc) {
	if (instruction->isEmpty) {
		printf("Trying to attach empty instruction to unit.\n");
		return -1;
	}
	int type = instruction->instType;
	if (fus->fu[type]->canInsert) {
		for (int i = 0; i < fus->fu[type]->numOfTotalUnits; i++) {
			if (fus->fu[type]->units[i]->isEmpty) {
				fus->fu[type]->units[i]->instruction = instruction;
				fus->fu[type]->units[i]->busy = Yes;
				fus->fu[type]->units[i]->f_i = instruction->regDst;
				fus->fu[type]->units[i]->f_j = instruction->regSrc0;
				fus->fu[type]->units[i]->f_k = instruction->regSrc1;

				fus->fu[type]->units[i]->r_j = (regsStatus[instruction->regSrc0] == -1) ? Yes : No;
				fus->fu[type]->units[i]->r_k = (regsStatus[instruction->regSrc1] == -1) ? Yes : No;

				fus->fu[type]->units[i]->q_j_type = (regsStatus[instruction->regSrc0]);
				fus->fu[type]->units[i]->q_k_type = (regsStatus[instruction->regSrc0]);
				fus->fu[type]->units[i]->q_j_index = (regsStatus[instruction->regSrc1]);
				fus->fu[type]->units[i]->q_k_index = (regsStatus[instruction->regSrc1]);

				fus->fu[type]->numOfActiveUnits++;
				if (fus->fu[type]->numOfActiveUnits == fus->fu[type]->numOfTotalUnits) {
					fus->fu[type]->canInsert = No;
				}
				fus->fu[type]->units[i]->isEmpty = No;
				
				
				fus->fu[type]->units[i]->instruction->stateCC[ISSUE] = cc;
							   				
				regsStatus[instruction->regDst] = fus->fu[type]->units[i]->type;

				return i;
			}
		}
	}
	printf("Cant attach instruction to unit.\n");
	return -1;
}