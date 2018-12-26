#define _CRT_SECURE_NO_WARNINGS
#include "Simulator.h"


int simulator(char** filesPaths) {
	FILE* filesFd[NUM_OF_FILES];
	Config* cfg;
	IntructionQueue* instQueue;
	FunctionalUnit* functionalUnit;
	Unit* result[NUM_OF_REGISTERS] = { 0 };
	int memory[MEMORY_LENGTH] = { 0 };
	double regs[NUM_OF_REGISTERS] = { 0 };

	int numOfLines = 0;
	unsigned int pc = 0;
	int instNum = 0;
	unsigned int op = 0;
	unsigned int lineNum = 0;
	int instQueueIndex = -1;
	int cc = -1;
	int canRun = Yes;
	int numOfActiveUnits = 0;

	char* line = calloc(1, sizeof(char) * MAX_LINE_LENGTH);
	if (!line) {
		printf("Error! couldnt allocated memory for line.\n");
		return 0;
	}
	/*Instructions queue initialize.*/
	instQueue = createInstructionQueue();
	if (!instQueue) {
		printf("Error! couldnt allocated memory for instruction queue.\n");
		freeSolution(0, line, 0, 0, 0, 0);
		return 0;
	}
	/*Files verifictaion.*/
	if (!verifyFiles(filesFd, filesPaths)) {
		return 0;
	}
	/*Memory initialize.*/
	if ((numOfLines = initializeMemory(filesFd[MEMIN], line, memory)) == 0) {
		printf("Error! memin.txt file is empty.\n");
		freeSolution(filesFd, line, 0, 0, 0, instQueue);
		return 0;
	}
	/*Registers initialize.*/
	initializeRegs(regs);
	if ((cfg = parseConfig(filesFd[CONFIG], line)) == 0) {
		printf("Error! failed to create config obg from cfg.txt.\n");
		freeSolution(filesFd, line, cfg, 0, 0, instQueue);
		return 0;
	}
	/*Functional units struct initialize*/;
	if ((functionalUnit = createFunctionalUnit(cfg)) == 0) {
		printf("Error! failed to create functional units struct from config.\n");
		freeSolution(filesFd, line, cfg, 0, functionalUnit, instQueue);
		return 0;
	}
	while (canRun) {
		cc++;
		printf("*****************************FETCH*****************************\n");
		/*Create command from memory.*/
		Instruction* instruction = createInstruction();
		parseInstruction(instruction, memory[instNum]);
		if (!instruction){
			printf("Error! failed to create cmd from memin.\n");
			freeSolution(filesFd, line, cfg, instruction, functionalUnit, instQueue);
			return 0;
		}
		else if (instruction->opcode == OP_HALT) {
			for (int i = 0; i < NUM_OF_REGISTERS; i++) {
				if (!result[i]) {
					canRun = No;
				}
			}
		}
		instQueueIndex = addInstructionToInstructionQueue(instQueue, instruction);
		if (instQueueIndex != -1) {
			/*Adding new command to instruction queue work well.*/
			instNum++;
		}
		printf("*****************************ISSUE*****************************\n");
		issue(functionalUnit, instQueue, &result, instQueueIndex, cc);
		if (cc == 0) {
			continue;
		}
		printf("*****************************READ_OPERAND*****************************\n");
		readOperand(functionalUnit, cc);
		printf("*****************************EXECUTION*****************************\n");
		execution(functionalUnit, memory, regs, cc);
		printf("*****************************WRITE_RESULT*****************************\n");
		writeResult(functionalUnit, instQueue, result, regs, cc, numOfActiveUnits);

		//printTracinstFile(filesFd[TRACEINST], instruction, cc, Unit* unit, int* cycles);
		
		//printTraceunitFile(filesFd[TRACEUNIT], int cycle, Unit* unit, int* regsIndices, Unit** qUnits, int* r);
	}

	printMemoutFile(filesFd[MEMOUT], memory, MEMORY_LENGTH);
	
	printRegoutFile(filesFd[REGOUT], regs);


	printf("End of Simulator!!!");
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
		regs[i] = i/1.0;
	}
}

int issue(FunctionalUnit* fu, IntructionQueue* queue, Unit* result[NUM_OF_REGISTERS], int cmdIndex, int cc){
	int instUnitIndex = issueInstructionToUnit(fu, queue->queue[cmdIndex], &result, cc);
	if (instUnitIndex == -1) {
		printf("Error, Issue failed.\n");
		return 0;
	}
	return 1;
}

