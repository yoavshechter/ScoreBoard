#include "Utils.h"

void printMemoutFile(FILE* fd, int* memory, int maxLines) {
	for (int i = 0; i < maxLines; i++) {
		fprintf(fd, "%d.%.8x\n", i, memory[i]);
	}
}

void printRegoutFile(FILE* fd, int* regs) {
	int i;
	for (i = 0; i < MAX_REGISTERS; i++) {
		fprintf(fd, "%d/.", i);
		fprintf(fd, "%.8x", regs[i]);
	}
}

void printTracinstFile(FILE* fd, Command* cmd, int pc, Unit* unit, int* cycles) {
	int i = 0;
	fprintf(fd, "%.8x ", cmd);
	fprintf(fd, "%d ", pc);
	fprintf(fd, "%s ", unit->unitName);
	for (i = 0; i < NUM_OF_CYCLES_TYPES - 1; i++) {
		fprintf(fd, "%d ", cycles[i]);
	}
	fprintf(fd, "%d\n", cycles[NUM_OF_CYCLES_TYPES - 1]);
}

void printTraceunitFile(FILE* fd, int cycle, Unit* unit, int* regsIndices, Unit** qUnits, int* r) {
	int i = 0;
	fprintf(fd, "%d ", cycle);
	fprintf(fd, "%s ", unit->unitName);
	for (i = 0; i < 3; i++) {
		fprintf(fd, "F%d ", regsIndices[i]);
	}
	for (i = 0; i < 2; i++) {
		if (!qUnits[i]) {
			fprintf(fd, "%s ", qUnits[i]->unitName);
		}
		else {
			fprintf(fd, "- ");
		}
	}
	fprintf(fd, (r[0]) ? "Yes " : "No ");
	fprintf(fd, (r[1]) ? "Yes" : "No");
}