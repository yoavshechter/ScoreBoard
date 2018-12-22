#include <stdio.h>
#include <string.h>
#include "Simulator.h"
#include "Commons.h"

int main(int argc, char **argv) {
	if (argc != NUM_OF_FILES + 1) {
		printf("Invalid number of arguments.\n");
		return 0;
	}
	char* filesPaths[NUM_OF_FILES + 1] = { argv[1], argv[2], argv[3], argv[4], argv[5], argv[6] };
	return simulator(filesPaths);
}