#define _GNU_SOURCE
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include "Config.h"

Config* createConfig() {
	Config* src = (Config*)malloc(sizeof(Config));
	if (!src) {
		return 0;
	}
	src->name = -1;
	return src;
}

void freeConfig(Config* cfg) {
	if (!cfg) {
		return;
	}
	free(cfg);
}

Config* parseConfig(FILE* cfgFd, char* line) {
	Config* cfg = createConfig();
	if (!cfg) {
		return 0;
	}
	size_t len = 0;
	ssize_t readLines = 0;
	int linesCounter = 0;
	
	while (fgets(line, MAX_LINE_LENGTH, cfgFd) != 0) {
		if (!parse(cfg, line)) {
			printf("Parser failed.\n");
			return 0;
		}
	}
	return cfg;
}

int parse(Config* cfg, char* line) {
	if (line == '\n') {
		return 1;
	}
	char* delimeter = " =\n\t";
	char* ptr = strtok(line, delimeter);
	if (!ptr) {
		return 1;
	}
	int param;
	char unitTraceName[MAX_LENGTH];
	for (int i = 0; i < NUM_OF_UNITS; i++) {
		if (strcmp(ptr, configUnitsTypes[i]) == 0) {
			if ((param = parseParam(ptr, i, delimeter)) == -1) {
				return 0;
			}
			cfg->units[i] = param;
			return 1;
		}
	}
	for (int i = 0; i < NUM_OF_UNITS; i++) {
		if (strcmp(ptr, configUnitsTypes[i+NUM_OF_UNITS]) == 0) {
			if ((param = parseParam(ptr, i, delimeter)) == -1) {
				return 0;
			}
			cfg->delays[i] = param;
			return 1;
		}
	}
	if (strcmp(ptr, configUnitsTypes[TRACE_UNIT]) == 0) {
		if ((param = parseTraceUnitParam(ptr, TRACE_UNIT, delimeter, unitTraceName)) == -1) {
			return 0;
		}
		if (unitTraceName) {
			char* del = "01234";
			cfg->unitNum = extractDigitFromStr(unitTraceName);
			char* ptr = strtok(unitTraceName, del);
			int name = unitTraceNameToInt(unitTraceName);
			if (name == -1) {
				printf("Error! cfg trace_unit_name invalide.\n");
				return 0;
			}
			cfg->name = name;
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

int extractDigitFromStr(char* str) {
	char* ptr = str;
	while (*ptr) {
		if (isdigit(*ptr)) {
			int res = atoi(ptr);
			return res;
		}
		else {
			ptr++;
		}
	}
	return -1;
}

int unitTraceNameToInt(char* str) {
	for (int i = 0; i < NUM_OF_UNITS; i++) {
		if (strcmp(str, unitsTypeNames[i]) == 0) {
			return i;
		}
	}
	return -1;
}