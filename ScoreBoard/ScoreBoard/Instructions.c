#include "Instructions.h"

Instruction* createInstruction() {
	Instruction* src = (Instruction*)malloc(sizeof(Instruction));
	if (!src) {
		return 0;
	}
	src->isEmpty = Yes;
	for (int i = 0; i < NUM_OF_CYCLES_TYPES; i++) {
		src->stateCC[i] = -1;
	}
	src->instRes = -1;
	src->remainTime = -1;
	src->queueIndex = -1;
	src->instType = -1;
	src->status = -1;
	return src;
}

void freeInstruction(Instruction* src) {
	if (!src) {
		return;
	}
	free(src);
}

void parseInstruction(Instruction* src, int cmdLine) {
	unsigned int temp = cmdLine;
	temp <<= 4;
	src->opcode = 0xFFFFFFF & (temp >> 28);
	temp <<= 4;
	src->regDst = 0xFFFFFFF & (temp >> 28);
	temp <<= 4;
	src->regSrc0 = 0xFFFFFFF & (temp >> 28);
	temp <<= 4;
	src->regSrc1 = 0xFFFFFFF & (temp >> 28);
	temp <<= 4;
	src->immidiate = 0xFFFFF & (temp >> 20);
	src->isEmpty = No;
	if (src->opcode != OP_HALT) {
		src->instType = src->opcode;
	}
	else {
		src->instType = -1;
	}
}

IntructionQueue* createInstructionQueue() {
	IntructionQueue* instQueue = (IntructionQueue*)malloc(sizeof(IntructionQueue));
	if (!instQueue) {
		printf("Error! Failed to allocate memory for instructions queue.\n");
		return 0;
	}
	for (int i = 0; i < NUM_OF_INSTRUCTION_IN_QUEUE; i++) {
		instQueue->queue[i] = createInstruction();
		if (!instQueue->queue[i]) {
			printf("Error! Failed to allocate memory for instructions in queue.\n");
			for (int j = i; j > -1; j--) {
				freeInstruction(instQueue->queue[j]);
			}
			return 0;
		}
	}
	instQueue->isQueueFull = No;
	instQueue->isQueueEmpty = Yes;
	return instQueue;
}

void freeInstructionQueue(IntructionQueue* instQueue) {
	if (!instQueue) {
		return;
	}
	for (int i = 0; i < NUM_OF_INSTRUCTION_IN_QUEUE; i++) {
		freeInstruction(instQueue->queue[i]);
	}
}

int addInstructionToInstructionQueue(IntructionQueue* instQueue, Instruction* instruction) {
	if (instQueue->isQueueFull) {
		printf("Instructions Queue is full, cant insert more commands.\n");
		return -1;
	}
	for (int i = 0; i < NUM_OF_INSTRUCTION_IN_QUEUE; i++) {
		if (instQueue->queue[i]->isEmpty) {
			freeInstruction(instQueue->queue[i]);
			printf("Insert cmd to queue at position %d.\n", i);
			instQueue->queue[i] = instruction;
			instruction->queueIndex = i;
			checkIfQueueIsFullOrEmpty(instQueue);
			return i;
		}
	}
	return -1;
}

int removeInstructionToInstructionQueue(IntructionQueue* instQueue, int instIndex) {
	if (instQueue->isQueueEmpty) {
		printf("Instructions Queue is empty, cant remove commands.\n");
		return 0;
	}
	instQueue->queue[instIndex] = createInstruction();
	checkIfQueueIsFullOrEmpty(instQueue);
	return 1;
}

void checkIfQueueIsFullOrEmpty(IntructionQueue* instQueue) {
	int numOfInstruction = 0;
	for (int i = 0; i < NUM_OF_INSTRUCTION_IN_QUEUE; i++) {
		if (instQueue->queue[i]->isEmpty != 0) {
			numOfInstruction++;
		}
	}
	if (numOfInstruction == NUM_OF_INSTRUCTION_IN_QUEUE) {
		printf("Check if Instructions Queue is full, cant insert more commands.\n");
		instQueue->isQueueFull = Yes;
		instQueue->isQueueEmpty = No;
	}
	else if(numOfInstruction == 0){
		printf("Check if Instructions Queue is empty, cant remove more commands.\n");
		instQueue->isQueueEmpty = Yes;
		instQueue->isQueueFull = No;
	}
	else {
		instQueue->isQueueFull = No;
		instQueue->isQueueEmpty = No;
	}
}