#include "Commands.h"

Command* createCmd() {
	Command* cmd = (Command*)malloc(sizeof(Command));
	if (!cmd) {
		return 0;
	}
	cmd->isEmpty = Yes;
	return cmd;
}

void freeCmd(Command* cmd) {
	if (!cmd) {
		return;
	}
	free(cmd);
}

void parseCmd(Command* cmd, int cmdLine) {
	unsigned int temp = cmdLine;
	temp <<= 4;
	cmd->opcode = 0xFFFFFFF & (temp >> 28);
	temp <<= 4;
	cmd->regDst = 0xFFFFFFF & (temp >> 28);
	temp <<= 4;
	cmd->regSrc0 = 0xFFFFFFF & (temp >> 28);
	temp <<= 4;
	cmd->regSrc1 = 0xFFFFFFF & (temp >> 28);
	temp <<= 4;
	cmd->immidiate = 0xFFFFF & (temp >> 20);
	cmd->isEmpty = No;
}

IntructionQueue* createInstructionQueue() {
	IntructionQueue* instQueue = (IntructionQueue*)malloc(sizeof(IntructionQueue));
	if (!instQueue) {
		return 0;
	}
	for (int i = 0; i < NUM_OF_INSTRUCTION_IN_QUEUE; i++) {
		instQueue->queue[i] = createCmd();
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
		freeCmd(instQueue->queue[i]);
	}
}

int addCmdToIntructionQueue(IntructionQueue* instQueue, Command* cmd) {
	if (instQueue->isQueueFull) {
		printf("Instructions Queue is full, cant insert more commands.\n");
		return 0;
	}
	for (int i = 0; i < NUM_OF_INSTRUCTION_IN_QUEUE; i++) {
		if (instQueue->queue[i]->isEmpty) {
			freeCmd(instQueue->queue[i]);
			printf("Insert cmd to queue at position %d.\n", i);
			instQueue->queue[i] = cmd;
			break;
		}
	}
	checkIfQueueIsFullOrEmpty(instQueue);
	return 1;
}

int removeCmdToIntructionQueue(IntructionQueue* instQueue, int cmdIndex) {
	if (instQueue->isQueueEmpty) {
		printf("Instructions Queue is empty, cant remove commands.\n");
		return 0;
	}
	instQueue->queue[cmdIndex] = 0;
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