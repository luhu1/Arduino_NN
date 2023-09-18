#pragma once

#include "matrix.h"

float sigmoid(float input);
Matrix* sigmoidPrime(Matrix* m);
Matrix* softmax(Matrix* m);
