#define _CRT_SECURE_NO_WARNINGS
#include "Simulator.h"


int simulator(char** filesPaths) {
	FILE* filesFd[NUM_OF_FILES];
	Config* cfg;
	IntructionQueue* instQueue;
	FunctionalUnit* functionalUnit;
	int memory[MEMORY_LENGTH] = { 0 };
	double regs[NUM_OF_REGISTERS] = { 0 };
	int numOfLines = 0;
	unsigned int pc = 0;
	unsigned int op = 0;
	unsigned int lineNum = 0;

	char* line = calloc(1, sizeof(char) * MAX_LINE_LENGTH);
	if (!line) {
		printf("Error! couldnt allocated memory.\n");
		return 0;
	}
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
	}
	/*Functional units struct initialize*/;
	if ((functionalUnit = createFunctionalUnit(cfg)) == 0) {
		printf("Error! failed to create functional units struct from config.\n");
		freeSolution(filesFd, line, cfg, 0, functionalUnit, instQueue);
	}
	while (op != OP_HALT) {
		printf("*****************************FETCH*****************************");
		/*Create command from memory.*/
		Command* cmd = createCmd();
		parseCmd(cmd, memory[pc]);
		if (!cmd){
			freeSolution(filesFd, line, cfg, cmd, functionalUnit, instQueue);
		}
		if (addCmdToIntructionQueue(instQueue, cmd)) {
			/*Adding new command to instruction queue with success.*/
			pc++;
		}
		printf("*****************************ISSUE*****************************");
		issue();
		printf("*****************************READ_OPERAND*****************************");
		readOperand();
		printf("*****************************EXECUTION*****************************");
		execution();
		printf("*****************************WRITE_RESULT*****************************");
		writeResult();
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
		regs[i] = i/1.0;
	}
}

void issue(){
	
}

void readOperand(){}

void execution(){}

void writeResult() {

}

void freeSolution(FILE** fds, char* line, Config* cfg, Command* cmd, FunctionalUnit* fus, IntructionQueue* queue) {
	for (int i = 0; i < NUM_OF_FILES; i++) {
		fclose(fds[i]);
	}
	if (line) { free(line); }
	freeCmd(cmd);
	freeConfig(cfg);
	freeFunctionalUnit(fus);
	freeInstructionQueue(queue);
}