int issueInstructionToUnit(FunctionalUnit* fus, Instruction* instruction, Unit* result[NUM_OF_REGISTERS], int cc) {
	if (instruction->isEmpty == Yes) {
		printf("Trying to attach empty instruction to unit.\n");
		return -1;
	}
	int type = instruction->instType;
	if (fus->fu[type]->canInsert) {
		for (int i = 0; i < fus->fu[type]->numOfTotalUnits; i++) {
			if (!fus->fu[type]->units[i]->isEmpty) {
				if (!fus->fu[type]->units[i]->busy && !result[instruction->regDst]) {
					fus->fu[type]->units[i]->busy = Yes;
					fus->fu[type]->units[i]->instruction = instruction;
					fus->fu[type]->units[i]->op = instruction->opcode;

					fus->fu[type]->units[i]->f_i = instruction->regDst;
					fus->fu[type]->units[i]->f_j = instruction->regSrc0;
					fus->fu[type]->units[i]->f_k = instruction->regSrc1;

					fus->fu[type]->units[i]->q_j = result[instruction->regSrc0];
					fus->fu[type]->units[i]->q_k = result[instruction->regSrc0];

					fus->fu[type]->units[i]->r_j = (!result[instruction->regSrc0]) ? Yes : No;
					fus->fu[type]->units[i]->r_k = (!result[instruction->regSrc1]) ? Yes : No;

					fus->fu[type]->numOfActiveUnits++;
					if (fus->fu[type]->numOfActiveUnits == fus->fu[type]->numOfTotalUnits) {
						fus->fu[type]->canInsert = No;
					}
					fus->fu[type]->units[i]->isEmpty = No;

					fus->fu[type]->units[i]->instruction->stateCC[ISSUE] = cc;

					result[instruction->regDst] = fus->fu[type]->units[i];

					return i;
				}
			}
		}
	}
	printf("Cant attach instruction to unit.\n");
	return 0;
}

void readOperand(FunctionalUnit* fu, int cc){
	for (int i = 0; i < NUM_OF_UNITS; i++) {
		for (int j = 0; j < fu->fu[i]->numOfTotalUnits; j++) {
			if (!fu->fu[i]->units[j]->isEmpty) {
				if (fu->fu[i]->units[j]->r_j == Yes && fu->fu[i]->units[j]->r_k == Yes){
					if (fu->fu[i]->units[j]->instruction->stateCC[READ_OPERAND] == -1) {
						if (i != UNIT_LD) {
							fu->fu[i]->units[j]->instruction->stateCC[READ_OPERAND] = cc;
							fu->fu[i]->units[j]->instruction->remainTime = fu->fu[i]->delay - 1;
							if (fu->fu[i]->type == UNIT_ADD) {
								fu->fu[i]->units[j]->q_j = 0;
								fu->fu[i]->units[j]->q_k = 0;
							}
						}
						else if (i == UNIT_LD && fu->fu[i]->units[j]->instruction->stateCC[ISSUE] < cc) {
							fu->fu[i]->units[j]->instruction->stateCC[READ_OPERAND] = cc;
							fu->fu[i]->units[j]->instruction->remainTime = fu->fu[i]->delay - 1;
						}
					}
				}
			}
		}
	}
}

void execution(FunctionalUnit* fu, int* mem, double* regs, int cc){
	for (int i = 0; i < NUM_OF_UNITS; i++) {
		for (int j = 0; j < fu->fu[i]->numOfTotalUnits; j++) {
			if (!fu->fu[i]->units[j]->isEmpty) {
				if (fu->fu[i]->units[j]->instruction->remainTime >= 0 &&
					0 < fu->fu[i]->units[j]->instruction->stateCC[READ_OPERAND] <= cc) {
					if (fu->fu[i]->units[j]->instruction->instRes == -1) {
						executionInstruction(fu, mem, regs, fu->fu[i]->units[j]->instruction->opcode, i, j, cc);
					}
				}
			}
		}
	}
}

void executionInstruction(FunctionalUnit* fu, int* mem, double* regs, unsigned int instOp, int i, int j, int cc) {
	int tempLD = 0;
	switch (instOp) {
	case OP_ADD:
		fu->fu[i]->units[j]->instruction->instRes = regs[fu->fu[i]->units[j]->f_j] + regs[fu->fu[i]->units[j]->f_k];
		printf("Execution: Add F[%d]+F[%d]", fu->fu[i]->units[j]->f_j, fu->fu[i]->units[j]->f_k);
		break;
	case OP_SUB:
		fu->fu[i]->units[j]->instruction->instRes = regs[fu->fu[i]->units[j]->f_j] - regs[fu->fu[i]->units[j]->f_k];
		printf("Execution: Sub F[%d]-F[%d]", fu->fu[i]->units[j]->f_j, fu->fu[i]->units[j]->f_k);
		break;
	case OP_MULT:
		fu->fu[i]->units[j]->instruction->instRes = regs[fu->fu[i]->units[j]->f_j] * regs[fu->fu[i]->units[j]->f_k];
		printf("Execution: Mult F[%d]*F[%d]", fu->fu[i]->units[j]->f_j, fu->fu[i]->units[j]->f_k);
		break;
	case OP_DIV:
		fu->fu[i]->units[j]->instruction->instRes = regs[fu->fu[i]->units[j]->f_j] / regs[fu->fu[i]->units[j]->f_k];
		printf("Execution: Div F[%d]/F[%d]", fu->fu[i]->units[j]->f_j, fu->fu[i]->units[j]->f_k);
		break;
	case OP_LD:
		tempLD = mem[fu->fu[i]->units[j]->instruction->immidiate];
		fu->fu[i]->units[j]->instruction->instRes = singlePrecisionToFloat(tempLD);
		printf("Execution: Ld F[%f] = MEM[%d]", fu->fu[i]->units[j]->instruction->instRes, tempLD);
		break;
	case OP_ST:
		fu->fu[i]->units[j]->instruction->instRes = regs[fu->fu[i]->units[j]->f_k];
		checkIfLdAndStCollide(fu, fu->fu[i]->units[j]->instruction, cc);
		printf("Execution: St MEM[%d] = F[%d] = %f", mem[fu->fu[i]->units[j]->instruction->immidiate],
			fu->fu[i]->units[j]->f_k,
			regs[fu->fu[i]->units[j]->f_k]);
		break;
	}
	if (instOp != ST_UNIT) {
		fu->fu[i]->units[j]->instruction->remainTime--;
	}
	if (fu->fu[i]->units[j]->instruction->remainTime == 0) {
		fu->fu[i]->units[j]->instruction->stateCC[EXECUTION] = cc;
		if (instOp == ST_UNIT) {
			mem[fu->fu[i]->units[j]->instruction->immidiate] = floatToSinglePrecision(fu->fu[i]->units[j]->instruction->instRes);
		}
	}
}

