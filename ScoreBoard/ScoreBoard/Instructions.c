#include "Instructions.h"

Instruction* createInstruction() {
	Instruction* src = (Instruction*)malloc(sizeof(Instruction));
	if (!src) {
		return 0;
	}
	src->isEmpty = Yes;
	src->writeToFile = No;
	for (int i = 0; i < NUM_OF_CYCLES_TYPES; i++) {
		src->stateCC[i] = -1;
	}
	src->instRes = -1;
	src->executionTime = -1;
	src->queueIndex = -1;
	src->instType = -1;
	src->instIndex = -1;
	src->status = -1;
	src->fetchedTime = 0;
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
		if (src->opcode != OP_LD) {
			src->instType = src->opcode;
		}
		else {
			src->instType = src->opcode;
		}
	}
	else {
		src->instType = -1;
	}
	printf("Instruction parsed correct(op:%s, dst:F%d, src0:F%d, src1:F%d, imm:%d)\n", opcodeNames[src->opcode], src->regDst, src->regSrc0, src->regSrc1, src->immidiate);
}

InstructionQueue* createInstructionQueue() {
	InstructionQueue* instQueue = (InstructionQueue*)malloc(sizeof(InstructionQueue));
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

void freeInstructionQueue(InstructionQueue* instQueue) {
	if (!instQueue) {
		return;
	}
	for (int i = 0; i < NUM_OF_INSTRUCTION_IN_QUEUE; i++) {
		freeInstruction(instQueue->queue[i]);
	}
}

int addInstructionToInstructionQueue(InstructionQueue* instQueue, Instruction* instruction) {
	if (instruction->instType == -1) {
		printf("Instructions is HALT, will not push to the queue.\n");
		return -1;
	}
	if (instQueue->isQueueFull) {
		printf("Instructions Queue is full, cant insert more commands.\n");
		return -1;
	}
	for (int i = 0; i < NUM_OF_INSTRUCTION_IN_QUEUE; i++) {
		if (instQueue->queue[i]->isEmpty) {
			printf("Insert cmd to queue at position %d.\n", i);
			instQueue->queue[i] = instruction;
			instruction->queueIndex = i;
			checkIfQueueIsFullOrEmpty(instQueue);
			return i;
		}
	}
	return -1;
}

int removeInstructionFromInstructionQueue(InstructionQueue* instQueue, int instIndex) {
	if (instQueue->isQueueEmpty) {
		printf("Instructions Queue is empty, cant remove commands.\n");
		return 0;
	}
	instQueue->queue[instIndex] = createInstruction();
	checkIfQueueIsFullOrEmpty(instQueue);
	return 1;
}

void checkIfQueueIsFullOrEmpty(InstructionQueue* instQueue) {
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