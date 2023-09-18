#include "img.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 10000

void  csv_to_imgs(char* file_string, int number_of_imgs, char label[], Matrix* data[]) {
	FILE *fp;
	//Img** imgs[5000 * sizeof(Img*)];
	char row[MAXCHAR];
	fp = fopen(file_string, "r");

	// Read the first line 
	fgets(row, MAXCHAR, fp);
	int i = 0;
	while (feof(fp) != 1 && i < number_of_imgs) {

		int j = 0;
		fgets(row, MAXCHAR, fp);
		char* token = strtok(row, ",");
		data[i] = matrix_create(28, 28);
		while (token != NULL) {
			if (j == 0) {
				label[i] = atoi(token);
			} else {
				data[i]->entries[(j-1) / 28][(j-1) % 28] = atoi(token) / 256.0;
			}
			token = strtok(NULL, ",");
			j++;
		}
		i++;
	}
	fclose(fp);
}

void img_print(Img* img) {
	matrix_print(img->img_data);
	printf("Img Label: %d\n", img->label);
}
/*
void img_free(Img* img) {
	matrix_free(img->img_data);
	free(img);
	img = NULL;
}

void imgs_free(Img** imgs, int n) {
	for (int i = 0; i < n; i++) {
		img_free(imgs[i]);
	}
}
*/