#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Commons.h"

typedef intptr_t ssize_t;

/*Config struct, hold data from cfg.txt*/
typedef struct configParams {
	int units[6];
	int delays[6];
	char* unitName;
	int unitNum;
}Config;

typedef enum unitsEnum{
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

/*Create config object.*/
Config* createConfig();

/*Free config object.*/
void freeConfig(Config* cfg);

/*Main parser, iterate over config file.*/
Config* parseConfig(FILE* cfgFd, char* line);

/*Seperate config params by type and execute the right parser function.*/
int parse(Config* cfg, char* line);

/*Parameters parser, used voth for numOfUnit & delay params.*/
int parseParam(char* ptr, int paramType, char* delimeter);

/*Trace unit parameter parser.*/
int parseTraceUnitParam(char* ptr, int paramType, char* delimeter, char* unitTraceName);

/*Extract digit from string.*/
int extractDigitFromStr(char* str);