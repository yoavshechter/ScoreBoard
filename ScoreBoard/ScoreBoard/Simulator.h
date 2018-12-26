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

int issue(FunctionalUnit* fu, IntructionQueue* queue, Unit* result[NUM_OF_REGISTERS], int cmdIndex, int cc);

/*Attach instruction to correct unit in the functional units struct*/
int issueInstructionToUnit(FunctionalUnit* fus, Instruction* instruction, Unit* result[NUM_OF_REGISTERS], int cc);

void readOperand(FunctionalUnit* fu, int cc);

void execution(FunctionalUnit* fu, int* mem, double* regs, int cc);

void executionInstruction(FunctionalUnit* fu, int* mem, double* regs, unsigned int instOp, int i, int j, int cc);

void writeResult(FunctionalUnit* fu, IntructionQueue* queue, Unit* result[NUM_OF_REGISTERS], double* regs, int cc, int numOfActiveUnits);

void writeResultFunctionalUnit(FunctionalUnit* fus, Unit* unit, Unit* result[NUM_OF_REGISTERS], double* regs, int cc, int numOfActiveUnits);

void checkIfLdAndStCollide(FunctionalUnit* fu, Instruction* stInst, int cc);

void updateAllFields(Unit* unit, int* regsStatus);
/*Free all the data that has been allocated for exit solution properly.*/
void freeSolution(FILE** fds, char* line, Config* cfg, Instruction* cmd, FunctionalUnit* fus, IntructionQueue* queue);