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
void printTracinstFile(FILE* fd, Instruction* instruction, int pc, Unit* unit, int* cycles);

/*Print traceunit.txt file.*/
void printTraceunitFile(FILE* fd, int cycle, Unit* unit, int* regsIndices, Unit** qUnits, int* r);

/*Command to Hexadecimal value.*/
int cmdToHex(Instruction* instruction);

float singlePrecisionToFloat(unsigned long sp);

int floatToSinglePrecision(float f);

int isUnitsEqual(Unit* src1, Unit* src2);