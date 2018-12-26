#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "Commons.h"

typedef struct instructionCommand {
	unsigned int opcode;
	unsigned int regDst;
	unsigned int regSrc0;
	unsigned int regSrc1;
	unsigned int immidiate;
	
	int instType;
	int status;
	int isEmpty;
	int queueIndex;

	int remainTime;
	int stateCC[NUM_OF_CYCLES_TYPES];
	double instRes;
}Instruction;

typedef struct instructionQueue {
	Instruction* queue[NUM_OF_INSTRUCTION_IN_QUEUE];
	int isQueueFull;
	int isQueueEmpty;
}IntructionQueue;


/*Create command object.*/
Instruction* createInstruction();

/*Free command object.*/
void freeInstruction(Instruction* src);

/*Parser command from hex value to cmd fields.*/
void parseInstruction(Instruction* src, int cmdLine);

/*Create instruction queue object.*/
IntructionQueue* createInstructionQueue();

/*Free instruction queue object.*/
void freeInstructionQueue(IntructionQueue* queue);

/*Add cmd to instruction queue if queue isnt full.*/
int addInstructionToInstructionQueue(IntructionQueue* instQueue, Instruction* inst);

/*Remove cmd to instruction queue if queue isnt empty.*/
int removeInstructionToInstructionQueue(IntructionQueue* instQueue, int instIndex);

/*Check if instruction queue is full.*/
void checkIfQueueIsFullOrEmpty(IntructionQueue* instQueue);