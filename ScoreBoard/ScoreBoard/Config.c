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
	if (!cfg) {
		return 0;
	}
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
	char* delimeter = " =\n";
	char* ptr = strtok(line, delimeter);
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
		if (strcmp(ptr, configUnitsTypes[i + NUM_OF_UNITS]) == 0) {
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

ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
	size_t pos;
	int c;

	if (lineptr == NULL || stream == NULL || n == NULL) {
		errno = EINVAL;
		return -1;
	}

	c = fgetc(stream);
	if (c == EOF) {
		return -1;
	}

	if (*lineptr == NULL) {
		*lineptr = malloc(128);
		if (*lineptr == NULL) {
			return -1;
		}
		*n = 128;
	}

	pos = 0;
	while (c != EOF) {
		if (pos + 1 >= *n) {
			size_t new_size = *n + (*n >> 2);
			if (new_size < 128) {
				new_size = 128;
			}
			char *new_ptr = realloc(*lineptr, new_size);
			if (new_ptr == NULL) {
				return -1;
			}
			*n = new_size;
			*lineptr = new_ptr;
		}

		((unsigned char *)(*lineptr))[pos++] = c;
		if (c == '\n') {
			break;
		}
		c = fgetc(stream);
	}

	(*lineptr)[pos] = '\0';
	return pos;
}