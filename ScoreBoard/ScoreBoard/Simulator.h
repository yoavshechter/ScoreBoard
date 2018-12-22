#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "Utils.h"

int simulator(char** filesPaths);

int initializeMemory(FILE* meminFd, char* line, int* memory);

void initializeRegs(double* regs);

void freeSolution(FILE** fds, char* line, Config* cfg, Command* cmd);