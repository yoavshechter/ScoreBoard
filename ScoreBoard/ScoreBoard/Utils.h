#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include "Commands.h"
#include "Config.h"
#include "FunctionalUnit.h"


extern char* filesNames[NUM_OF_FILES];

/*Check Verification of files.*/
int verifyFiles(FILE** filesFd, char ** filesPaths);

/*Decimal to Single Precision.*/
double decToSinglePrecision(int dec);

/*Print memout.txt file.*/
void printMemoutFile(FILE* fd, int* memory, int maxLines);

/*Print regout.txt file.*/
void printRegoutFile(FILE* fd, double* regs);

/*Print traceinst.txt file.*/
void printTracinstFile(FILE* fd, Command* cmd, int pc, Unit* unit, int* cycles);

/*Print traceunit.txt file.*/
void printTraceunitFile(FILE* fd, int cycle, Unit* unit, int* regsIndices, Unit** qUnits, int* r);

/*Command to Hexadecimal value.*/
int cmdToHex(Command* cmd);