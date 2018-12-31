#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "FunctionalUnit.h"

/*Check Verification of files.*/
int verifyFiles(FILE** filesFd, char ** filesPaths);

/*Print memout.txt file.*/
void printMemoutFile(FILE* fd, int* memory, int maxLines);

/*Print regout.txt file.*/
void printRegoutFile(FILE* fd, double* regs);

/*Print traceinst.txt file.*/
void printTracinstFile(FILE* fd, Instruction* instruction, int type, int num);

/*Print traceunit.txt file.*/
void printTraceunitFile(FILE* fd, FunctionalUnit* fus, int* resultTypes, int* resultIndexes, int cc);

/*Command to Hexadecimal value.*/
int cmdToHex(Instruction* instruction);

float singlePrecisionToFloat(unsigned long singlePrecision);

int floatToSinglePrecision(float fl);

int isUnitsEqual(FunctionalUnit* fu, int q_type, int q_index, Unit* src2, int isJ);

void printUnit(Unit* src);

void cleanAndWriteToFiles(FILE* fd, FunctionalUnit* fus, InstructionQueue* queue);