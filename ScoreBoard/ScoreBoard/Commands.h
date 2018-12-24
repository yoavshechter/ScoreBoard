#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "Commons.h"

typedef struct opcodeCommand {
	char* cmdName;
	unsigned int opcode;
	unsigned int regDst;
	unsigned int regSrc0;
	unsigned int regSrc1;
	unsigned int immidiate;
	int isEmpty;
}Command;

typedef struct instructionQueue {
	Command* queue[NUM_OF_INSTRUCTION_IN_QUEUE];
	int isQueueFull;
	int isQueueEmpty;
}IntructionQueue;


/*Create command object.*/
Command* createCmd();

/*Free command object.*/
void freeCmd(Command* cmd);

/*Parser command from hex value to cmd fields.*/
void parseCmd(Command* cmd, int cmdLine);

/*Create instruction queue object.*/
IntructionQueue* createInstructionQueue();

/*Free instruction queue object.*/
void freeInstructionQueue(IntructionQueue* queue);

/*Add cmd to instruction queue if queue isnt full.*/
int addCmdToIntructionQueue(IntructionQueue* instQueue, Command* cmd);

/*Remove cmd to instruction queue if queue isnt empty.*/
int removeCmdToIntructionQueue(IntructionQueue* instQueue, int cmdIndex);

/*Check if instruction queue is full.*/
void checkIfQueueIsFullOrEmpty(IntructionQueue* instQueue);