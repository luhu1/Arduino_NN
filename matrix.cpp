#include <Arduino.h>
#include "matrix.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXCHAR 100

Matrix* matrix_create(int row, int col) {
	Matrix *matrix = (Matrix*) malloc(sizeof(Matrix));
	matrix->rows = row;
	matrix->cols = col;
	matrix->entries =  (double**) malloc(row * sizeof(double*));
	for (int i = 0; i < row; i++) {
		matrix->entries[i] = (double*) malloc(col * sizeof(double));
	}
	return matrix;
}

void matrix_fill(Matrix *m, int n) {
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			m->entries[i][j] = n;
		}
	}
}

void matrix_free(Matrix *m) {
	for (int i = 0; i < m->rows; i++) {
		free(m->entries[i]);
	}
	free(m->entries);
	free(m);
	m = NULL;
}

void matrix_print(Matrix* m) {
	Serial.print("Rows: ");
  Serial.println(m->rows);
  Serial.print("Columns: ");
  Serial.println( m->cols);
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			Serial.print("%1.3f ");
      Serial.println(m->entries[i][j]);
		}
		Serial.println("\n");
	}
}

Matrix* matrix_copy(Matrix* m) {
	Matrix* mat = matrix_create(m->rows, m->cols);
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			mat->entries[i][j] = m->entries[i][j];
		}
	}
	return mat;
}

void matrix_save(Matrix* m, char* file_string) {
	FILE* file = fopen(file_string, "w");
	fprintf(file, "%d\n", m->rows);
	fprintf(file, "%d\n", m->cols);
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			fprintf(file, "%.6f\n", m->entries[i][j]);
		}
	}
	printf("Successfully saved matrix to %s\n", file_string);
	fclose(file);
}

Matrix* matrix_load(char* file_string) {
	FILE* file = fopen(file_string, "r");
	char entry[MAXCHAR]; 
	fgets(entry, MAXCHAR, file);
	int rows = atoi(entry);
	fgets(entry, MAXCHAR, file);
	int cols = atoi(entry);
	Matrix* m = matrix_create(rows, cols);
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			fgets(entry, MAXCHAR, file);
			m->entries[i][j] = strtod(entry, NULL);
		}
	}
	printf("Sucessfully loaded matrix from %s\n", file_string);
	fclose(file);
	return m;
}

double uniform_distribution(double low, double high) {
	double difference = high - low; // The difference between the two
	int scale = 10000;
	int scaled_difference = (int)(difference * scale);
	return low + (1.0 * (rand() % scaled_difference) / scale);
}

void matrix_randomize(Matrix* m, int n) {
	// Pulling from a random distribution of 
	// Min: -1 / sqrt(n)
	// Max: 1 / sqrt(n)
	double min = -1.0 / sqrt(n);
	double max = 1.0 / sqrt(n);
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			m->entries[i][j] = uniform_distribution(min, max);
		}
	}
}

int matrix_argmax(Matrix* m) {
	// Expects a Mx1 matrix
	double max_score = 0;
	int max_idx = 0;
	for (int i = 0; i < m->rows; i++) {
		if (m->entries[i][0] > max_score) {
			max_score = m->entries[i][0];
			max_idx = i;
		}
	}
	return max_idx;
}
// why am I getting this wrong. It seems like the rows/ columns are switched but it doesnt look that way tho.
Matrix* matrix_flatten(Matrix* m, int axis) {
	// Axis = 0 -> Column Vector, Axis = 1 -> Row Vector
  /*
	Matrix* mat;
	if (axis == 0) {
		mat = matrix_create(m->rows * m->cols, 1);
	} else if (axis == 1) {
		mat = matrix_create(1, m->rows * m->cols);
	} else {
		printf("Argument to matrix_flatten must be 0 or 1");
		exit(EXIT_FAILURE);
	}
  Serial.println("inside flattening"); 
  Serial.print("row is: " );
  Serial.print(m->rows);
  Serial.print("cols is : ");
  Serial.println(m->cols);
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
      Serial.println("inside for loop");
      Serial.println( axis);
      Serial.println(m->entries[i][j]);
			if (axis == 0){ 
        mat->entries[i * m->cols  + j][0] = m->entries[i][j];
        Serial.println("finish if statement");
      }
			else if (axis == 1) mat->entries[0][i * m->cols + j] = m->entries[i][j];
      Serial.println(" after if/else statement"); 
		}
	}
  Serial.println("done with flattening");
	return mat;
  */
  	Matrix* mat;
	if (axis == 0) {
    Serial.println( "mat is at 0 "); 
		mat = matrix_create(m->rows * m->cols, 1);
	} else if (axis == 1) {
    Serial.println("mat is at 1");
		mat = matrix_create(1, m->rows * m->cols);
	} else {
		Serial.println("Argument to matrix_flatten must be 0 or 1");
		exit(EXIT_FAILURE);
	}
  
  Serial.println("Inside flattening");

	int index = 0;  // Index for the flattened matrix

	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
      //Serial.println("inside the nested for loop");
			if (axis == 0) {
				mat->entries[0][index] = m->entries[i][j];
			} else if (axis == 1) {
				mat->entries[index][0] = m->entries[i][j];
			}
			index++; // Increment the index for the flattened matrix
		}
	}

	Serial.println("Done with flattening");
	return mat;
}
