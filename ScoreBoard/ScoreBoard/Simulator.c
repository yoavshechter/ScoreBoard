#define _CRT_SECURE_NO_WARNINGS
#include "Simulator.h"


int simulator(char** filesPaths) {
	FILE* filesFd[NUM_OF_FILES];
	Config* cfg;
	IntructionQueue* instQueue;
	FunctionalUnit* functionalUnit;
	int memory[MEMORY_LENGTH] = { 0 };
	double regs[NUM_OF_REGISTERS] = { 0 };
	int regsStatus[NUM_OF_REGISTERS];
	int numOfLines = 0;
	unsigned int pc = 0;
	int instNum = 0;
	unsigned int op = 0;
	unsigned int lineNum = 0;
	int instQueueIndex = -1;
	int cc = 0;
	int canRun = Yes;
	
	for (int i = 0; i < NUM_OF_REGISTERS; i++) {
		regsStatus[i] = -1;
	}

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
				if (regsStatus[i] != -1) {
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
		issue(instQueue, functionalUnit, instQueueIndex, cc, regsStatus);
		if (cc == 0) {
			cc++;
			continue;
		}
		printf("*****************************READ_OPERAND*****************************\n");
		readOperand(functionalUnit, cc);
		printf("*****************************EXECUTION*****************************\n");
		execution();
		printf("*****************************WRITE_RESULT*****************************\n");
		writeResult();

		cc++;
	}

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

int issue(IntructionQueue* queue, FunctionalUnit* fu, int cmdIndex, int cc, int* regsStatus){
	int instUnitIndex = issueInstructionToUnit(fu, queue->queue[cmdIndex], regsStatus, cc);
	if (instUnitIndex == -1) {
		printf("Error, Issue failed.\n");
		return 0;
	}
	return 1;
}

void readOperand(FunctionalUnit* fu, int cc){
	for (int i = 0; i < NUM_OF_UNITS; i++) {
		for (int j = 0; j < fu->fu[i]->numOfTotalUnits; j++) {
			if (!fu->fu[i]->units[j]->isEmpty) {
				if (fu->fu[i]->units[j]->instruction->stateCC[READ_OPERAND] == -1) {
					if (i == UNIT_ST && fu->fu[i]->units[j]->r_k == Yes) {
						fu->fu[i]->units[j]->instruction->stateCC[READ_OPERAND] = cc;
						fu->fu[i]->units[j]->instruction->remainTime = fu->fu[i]->delay - 1;
					}
					else if (i == UNIT_LD && fu->fu[i]->units[j]->instruction->stateCC[ISSUE] < cc) {
						fu->fu[i]->units[j]->instruction->stateCC[READ_OPERAND] = cc;
						fu->fu[i]->units[j]->instruction->remainTime = fu->fu[i]->delay - 1;
					}
					else {
						if (fu->fu[i]->units[j]->r_j == Yes && fu->fu[i]->units[j]->r_k == Yes) {
							fu->fu[i]->units[j]->instruction->stateCC[READ_OPERAND] = cc;
							fu->fu[i]->units[j]->instruction->remainTime = fu->fu[i]->delay - 1;
							if (fu->fu[i]->type == UNIT_ADD) {
								fu->fu[i]->units[j]->q_j_index = -1;
								fu->fu[i]->units[j]->q_k_index = -1;
							}
						}
					}
				}
			}
		}
	}
}

void execution(){

}

void writeResult() {

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