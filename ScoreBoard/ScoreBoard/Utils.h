#pragma once
#include <stdio.h>
#include <string.h>
#include "Commons.h"


void printMemoutFile(FILE* fd, int* memory, int maxLines);

void printRegoutFile(FILE* fd, int* regs);

void printTracinstFile(FILE* fd, Command* cmd, int pc, Unit* unit, int* cycles);

void printTraceunitFile(FILE* fd, int cycle, Unit* unit, int* regsIndices, Unit** qUnits, int* r);