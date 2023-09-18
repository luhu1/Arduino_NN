#pragma once

#include "matrix.h"
#include "Arduino.h"

typedef struct {
	int input;
	int hidden;
	int output;
	float learning_rate;
	Matrix* hidden_weights;
	Matrix* output_weights;
} NeuralNetwork;

NeuralNetwork* network_create(int input, int hidden, int output, float lr);
void network_train(NeuralNetwork* net, Matrix* input_data, Matrix* output_data);
void network_train_batch_imgs(NeuralNetwork* net, byte label[], Matrix* data[], int batch_size);
Matrix* network_predict_img(NeuralNetwork* net, Matrix* data);
float network_predict_imgs(NeuralNetwork* net, byte label[], Matrix* data[], int n);
Matrix* network_predict(NeuralNetwork* net, Matrix* input_data);
void network_save(NeuralNetwork* net, char* file_string);
NeuralNetwork* network_load(char* file_string);
void network_print(NeuralNetwork* net);
void network_free(NeuralNetwork* net);
