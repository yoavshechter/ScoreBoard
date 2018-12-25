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

int issue(IntructionQueue* queue, FunctionalUnit* fu, int cmdIndex, int cc, int* regsStatus);

void readOperand(FunctionalUnit* fu, int cc);

void execution();

void writeResult();

/*Free all the data that has been allocated for exit solution properly.*/
void freeSolution(FILE** fds, char* line, Config* cfg, Instruction* cmd, FunctionalUnit* fus, IntructionQueue* queue);