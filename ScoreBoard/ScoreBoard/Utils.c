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
		fprintf(fd, "%.8x\n", memory[i]);
	}
}

void printRegoutFile(FILE* fd, double* regs) {
	int i;
	for (i = 0; i < NUM_OF_REGISTERS; i++) {
		fprintf(fd, "%f\n", regs[i]);
	}
}

void printTraceunitFile(FILE* fd, FunctionalUnit* fus, int* resultTypes, int* resultIndexes, int cc) {
	int unitBusy = No;
	for (int i = 0; i < NUM_OF_REGISTERS; i++) {
		if (resultTypes[i] == fus->unitName && resultIndexes[i] == fus->unitNum) {
			unitBusy = Yes;
		}
	}
	if (unitBusy) {
		fprintf(fd, "%d ", cc);
		fprintf(fd, "%s%d ", unitsTypeNames[fus->unitName], fus->unitNum);

		fprintf(fd, "F%d ", fus->fu[fus->unitName]->units[fus->unitNum]->f_i);
		fprintf(fd, "F%d ", fus->fu[fus->unitName]->units[fus->unitNum]->f_j);
		fprintf(fd, "F%d ", fus->fu[fus->unitName]->units[fus->unitNum]->f_k);

		int q_j = (fus->fu[fus->unitName]->units[fus->unitNum]->q_j_type == -1) ? 0 : 1;
		int q_k = (fus->fu[fus->unitName]->units[fus->unitNum]->q_k_type == -1) ? 0 : 1;

		if (q_j) {
			fprintf(fd, "%s%d ", unitsTypeNames[fus->fu[fus->unitName]->units[fus->unitNum]->q_j_type], fus->fu[fus->unitName]->units[fus->unitNum]->q_j_index);
		}
		else {
			fprintf(fd, "- ");
		}
		if (q_k) {
			fprintf(fd, "%s%d ", unitsTypeNames[fus->fu[fus->unitName]->units[fus->unitNum]->q_k_type], fus->fu[fus->unitName]->units[fus->unitNum]->q_k_index);
		}
		else {
			fprintf(fd, "- ");
		}

		fprintf(fd, (fus->fu[fus->unitName]->units[fus->unitNum]->r_j) ? "Yes " : "No ");
		fprintf(fd, (fus->fu[fus->unitName]->units[fus->unitNum]->r_k) ? "Yes\n" : "No\n");
	}
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

float singlePrecisionToFloat(unsigned long singlePrecision) {
	unsigned long sign, exp, fractionBits, fractionB;
	int i = 0;
	float res, fraction = 1.0;
	if (singlePrecision == 0) {
		return 0.0;
	}
	sign = singlePrecision >> 31;
	exp = singlePrecision << 1 >> 24;
	fractionBits = singlePrecision << 9 >> 9;
	for (i = 0; i < 23; i++) {
		fractionB = fractionBits << (i + 9);
		fractionB = fractionB >> 31;
		fraction += fractionB * powf(2, -1 * (i + 1));
	}
	res = powf(-1, sign) * powf(2, exp - 127) * fraction;

	return res;
}

int floatToSinglePrecision(float f) {
	int exp, fraction, expLenBits, i = 0, fractionToBit = 0;
	int floatInInt = (int)floor(f);
	int res;

	float rat = f - floatInInt;
	int sign = f > 0.0 ? 0 : 1;

	expLenBits = floor(log2(floatInInt));
	exp = expLenBits + 127;
	fraction = floatInInt << (23 - expLenBits);
	for (i = 0; i < (23 - expLenBits); i++) {
		rat = rat * 2;
		fractionToBit <<= 1;
		if (1 <= rat){
			fractionToBit++;
			rat--;
		}
	}
	fractionToBit >>= 1;
	fraction = fraction | fractionToBit;
	fraction = fraction & 0x7FFFFF;
	res = (((sign << 8) + exp) << 23) + fraction;

	return res;
}

int isUnitsEqual(FunctionalUnit* fu, int q_type, int q_index, Unit* src2, int isJ) {
	for (int i = 0; i < NUM_OF_UNITS; i++) {
		if (i == q_type) {
			for (int j = 0; j < fu->fu[i]->numOfTotalUnits; j++) {
				if (j == q_index) {
					if (fu->fu[i]->units[j] == src2) {
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

void printUnit(Unit* src) {
	int scenario = 0;
	if (src->q_j_index == -1 && src->q_k_index != -1) {
		scenario = 1;
	}
	if (src->q_k_index == -1 && src->q_j_index != -1) {
		scenario = 2;
	}
	if (src->q_j_index == -1 && src->q_k_index == -1) {
		scenario = 3;
	}
	if (src->q_j_index != -1 && src->q_k_index != -1) {
		scenario = 4;
	}
	switch (scenario) {
	case 1:
		printf("Unit: name=%s%d, busy=%d, f_i=%d, f_j=%d, f_k=%d, q_j=-, q_k=%s%d, r_j=%d, r_k=%d\n", unitsTypeNames[src->type], src->unitNum, src->busy, src->f_i, src->f_j, src->f_k,
			unitsTypeNames[src->q_k_index], src->q_k_index, src->r_j, src->r_k);
		break;
	case 2:
		printf("Unit: name=%s%d, busy=%d, f_i=%d, f_j=%d, f_k=%d, q_j=%s%d, q_k=-, r_j=%d, r_k=%d\n", unitsTypeNames[src->type], src->unitNum, src->busy, src->f_i, src->f_j, src->f_k,
			unitsTypeNames[src->q_j_index], src->q_j_index, src->r_j, src->r_k);
		break;
	case 3:
		printf("Unit: name=%s%d, busy=%d, f_i=%d, f_j=%d, f_k=%d, q_j=-, q_k=-, r_j=%d, r_k=%d\n", unitsTypeNames[src->type], src->unitNum, src->busy, src->f_i, src->f_j, src->f_k,
			src->r_j, src->r_k);
		break;
	case 4:
		printf("Unit: name=%s%d, busy=%d, f_i=%d, f_j=%d, f_k=%d, q_j=%s%d, q_k=%s%d, r_j=%d, r_k=%d\n", unitsTypeNames[src->type], src->unitNum, src->busy, src->f_i, src->f_j, src->f_k,
			unitsTypeNames[src->q_j_type], src->q_j_index, unitsTypeNames[src->q_k_index], src->q_k_index, src->r_j, src->r_k);
		break;
	}
	
}

void cleanAndWriteToFiles(FILE* fd, FunctionalUnit* fus, InstructionQueue* queue) {
	for (int i = 0; i < NUM_OF_INSTRUCTION_IN_QUEUE; i++) {
		if (!queue->queue[i]->isEmpty && queue->queue[i]->stateCC[WRITE_RESILT] > 0) {
			//scanf("%d", &queue->queue[i]->fetchedTime);
			int type = queue->queue[i]->instType, index = queue->queue[i]->instIndex;
			insertFirst(fus->fu[type]->units[index]);
			//freeUnit(fus->fu[type]->units[index]);
			//removeInstructionFromInstructionQueue(queue, i);
			fus->fu[type]->units[index]->instruction->isEmpty = Yes;
			fus->fu[type]->units[index] = createUnit(type, index);
			fus->fu[type]->units[index]->isEmpty = Yes;
			fus->fu[type]->numOfActiveUnits--;
		}
	}
}

void printUnitsToTraceInstFile(FILE* fd) {
	sort();
	int listLength = length();
	for (int i = 0; i < listLength; i++) {
		printTracinstFile(fd, head->op, head->unitType, head->unitIndex, head->fetchCC, head->issueCC, head->readCC, head->exeCC, head->writeCC);
		deleteFirst();
	}
}

void printTracinstFile(FILE* fd, unsigned int opLine, int type, int index, int fetch, int issue, int read, int exe, int write) {
	fseek(fd, 0, SEEK_END);
	int i = 0;
	fprintf(fd, "%.8x ", opLine);
	fprintf(fd, "%d ", fetch);
	fprintf(fd, "%s%d", unitsTypeNames[type], index);
	fprintf(fd, " %d", issue);
	fprintf(fd, " %d", read);
	fprintf(fd, " %d", exe);
	fprintf(fd, " %d", write);
	fprintf(fd, "\n");
}

PrintUnit* createEmptyPrintUnit() {
	PrintUnit* src = (PrintUnit*)malloc(sizeof(PrintUnit));
	if (!src) {
		return;
	}
	src->op = -1;
	src->unitType = -1;
	src->unitIndex = -1;
	src->fetchCC = -1;
	src->issueCC = -1;
	src->readCC = -1;
	src->exeCC = -1;
	src->writeCC = -1;
	return src;
}

void insertFirst(Unit* unitSrc) {
	//create a link
	PrintUnit* src = (PrintUnit*)malloc(sizeof(PrintUnit));
	if (!src) {
		return;
	}
	src->op = unitSrc->instruction->line;
	src->unitType = unitSrc->type;
	src->unitIndex = unitSrc->unitNum;
	src->fetchCC = unitSrc->instruction->fetchedTime;
	src->issueCC = unitSrc->instruction->stateCC[ISSUE];
	src->readCC = unitSrc->instruction->stateCC[READ_OPERAND];
	src->exeCC = unitSrc->instruction->stateCC[EXECUTION];
	src->writeCC = unitSrc->instruction->stateCC[WRITE_RESILT];

	//point it to old first node
	src->next = head;

	//point first to new first node
	head = src;
}

PrintUnit* deleteFirst() {

	//save reference to first link
	PrintUnit *tempLink = head;

	//mark next to first link as first 
	head = head->next;

	//return the deleted link
	return tempLink;
}

int isEmpty() {
	return head == NULL;
}

int length() {
	int length = 0;
	PrintUnit *current;

	for (current = head; current != NULL; current = current->next) {
		length++;
	}

	return length;
}

void sort() {

	int i, j, k, tempKey;
	PrintUnit *current;
	PrintUnit *next;
	PrintUnit *temp = createEmptyPrintUnit();

	int size = length();
	k = size;

	for (i = 0; i < size - 1; i++, k--) {
		current = head;
		next = head->next;

		for (j = 1; j < k; j++) {

			if (current->fetchCC > next->fetchCC) {
				setData(current, temp);
				setData(next, current);
				setData(temp, next);
			}

			current = current->next;
			next = next->next;
		}
	}
}

void setData(PrintUnit* unitSrc, PrintUnit* dst) {
	dst->op = unitSrc->op;
	dst->unitType = unitSrc->unitType;
	dst->unitIndex = unitSrc->unitIndex;
	dst->fetchCC = unitSrc->fetchCC;
	dst->issueCC = unitSrc->issueCC;
	dst->readCC = unitSrc->readCC;
	dst->exeCC = unitSrc->exeCC;
	dst->writeCC = unitSrc->writeCC;

}