#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Commons.h"

typedef intptr_t ssize_t;

/*Config struct, hold data from cfg.txt*/
typedef struct configParams {
	int units[6];
	int delays[6];
	int name;
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

int unitTraceNameToInt(char* str);