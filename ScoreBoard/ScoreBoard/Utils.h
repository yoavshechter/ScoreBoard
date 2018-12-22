#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include "Commons.h"
#include "Commands.h"

typedef intptr_t ssize_t;

extern char* filesNames[NUM_OF_FILES];

int verifyFiles(FILE** filesFd, char ** filesPaths);

double decToSinglePrecision(int dec);

void printMemoutFile(FILE* fd, int* memory, int maxLines);

void printRegoutFile(FILE* fd, int* regs);

void printTracinstFile(FILE* fd, Command* cmd, int pc, Unit* unit, int* cycles);

void printTraceunitFile(FILE* fd, int cycle, Unit* unit, int* regsIndices, Unit** qUnits, int* r);

int cmdToHex(Command* cmd);

ssize_t getline(char **lineptr, size_t *n, FILE *stream);