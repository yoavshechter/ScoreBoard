#include "Simulator.h"

int simulator(char** filesPaths) {
	FILE* filesFd[NUM_OF_FILES];
	if (!verifyFiles(filesFd, filesPaths)) {
		return 0;
	}

	return 0;
}