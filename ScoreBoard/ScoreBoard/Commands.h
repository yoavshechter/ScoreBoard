#pragma once
#include <stdlib.h>
#include "Commons.h"

typedef struct opcodeCommand {
	char* cmdName;
	unsigned int opcode;
	unsigned int regDst;
	unsigned int regSrc0;
	unsigned int regSrc1;
	unsigned int immidiate;
}Command;

Command* createCmd();

void freeCmd(Command* cmd);

Command* parseCmd(Command* cmd, int cmdLine);
