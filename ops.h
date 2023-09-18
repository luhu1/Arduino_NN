#pragma once

#include "matrix.h"

Matrix* multiply(Matrix* m1, Matrix* m2);
Matrix* add(Matrix* m1, Matrix* m2);
Matrix* subtract(Matrix* m1, Matrix* m2);
Matrix* dot(Matrix* m1, Matrix* m2);
Matrix* apply(float (*func)(float), Matrix* m);
Matrix* scale(float n, Matrix* m);
Matrix* addScalar(float n, Matrix* m);
Matrix* transpose(Matrix* m);
