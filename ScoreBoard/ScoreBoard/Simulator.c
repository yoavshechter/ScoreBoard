#define _CRT_SECURE_NO_WARNINGS
#include "Simulator.h"


int simulator(char** filesPaths) {
	FILE* filesFd[NUM_OF_FILES];
	Config* cfg;
	InstructionQueue* instQueue;
	FunctionalUnit* functionalUnit;
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
	int haltSet = No;
	int tempNumOfWorkingInst = numOfWorkingUnits;
	int canFetch = Yes;

	int resultTypes[NUM_OF_REGISTERS];
	int resultIndexes[NUM_OF_REGISTERS];
	for (int i = 0; i < NUM_OF_REGISTERS; i++) {
		resultTypes[i] = -1;
		resultIndexes[i] = -1;
	}

	char* line = calloc(1, sizeof(char) * MAX_LINE_LENGTH);
	if (!line) {
		printf("Error! couldnt allocated memory for line.\n");
		freeSolution(0, 0, 0, 0, 0, 0);
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

	while (1) {
		cc++;
		printf("*****************************START_CLOCK_CYCLE_NUM_%d*****************************\n", cc);
		printTraceunitFile(filesFd[TRACEUNIT], functionalUnit, resultTypes, resultIndexes, cc);
		Instruction* instruction = createInstruction();
		if (!instruction) {
			printf("Error! failed to create cmd from memin.\n");
			freeSolution(filesFd, line, cfg, instruction, functionalUnit, instQueue);
			return 0;
		}
		printf("*****************************FETCH*****************************\n");
		/*Create command from memory.*/
		if (!haltSet) {
			parseInstruction(instruction, memory[instNum]);
			if (instruction->opcode == OP_HALT) {
				haltSet = Yes;
				canRun = No;
				for (int i = 0; i < NUM_OF_REGISTERS; i++) {
					if (resultTypes[i] != -1) {
						canRun = Yes;
					}
				}
			}
			instQueueIndex = addInstructionToInstructionQueue(instQueue, instruction);
			if (instQueueIndex != -1) {
				/*Adding new command to instruction queue work well.*/
				instNum++;
				instruction->fetchedTime = cc;
			}
			if (cc == 0) {
				continue;
			}
			printf("*****************************ISSUE*****************************\n");
			tempNumOfWorkingInst = issue(functionalUnit, instQueue, resultTypes, resultIndexes, instQueueIndex, cc);
			if (tempNumOfWorkingInst == 0) {
				removeInstructionFromInstructionQueue(instQueue, instQueueIndex);
				instNum--;
				instruction->fetchedTime = -1;
			}
			else {
				numOfWorkingUnits += tempNumOfWorkingInst;
			}
		}
		else {
			canRun = No;
			for (int i = 0; i < NUM_OF_REGISTERS; i++) {
				if (resultTypes[i] != -1) {
					canRun = Yes;
				}
			}
			if (!canRun && numOfWorkingUnits == 0) {
				cc--;
				break;
			}
			printf("*****************************ISSUE*****************************\n");
			if (numOfWorkingUnits > 0) {
				numOfWorkingUnits += issue(functionalUnit, instQueue, resultTypes, resultIndexes, instQueueIndex, cc);
			}

		}
		printf("*****************************READ_OPERAND*****************************\n");
		readOperand(functionalUnit, regs, cc);
		printf("*****************************EXECUTION*****************************\n");
		execution(functionalUnit, memory, regs, cc);
		printf("*****************************WRITE_resultTypes*****************************\n");
		writeResult(filesFd, functionalUnit, instQueue, memory, resultTypes, resultIndexes, regs, cc);

		cleanAndWriteToFiles(filesFd[TRACEINST], functionalUnit, instQueue);

		printf("Result Type:");
		for (int i = 0; i < NUM_OF_REGISTERS; i++) {
			if (resultTypes[i] != -1) {
				printf(" %s%d", unitsTypeNames[resultTypes[i]], resultIndexes[i]);
			}
			else {
				printf(" -");
			}
		}
		printf("\n");	
	}

	printUnitsToTraceInstFile(filesFd[TRACEINST]);

	printMemoutFile(filesFd[MEMOUT], memory, MEMORY_LENGTH);
	
	printRegoutFile(filesFd[REGOUT], regs);

	freeSolution(filesFd, line, cfg, 0, functionalUnit, instQueue);
	printf("End of Simulator!!!\n");
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

int issue(FunctionalUnit* fu, InstructionQueue* queue, int* resultTypes, int* resultIndexes, int cmdIndex, int cc) {
	int instUnitIndex = -1;
	for (int i = 0; i < NUM_OF_INSTRUCTION_IN_QUEUE; i++) {
		instUnitIndex += issueInstructionToUnit(fu, queue->queue[i], resultTypes, resultIndexes, cc);
		if (instUnitIndex == 0) {
			return 1;
		}
	}
	printf("Error, Issue failed.\n");
	return 0;
}

int issueInstructionToUnit(FunctionalUnit* fus, Instruction* instruction, int* resultTypes, int* resultIndexes, int cc) {
	if (instruction->isEmpty == Yes || !instruction) {
		return 0;
	}
	int type = instruction->instType;
	/*Halt instruction*/
	if (type < 0 || type > 5) {
		printf("Halt Instruction occuered.\n");
		return 0;
	}
	if (fus->fu[type]->canInsert) {
		for (int i = 0; i < fus->fu[type]->numOfTotalUnits; i++) {
			if (fus->fu[type]->units[i]->isEmpty || !fus->fu[type]->units[i]) {
				if (!fus->fu[type]->units[i]->busy && instruction->fetchedTime != cc && instruction->stateCC[ISSUE] == -1) {
					if ((resultTypes[instruction->regDst] == -1 && type != UNIT_ST )||( type == UNIT_ST)) {
						fus->fu[type]->units[i]->busy = Yes;
						fus->fu[type]->units[i]->instruction = instruction;
						fus->fu[type]->units[i]->op = instruction->opcode;

						fus->fu[type]->units[i]->f_i = instruction->regDst;
						fus->fu[type]->units[i]->f_j = instruction->regSrc0;
						fus->fu[type]->units[i]->f_k = instruction->regSrc1;

						fus->fu[type]->units[i]->q_j_index = resultIndexes[instruction->regSrc0];
						fus->fu[type]->units[i]->q_k_index = resultIndexes[instruction->regSrc1];

						fus->fu[type]->units[i]->q_j_type = resultTypes[instruction->regSrc0];
						fus->fu[type]->units[i]->q_k_type = resultTypes[instruction->regSrc1];

						fus->fu[type]->units[i]->r_j = (resultTypes[instruction->regSrc0] == -1) ? Yes : No;
						fus->fu[type]->units[i]->r_k = (resultTypes[instruction->regSrc1] == -1) ? Yes : No;

						fus->fu[type]->numOfActiveUnits++;
						if (fus->fu[type]->numOfActiveUnits == fus->fu[type]->numOfTotalUnits) {
							fus->fu[type]->canInsert = No;
						}
						fus->fu[type]->units[i]->isEmpty = No;

						fus->fu[type]->units[i]->instruction->stateCC[ISSUE] = cc;

						if (fus->fu[type]->units[i]->type != UNIT_ST) {
							resultTypes[instruction->regDst] = fus->fu[type]->units[i]->type;
							resultIndexes[instruction->regDst] = fus->fu[type]->units[i]->unitNum;
						}

						fus->fu[type]->units[i]->instruction->instIndex = i;

						printUnit(fus->fu[type]->units[i]);
						printf("ISSUE: %s%d at cc:%d\n", unitsTypeNames[type], i, cc);
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

void readOperand(FunctionalUnit* fu, double* regs, int cc){
	for (int i = 0; i < NUM_OF_UNITS; i++) {
		for (int j = 0; j < fu->fu[i]->numOfTotalUnits; j++) {
			if (!fu->fu[i]->units[j]->isEmpty && !fu->fu[i]->units[j]->instruction->isEmpty) {
				if (fu->fu[i]->units[j]->instruction->stateCC[ISSUE] < cc) {
					if (fu->fu[i]->units[j]->r_j == Yes && fu->fu[i]->units[j]->r_k == Yes) {
						if (fu->fu[i]->units[j]->instruction->stateCC[READ_OPERAND] == -1) {
							fu->fu[i]->units[j]->f_j_value = regs[fu->fu[i]->units[j]->f_j];
							fu->fu[i]->units[j]->f_k_value = regs[fu->fu[i]->units[j]->f_k];

							fu->fu[i]->units[j]->r_j = No;
							fu->fu[i]->units[j]->r_k = No;
							fu->fu[i]->units[j]->instruction->stateCC[READ_OPERAND] = cc;
							if (fu->fu[i]->delay == 1) {
								fu->fu[i]->units[j]->instruction->executionTime = cc + fu->fu[i]->delay;
							}
							else {
								fu->fu[i]->units[j]->instruction->executionTime = cc + fu->fu[i]->delay - 1;
							}
							printf("READ_OPERAND: %s%d at cc:%d\n", opcodeNames[i], j, cc);
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
				if (fu->fu[i]->units[j]->instruction->executionTime >= 0 &&
					0 < fu->fu[i]->units[j]->instruction->stateCC[READ_OPERAND] &&
					fu->fu[i]->units[j]->instruction->stateCC[READ_OPERAND] < cc) {

					if (fu->fu[i]->units[j]->instruction->executionTime == cc) {
						if (fu->fu[i]->units[j]->instruction->instRes == -1) {
							executionInstruction(fu, mem, regs, fu->fu[i]->units[j]->instruction->opcode, i, j, cc);
							printf("EXECUTION: %s%d at cc:%d\n", opcodeNames[i], j, cc);
							
						}
						fu->fu[i]->units[j]->instruction->stateCC[EXECUTION] = cc;
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
		fu->fu[i]->units[j]->instruction->instRes = fu->fu[i]->units[j]->f_j_value + fu->fu[i]->units[j]->f_k_value;
		printf("Execution: Add F[%d] + F[%d] = %f\n", fu->fu[i]->units[j]->f_j, fu->fu[i]->units[j]->f_k, fu->fu[i]->units[j]->instruction->instRes);
		break;
	case OP_SUB:
		fu->fu[i]->units[j]->instruction->instRes = fu->fu[i]->units[j]->f_j_value - fu->fu[i]->units[j]->f_k_value;
		printf("Execution: Sub F[%d] - F[%d] = %f\n", fu->fu[i]->units[j]->f_j, fu->fu[i]->units[j]->f_k, fu->fu[i]->units[j]->instruction->instRes);
		break;
	case OP_MULT:
		fu->fu[i]->units[j]->instruction->instRes = fu->fu[i]->units[j]->f_j_value * fu->fu[i]->units[j]->f_k_value;
		printf("Execution: Mult F[%d] * F[%d] = %f\n", fu->fu[i]->units[j]->f_j, fu->fu[i]->units[j]->f_k, fu->fu[i]->units[j]->instruction->instRes);
		break;
	case OP_DIV:
		fu->fu[i]->units[j]->instruction->instRes = fu->fu[i]->units[j]->f_j_value / fu->fu[i]->units[j]->f_k_value;
		printf("Execution: Div F[%d] / F[%d] = %f\n", fu->fu[i]->units[j]->f_j, fu->fu[i]->units[j]->f_k, fu->fu[i]->units[j]->instruction->instRes);
		break;
	case OP_LD:
		tempLD = mem[fu->fu[i]->units[j]->instruction->immidiate];
		fu->fu[i]->units[j]->instruction->instRes = singlePrecisionToFloat(tempLD);
		printf("Execution: Ld F[%d] = MEM[%d] = %f\n", fu->fu[i]->units[j]->f_i, tempLD, fu->fu[i]->units[j]->instruction->instRes);
		break;
	case OP_ST:
		fu->fu[i]->units[j]->instruction->instRes = fu->fu[i]->units[j]->f_k_value;
		checkIfLdAndStCollide(fu, fu->fu[i]->units[j]->instruction, cc);
		printf("Execution: St MEM[%d] = F[%d] = %f\n", fu->fu[i]->units[j]->instruction->immidiate,
			fu->fu[i]->units[j]->f_k,
			regs[fu->fu[i]->units[j]->f_k]);
		break;
	}
}

void checkIfLdAndStCollide(FunctionalUnit* fu, Instruction* stInst, int cc) {
	for (int i = 0; i < fu->fu[LD_UNIT]->numOfTotalUnits; i++) {
		if(!fu->fu[LD_UNIT]->units[i]->isEmpty){
			if (fu->fu[LD_UNIT]->units[i]->instruction->immidiate == stInst->immidiate) {
				if (fu->fu[LD_UNIT]->units[i]->instruction->stateCC[ISSUE] < stInst->stateCC[ISSUE]) {
					if (fu->fu[LD_UNIT]->units[i]->instruction->stateCC[EXECUTION] <= cc &&
						stInst->executionTime == cc) {
						stInst->executionTime++;
					}
				}
			}
		}
	}
}

void writeResult(FILE** fds, FunctionalUnit* fu, InstructionQueue* queue, int* mem, int* resultTypes, int* resultIndexes, double* regs, int cc) {
	int canWriteresultTypes = 0, numOfUnits;
	for (int x = 0; x < NUM_OF_UNITS; x++) {
		numOfUnits = 0;
		for (int y = 0; y < fu->fu[x]->numOfTotalUnits; y++) {
			if (fu->fu[x]->units[y]->isEmpty) {
				continue;
			}
			writeResultFunctionalUnit(fds, fu, fu->fu[x]->units[y], mem, resultTypes, resultIndexes, regs, cc);
		}
	}
}

void writeResultFunctionalUnit(FILE** fds, FunctionalUnit* fus, Unit* unit, int* mem, int* resultTypes, int* resultIndexes, double* regs, int cc) {
	int canWriteresultTypes = 0;
	if (!unit->canWriteResult) {
		for (int x = 0; x < NUM_OF_UNITS; x++) {
			for (int y = 0; y < fus->fu[x]->numOfTotalUnits; y++) {
				if (fus->fu[x]->units[y]->isEmpty) {
					continue;
				}
				if ((fus->fu[x]->units[y]->f_j != unit->f_i || fus->fu[x]->units[y]->r_j == No) &&
					(fus->fu[x]->units[y]->f_k != unit->f_i || fus->fu[x]->units[y]->r_k == No)) {
					canWriteresultTypes++;
				}
			}
		}
		if (canWriteresultTypes != numOfWorkingUnits || numOfWorkingUnits == 0) {
			printf("Not all units are ready for write resultTypes.\n");
			return;
		}
		else {
			unit->canWriteResult = Yes;
		}
	}
	else {
		canWriteresultTypes = 0;
		for (int x = 0; x < NUM_OF_UNITS; x++) {
			for (int y = 0; y < fus->fu[x]->numOfTotalUnits; y++) {
				if (fus->fu[x]->units[y]->isEmpty) {
					continue;
				}
				if (0 < unit->instruction->stateCC[EXECUTION] && unit->instruction->stateCC[EXECUTION] < cc) {
					if (isUnitsEqual(fus, fus->fu[x]->units[y]->q_j_type, fus->fu[x]->units[y]->q_j_index, unit, 1)) {
						fus->fu[x]->units[y]->q_j_index = -1;
						fus->fu[x]->units[y]->q_j_type = -1;
						fus->fu[x]->units[y]->r_j = Yes;
					}
					else if (isUnitsEqual(fus, fus->fu[x]->units[y]->q_k_type, fus->fu[x]->units[y]->q_k_index, unit, 0)) {
						fus->fu[x]->units[y]->q_k_index = -1;
						fus->fu[x]->units[y]->q_k_type = -1;
						fus->fu[x]->units[y]->r_k = Yes;
					}
				}
			}
		}
		if (0 < unit->instruction->stateCC[EXECUTION] && unit->instruction->stateCC[EXECUTION] < cc) {
			switch (unit->type) {
			case UNIT_ST:
				mem[unit->instruction->immidiate] = floatToSinglePrecision(unit->instruction->instRes);
				break;
			case UNIT_LD:
				regs[unit->f_i] = unit->instruction->instRes;
				break;
			default:
				regs[unit->f_i] = unit->instruction->instRes;
				break;
			}
			numOfWorkingUnits--;
			unit->instruction->stateCC[WRITE_RESILT] = cc;
			resultTypes[unit->f_i] = -1;
			resultIndexes[unit->f_i] = -1;
			unit->busy = No;
			printf("Free Unit type: %s, num:%d.\n", unitsTypeNames[unit->type], unit->unitNum);
			printf("WRITE_RESULT: %s%d at cc:%d\n", unitsTypeNames[unit->type], unit->unitNum, cc);
		}
	}
}

void freeSolution(FILE** fds, char* line, Config* cfg, Instruction* cmd, FunctionalUnit* fus, InstructionQueue* queue) {
	for (int i = 0; i < NUM_OF_FILES; i++) {
		fclose(fds[i]);
	}
	if (line) { free(line); }
	freeInstruction(cmd);
	freeConfig(cfg);
	freeFunctionalUnit(fus);
	freeInstructionQueue(queue);
}
