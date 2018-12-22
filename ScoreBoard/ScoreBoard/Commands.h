#pragma once
typedef struct opcodeCommand {
	char* cmdName;
	unsigned int opcode;
	unsigned int regDst;
	unsigned int regSrc0;
	unsigned int regSrc1;
	unsigned int immidiate;
}Command;

Command* createCmd();

void cleanCmd(Command* cmd);

Command* parseCmd(Command* cmd, int cmdLine);
