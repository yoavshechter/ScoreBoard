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
	unsigned int line;
	
	int instType;
	int instIndex;
	int status;
	int isEmpty;
	int fetchedTime;
	int queueIndex;
	int writeToFile;
	int canIssue;

	int executionTime;
	int stateCC[NUM_OF_CYCLES_TYPES];
	double instRes;
}Instruction;

typedef struct instructionQueue {
	Instruction* queue[NUM_OF_INSTRUCTION_IN_QUEUE];
	int isQueueFull;
	int isQueueEmpty;
}InstructionQueue;


/*Create command object.*/
Instruction* createInstruction();

/*Free command object.*/
void freeInstruction(Instruction* src);

/*Parser command from hex value to cmd fields.*/
void parseInstruction(Instruction* src, int cmdLine);

/*Create instruction queue object.*/
InstructionQueue* createInstructionQueue();

/*Free instruction queue object.*/
void freeInstructionQueue(InstructionQueue* queue);

/*Add cmd to instruction queue if queue isnt full.*/
int addInstructionToInstructionQueue(InstructionQueue* instQueue, Instruction* inst);

/*Remove cmd to instruction queue if queue isnt empty.*/
int removeInstructionFromInstructionQueue(InstructionQueue* instQueue, int instIndex);

/*Check if instruction queue is full.*/
void checkIfQueueIsFullOrEmpty(InstructionQueue* instQueue);