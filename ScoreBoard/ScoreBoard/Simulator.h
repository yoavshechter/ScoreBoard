#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "Utils.h"



static int numOfWorkingUnits = 0;

/*Simulator function, execute all simulator steps.*/
int simulator(char** filesPaths);

/*Initialize memory array with data from memin.txt file.*/
int initializeMemory(FILE* meminFd, char* line, int* memory);

/*Initialize registers.*/
void initializeRegs(double* regs);

int issue(FunctionalUnit* fu, InstructionQueue* queue, int* resultTypes, int* resultIndexes, int cc);

/*Attach instruction to correct unit in the functional units struct*/
int issueInstructionToUnit(FunctionalUnit* fus, Instruction* instruction, int* resultTypes, int* resultIndexes, int cc);

void readOperand(FunctionalUnit* fu, double* regs, int cc);

void execution(FunctionalUnit* fu, int* mem, double* regs, int cc);

void executionInstruction(FunctionalUnit* fu, int* mem, double* regs, unsigned int instOp, int i, int j, int cc);

void writeResult(FILE** fds, FunctionalUnit* fu, InstructionQueue* queue, int* mem, int* resultTypes, int* resultIndexes, double* regs, int cc);

void writeResultFunctionalUnit(FILE** fds, FunctionalUnit* fus, Unit* unit, int* mem, int* resultTypes, int* resultIndexes, double* regs, int cc);

void checkIfLdAndStCollide(FunctionalUnit* fu, Instruction* stInst, int cc);

/*Free all the data that has been allocated for exit solution properly.*/
void freeSolution(FILE** fds, char* line, Config* cfg, Instruction* cmd, FunctionalUnit* fus, InstructionQueue* queue);

