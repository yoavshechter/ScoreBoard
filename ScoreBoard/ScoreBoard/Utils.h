#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "FunctionalUnit.h"

typedef struct unitForPrint {
	unsigned int op;
	int unitType;
	int unitIndex;
	int fetchCC;
	int issueCC;
	int readCC;
	int exeCC;
	int writeCC;
	struct unitToPrint* next;
}PrintUnit;

static PrintUnit *head = NULL;
static PrintUnit *current = NULL;

/*Check Verification of files.*/
int verifyFiles(FILE** filesFd, char ** filesPaths);

/*Print memout.txt file.*/
void printMemoutFile(FILE* fd, int* memory, int maxLines);

/*Print regout.txt file.*/
void printRegoutFile(FILE* fd, double* regs);

/*Print traceunit.txt file.*/
void printTraceunitFile(FILE* fd, FunctionalUnit* fus, int* resultTypes, int* resultIndexes, int cc);

/*Command to Hexadecimal value.*/
int cmdToHex(Instruction* instruction);

/*Convert single precision to float.*/
float singlePrecisionToFloat(unsigned long singlePrecision);

/*Convert float to single precision.*/
int floatToSinglePrecision(float fl);

/*Checks if units are equal.*/
int isUnitsEqual(FunctionalUnit* fu, int q_type, int q_index, Unit* src2, int isJ);

/*Print unit arguments to console.*/
void printUnit(Unit* src);

/*Clean functional units after write result.*/
void cleanAndWriteToFiles(FILE* fd, FunctionalUnit* fus, InstructionQueue* queue);

/*Print trace of instructions to TraceInst file.*/
void printUnitsToTraceInstFile(FILE* fd);
 
/*Print inst line to TraceInst file.*/
void printTracinstFile(FILE* fd, unsigned int opLine, int type, int index, int fetch, int issue, int read, int exe, int write);

/*Create empty unit using in print to TraceInst file.*/
PrintUnit* createEmptyPrintUnit();

/*Insert new unit to linked list head.*/
void insertFirst(Unit* unitSrc);

/*Delete first element of the linked list.*/
PrintUnit* deleteFirst();

/*Return the length of the linked list.*/
int length();

/*Sort the linked list by fetch cc.*/
void sort();

/*Set data from src printUnit to dst printUnit*/
void setData(PrintUnit* unitSrc, PrintUnit* dst);