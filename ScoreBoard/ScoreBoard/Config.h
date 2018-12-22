#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Commons.h"

typedef intptr_t ssize_t;

typedef struct configParams {
	int units[6];
	int delays[6];
	char name[MAX_LENGTH];
}Config;

enum units{
	ADD_UNIT = 0,
	SUB_UNIT,
	MULT_UNIT,
	DIV_UNIT,
	LD_UNIT,
	ST_UNIT,
	ADD_DELAY,
	SUB_DELAY,
	MULT_DELAY,
	DIV_DELAY,
	LD_DELAY,
	ST_DELAY,
	TRACE_UNIT,
}configUnits;

static char* configUnitsTypes[13] = { "add_nr_units", "sub_nr_units", "mul_nr_units", "div_nr_units", "ld_nr_units", "st_nr_units",
"add_delay" , "sub_delay", "mul_delay", "div_delay", "ld_delay", "st_delay", "trace_unit" };

Config* createConfig();

void freeConfig(Config* cfg);

Config* parseConfig(FILE* cfgFd, char* line);

int parse(Config* cfg, char* line);

int parseParam(char* ptr, int paramType, char* delimeter);

int parseTraceUnitParam(char* ptr, int paramType, char* delimeter, char* unitTraceName);