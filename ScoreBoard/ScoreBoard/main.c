#include <stdio.h>
#include <string.h>
#include "Simulator.h"

int main(int argc, char **argv) {
	if (argc != 7) {
		printf("Invalid number of arguments.\n");
		return 0;
	}
	return simulator(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
}