#include "Commands.h"

Command* createCmd() {
	Command* cmd = (Command*)malloc(sizeof(Command));
	if (!cmd) {
		return 0;
	}
	return cmd;
}

void freeCmd(Command* cmd) {
	if (!cmd) {
		return;
	}
	free(cmd);
}

Command* parseCmd(Command* cmd, int cmdLine) {
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
	return cmd;
}