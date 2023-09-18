/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/


#include <SPI.h>
#include <SD.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "activations.h"
#include "bnn.h"
#include "matrix.h"
#include "ops.h"
#include "sram.h"
#include <Arduino.h>

#define IMAGE_SIZE 784
#define number_imgs 1 // number of images I want to train. 
extern "C" char* sbrk(int incr);

static const char train_images[] = "tr-img";
static const char train_labels[] = "tr-la";
static const char test_images[] = "te-img";
static const char test_labels[] = "te-la";
File train_image_file, train_label_file;
File test_image_file, test_label_file;

// Byte is a data type of 8-bit integers, 0-255
byte data;
byte image[IMAGE_SIZE];
byte label;

unsigned long read_4_bytes(File& a_file)
{
    unsigned long file_info = 0;
    for(byte i = 0; i < 4; i++){
      file_info = file_info << 8;
      file_info = file_info | (unsigned long)a_file.read();
    }
    return file_info;
}


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print(F("Initializing SD card..."));
  //pinMode(10, OUTPUT); // change this to 53 on a mega  // don't follow this!!
  //digitalWrite(10, HIGH);
  if (!SD.begin(10)) {
    Serial.println(F("initialization failed!"));
    while (1);
  }
  
  Serial.println(F("initialization done."));
  //Serial.println(availableMemory());

  //display_freeram();

  // open the file for reading:
  train_image_file = SD.open(train_images);
  train_label_file = SD.open(train_labels);
  // test_image_file = SD.open(test_images);
  // test_label_file = SD.open(test_labels);
  int i;
  if (train_image_file && train_label_file) {
    // read from the training dataset
    if (train_image_file.available() && 
      train_label_file.available()) {
        
        // http://yann.lecun.com/exdb/mnist/
        // First several bytes of each data file contains information of the dataset.
        // Read these bytes for test files as well.
        
        Serial.println("Train Image File Magic Number:  " + String(read_4_bytes(train_image_file)) + "  Should be:  2051"); 
        Serial.println("Train Image File number of images:  " + String(read_4_bytes(train_image_file)) + "  Should be:  60000");
        Serial.println("Train Image File number of rows:  " + String(read_4_bytes(train_image_file)) + "  Should be:  28");
        Serial.println("Train Image File number of columns:  " + String(read_4_bytes(train_image_file)) + "  Should be:  28");

        Serial.println("Train Label File Magic Number:  " + String(read_4_bytes(train_label_file)) + "  Should be:  2049");
        Serial.println("Train Label File number of items:  " + String(read_4_bytes(train_label_file)) + "  Should be:  60000");

        Serial.println(F("Hold for 10s..."));
        delay(10000);
        Serial.println(F("Waiting for 10s"));
        }
        // a will increment through the number of images I want to train. 

      byte img_label[number_imgs]; 
	    Matrix* img_data[number_imgs];
     int a = 0; 


    //while (train_image_file.available() && 
    // train_label_file.available()  && (a < number_imgs)) {
      //Serial.println(availableMemory());
      //display_freeram();
     // Serial.println(freeRam());

      img_data[a] = matrix_create(28, 28);
      //Serial.println(availableMemory());
      // read one image sample
      for (i = 0; i < IMAGE_SIZE; ++i) {
        data = train_image_file.read();
        image[i] = data;
        img_data[a]->entries[i/28][i%28] = data; 
      }
      //Serial.println(availableMemory());
      // read one label sample
      label = train_label_file.read();
      img_label[a] = label;

      Serial.println (F("before training "));
      a++;

      /* for (i = 0; i < IMAGE_SIZE; ++i) {
          Serial.print(image[i]);
          Serial.print(" ");
        }*/


      // Serial.println("after training ");
        // You can uncommented the code above to see the raw data 
        // Code below shows the image in terminal
        //delay(2000); // Delay for 2sec for viewing the image
      /* for (i = 0; i < 28; ++i) {
          for (int j = 0; j < 28; ++j) {
            if(image[i * 28 + j] > 0){
                Serial.print("\u25A0"); // Unicode Black Square
              }
            else{
                Serial.print("\u25A1"); // Unicode White Square
              }
          }
          Serial.println(" ");
        }
        Serial.println("========== LABEL OF ABOVE IMAGE IS:  " + String(label) + " ==========");
        */
   // }
      //display_freeram();
     // Serial.println(freeRam());
      NeuralNetwork* net = network_create(784, 300, 10, 0.1);
      Serial.println(F("nextline of training"));
	    network_train_batch_imgs(net, img_label, img_data, number_imgs);
      Serial.println(F("run the training batches"));
	    network_save(net, "testing_net");
      Serial.println(F("save network"));
    // close the file:
    train_image_file.close();
    train_label_file.close();
  } else {
    // if the file didn't open, print an error:
    Serial.print(F("error opening "));
    Serial.println(train_images);
  }
	/* NeuralNetwork* net = network_load("testing_net");
  Matrix* img =  matrix_create(28, 28);
    for (int s = 0; s < 28; ++s) {
      for (int m = 0; m < 28; ++m) {
        img->entries[s][m] = image[s * 28 + m];
      }
    }
	 float score = network_predict_imgs(net, img, label, 1000);
	 printf("Score: %1.5f\n", score);
	 network_free(net);
   */
}

void loop() {
  // nothing happens after setup
}

