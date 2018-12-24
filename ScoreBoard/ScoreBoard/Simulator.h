#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "Utils.h"

/*Simulator function, execute all simulator steps.*/
int simulator(char** filesPaths);

/*Initialize memory array with data from memin.txt file.*/
int initializeMemory(FILE* meminFd, char* line, int* memory);

/*Initialize registers.*/
void initializeRegs(double* regs);

void issue();

void readOperand();

void execution();

void writeResult();

/*Free all the data that has been allocated for exit solution properly.*/
void freeSolution(FILE** fds, char* line, Config* cfg, Command* cmd, FunctionalUnit* fus, IntructionQueue* queue);