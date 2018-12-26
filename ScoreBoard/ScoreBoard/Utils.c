#include "Utils.h"


int verifyFiles(FILE** filesFd, char ** filesPaths) {
	for (int i = 0; i < NUM_OF_FILES; i++) {
		FILE* fd;
		if (i > 1) {
			fd = fopen(filesPaths[i], "w");
		}
		else {
			fd = fopen(filesPaths[i], "r");
		}
		if (!fd) {
			printf("Error! coulsnt open %s/.txt file.\n", filesNames[i]);
			for (int j = 0; j < i; j++) {
				fclose(filesFd[j]);
			}
			return 0;
		}
		filesFd[i] = fd;
	}
	return 1;
}

void printMemoutFile(FILE* fd, int* memory, int maxLines) {
	for (int i = 0; i < maxLines; i++) {
		fprintf(fd, "%d.%.8x\n", i, memory[i]);
	}
}

void printRegoutFile(FILE* fd, double* regs) {
	int i;
	for (i = 0; i < NUM_OF_REGISTERS; i++) {
		fprintf(fd, "%f", regs[i]);
	}
}

void printTracinstFile(FILE* fd, Instruction* instruction, int pc, Unit* unit, int* cycles) {
	int i = 0;
	fprintf(fd, "%.8x ", cmdToHex(instruction));
	fprintf(fd, "%d ", pc);
	fprintf(fd, "%s ", unitsTypeNames[unit->type]);
	for (i = 0; i < NUM_OF_CYCLES_TYPES - 1; i++) {
		fprintf(fd, "%d ", cycles[i]);
	}
	fprintf(fd, "%d\n", cycles[NUM_OF_CYCLES_TYPES - 1]);
}

void printTraceunitFile(FILE* fd, int cycle, Unit* unit, int* regsIndices, Unit** qUnits, int* r) {
	int i = 0;
	fprintf(fd, "%d ", cycle);
	fprintf(fd, "%s ", unitsTypeNames[unit->type]);
	for (i = 0; i < 3; i++) {
		fprintf(fd, "F%d ", regsIndices[i]);
	}
	for (i = 0; i < 2; i++) {
		if (!qUnits[i]) {
			fprintf(fd, "%s ", unitsTypeNames[qUnits[i]->type]);
		}
		else {
			fprintf(fd, "- ");
		}
	}
	fprintf(fd, (r[0]) ? "Yes " : "No ");
	fprintf(fd, (r[1]) ? "Yes" : "No");
}

int cmdToHex(Instruction* instruction) {
	unsigned int hex = 0;
	hex += instruction->opcode << 24;
	hex += instruction->regDst << 20;
	hex += instruction->regSrc0 << 16;
	hex += instruction->regSrc1 << 12;
	hex += 0xFFFFF & instruction->immidiate;
	return hex;
}

float singlePrecisionToFloat(unsigned long sp) {
	unsigned long sign, exp, fra_bits, fra_b;
	int i = 0;
	float res, fra = 1.0;
	if (sp == 0) {
		return 0.0;
	}
	sign = sp >> 31;
	exp = sp << 1 >> 24;
	fra_bits = sp << 9 >> 9;
	for (i = 0; i < 23; i++) {
		fra_b = fra_bits << (i + 9);
		fra_b = fra_b >> 31;
		fra += fra_b * powf(2, -1 * (i + 1));
	}
	res = powf(-1, sign) * powf(2, exp - 127) * fra;

	return res;
}

int floatToSinglePrecision(float f) {
	int sign, exp, fra, exp_bit_len, i = 0, fra_to_bit = 0;
	int integer = (int)floor(f);
	int res;
	float rat = f - integer;
	sign = f > 0.0 ? 0 : 1;
	exp_bit_len = floor(log2(integer));
	exp = exp_bit_len + 127;
	fra = integer << (23 - exp_bit_len);
	for (i = 0; i < (23 - exp_bit_len); i++) {
		rat = rat * 2;
		if (1 > rat) {
			fra_to_bit <<= 1;
			printf("0");
		}
		else {
			fra_to_bit += 1;
			fra_to_bit <<= 1;
			rat -= 1;
			printf("1");
		}
	}
	fra_to_bit >>= 1;
	fra = fra | fra_to_bit;
	fra = fra & 0x7FFFFF;
	res = (((sign << 8) + exp) << 23) + fra;

	return res;
}

int isUnitsEqual(Unit* src1, Unit* src2) {
	if (src1->type == src2->type && src1->unitNum == src2->unitNum) {
		return 1;
	}
	return 0;
}