void checkIfLdAndStCollide(FunctionalUnit* fu, Instruction* stInst, int cc) {
	for (int i = 0; i < fu->fu[LD_UNIT]->numOfTotalUnits; i++) {
		if (fu->fu[LD_UNIT]->units[i]->instruction->immidiate == stInst->immidiate) {
			if (fu->fu[LD_UNIT]->units[i]->instruction->stateCC[ISSUE] < stInst->stateCC[ISSUE]) {
				if (fu->fu[LD_UNIT]->units[i]->instruction->stateCC[EXECUTION] <= cc &&
					stInst->remainTime == 0) {
					stInst->remainTime++;
				}
			}
		}
	}
}

void writeResult(FunctionalUnit* fu, IntructionQueue* queue, Unit* result[NUM_OF_REGISTERS], double* regs, int cc, int numOfActiveUnits) {
	int canWriteResult = 0, numOfUnits;
	for (int x = 0; x < NUM_OF_UNITS; x++) {
		numOfUnits = 0;
		for (int y = 0; y < fu->fu[x]->numOfTotalUnits; y++) {
			if (fu->fu[x]->units[y]->isEmpty && fu->fu[x]->units[y]->instruction->isEmpty) {
				continue;
			}
			writeResultFunctionalUnit(fu, fu->fu[x]->units[y], result, regs, cc, numOfActiveUnits);
		}
	}
	//updateAllFields(fu->fu[i]->units[j], result);
}

void writeResultFunctionalUnit(FunctionalUnit* fus, Unit* unit, Unit* result[NUM_OF_REGISTERS], double* regs, int cc, int numOfActiveUnits) {
	int canWriteResult = 0;
	for (int x = 0; x < NUM_OF_UNITS; x++) {
		for (int y = 0; y < fus->fu[x]->numOfTotalUnits; y++) {
			if (0 < fus->fu[x]->units[y]->instruction->stateCC[EXECUTION] < cc &&
				fus->fu[x]->units[y]->instruction->remainTime < 0) {
				if ((fus->fu[x]->units[y]->f_j != unit->f_i || fus->fu[x]->units[y]->r_j == No) &&
					(fus->fu[x]->units[y]->f_k != unit->f_i || fus->fu[x]->units[y]->r_k == No)) {
					canWriteResult++;
				}
			}
		}
	}
	if (canWriteResult != numOfActiveUnits) {
		printf("Not all units are ready for write result.\n");
		return;
	}
	for (int x = 0; x < NUM_OF_UNITS; x++) {
		for (int y = 0; y < fus->fu[x]->numOfTotalUnits; y++) {
			if (fus->fu[x]->units[y]->isEmpty && fus->fu[x]->units[y]->instruction->isEmpty) {
				continue;
			}
			if (isUnitsEqual(fus->fu[x]->units[y]->q_j, unit)) {
				fus->fu[x]->units[y]->r_j = Yes;
			}
			if (isUnitsEqual(fus->fu[x]->units[y]->q_k, unit)) {
				fus->fu[x]->units[y]->r_k = Yes;
			}
		}
	}
	result[unit->f_i] = 0;
	unit->busy = No;
}

void updateAllFields(Unit* unit, int* regsStatus) {
	if (regsStatus[unit->f_j] == -1) {
		unit->r_j = 1;
	}
	else if (regsStatus[unit->f_k] == -1) {
		unit->r_k = 1;
	}
	else{
		unit->r_k = 0;
}
}

void freeSolution(FILE** fds, char* line, Config* cfg, Instruction* cmd, FunctionalUnit* fus, IntructionQueue* queue) {
	for (int i = 0; i < NUM_OF_FILES; i++) {
		fclose(fds[i]);
	}
	if (line) { free(line); }
	freeInstruction(cmd);
	freeConfig(cfg);
	freeFunctionalUnit(fus);
	freeInstructionQueue(queue);
}