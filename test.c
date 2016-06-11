#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
	int i;
	for (i=0;i<5;i++) {
			char filename[10];
			char stringI[2];
			snprintf(stringI, 2, "%d", i);
			strcpy(filename, "img0");
			strcat(filename,stringI);
			strcat(filename, ".bmp");
			printf("'%s'\n", filename);
	}
	return 0;
}