#define _GNU_SOURCE
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include "Config.h"

Config* createConfig() {
	Config* cfg = (Config*)malloc(sizeof(Config));
	if (!cfg) {
		return 0;
	}
	return cfg;
}

void cleanConfig(Config* cfg) {
	if (!cfg) {
		return;
	}
	free(cfg);
}

Config* parseConfig(FILE* cfgFd) {
	Config* cfg = createConfig();
	char* line = 0;
	size_t len = 0;
	ssize_t readLines = 0;
	int linesCounter = 0;
	
	while ((readLines = getline(&line, &len, cfgFd)) != -1) {
		if (!parse(cfg, line)) {
			printf("Parser failed.\n");
			return 0;
		}
	}
	return cfg;
}

int parse(Config* cfg, char* line) {
	char* delimeter = "=\n";
	char* ptr = strtok(line, delimeter);
	int param;
	char* unitTraceName = NULL;
	if (strcmp(ptr, configUnitsTypes[ADD_UNIT]) == 0) {
		if ((param = parseParam(ptr, ADD_UNIT, delimeter)) == -1) {
			return 0;
		}
		cfg->units[ADD_UNIT] = param;
	}
	else if (strcmp(ptr, configUnitsTypes[SUB_UNIT]) == 0) {
		if ((param = parseParam(ptr, SUB_UNIT, delimeter)) == -1) {
			return 0;
		}
		cfg->units[SUB_UNIT] = param;
	}
	else if (strcmp(ptr, configUnitsTypes[MULT_UNIT]) == 0) {
		if ((param = parseParam(ptr, MULT_UNIT, delimeter)) == -1) {
			return 0;
		}
		cfg->units[MULT_UNIT] = param;
	}
	else if (strcmp(ptr, configUnitsTypes[DIV_UNIT]) == 0) {
		if ((param = parseParam(ptr, DIV_UNIT, delimeter)) == -1) {
			return 0;
		}
		cfg->units[DIV_UNIT] = param;
	}
	else if (strcmp(ptr, configUnitsTypes[LD_UNIT]) == 0) {
		if ((param = parseParam(ptr, LD_UNIT, delimeter)) == -1) {
			return 0;
		}
		cfg->units[LD_UNIT] = param;
	}
	else if (strcmp(ptr, configUnitsTypes[ST_UNIT]) == 0) {
		if ((param = parseParam(ptr, ST_UNIT, delimeter)) == -1) {
			return 0;
		}
		cfg->units[ST_UNIT] = param;
	}
	else if (strcmp(ptr, configUnitsTypes[ADD_DELAY]) == 0) {
		if ((param = parseParam(ptr, ADD_DELAY, delimeter)) == -1) {
			return 0;
		}
		cfg->delays[ADD_DELAY - NUM_OF_UNITS] = param;
	}
	else if (strcmp(ptr, configUnitsTypes[SUB_DELAY]) == 0) {
		if ((param = parseParam(ptr, SUB_DELAY, delimeter)) == -1) {
			return 0;
		}
		cfg->delays[SUB_DELAY - NUM_OF_UNITS] = param;
	}
	else if (strcmp(ptr, configUnitsTypes[MULT_DELAY]) == 0) {
		if ((param = parseParam(ptr, MULT_DELAY, delimeter)) == -1) {
			return 0;
		}
		cfg->delays[MULT_DELAY - NUM_OF_UNITS] = param;
	}
	else if (strcmp(ptr, configUnitsTypes[DIV_DELAY]) == 0) {
		if ((param = parseParam(ptr, DIV_DELAY, delimeter)) == -1) {
			return 0;
		}
		cfg->delays[DIV_DELAY - NUM_OF_UNITS] = param;
	}
	else if (strcmp(ptr, configUnitsTypes[LD_DELAY]) == 0) {
		if ((param = parseParam(ptr, LD_DELAY, delimeter)) == -1) {
			return 0;
		}
		cfg->delays[LD_DELAY - NUM_OF_UNITS] = param;
	}
	else if (strcmp(ptr, configUnitsTypes[ST_DELAY]) == 0) {
		if ((param = parseParam(ptr, ST_DELAY, delimeter)) == -1) {
			return 0;
		}
		cfg->delays[ST_DELAY - NUM_OF_UNITS] = param;
	}
	else if (strcmp(ptr, configUnitsTypes[TRACE_UNIT]) == 0) {
		if ((param = parseTraceUnitParam(ptr, ST_DELAY, delimeter, unitTraceName)) == -1) {
			return 0;
		}
		if (unitTraceName) {
			strcpy(cfg->name, unitTraceName);
		}
	}
	return 1;
}

int parseParam(char* ptr, int paramType, char* delimeter) {
	ptr = strtok(NULL, delimeter);
	int num = atoi(ptr);
	return num;
}

int parseTraceUnitParam(char* ptr, int paramType, char* delimeter, char* unitTraceName){
	ptr = strtok(NULL, delimeter);
	if(!ptr){
		printf("Couldnt parse traceunit name.\n");
		return 0;
	}
	strcpy(unitTraceName, ptr);
	return 1;
}
