// binary.h
// header for binary.c
#ifndef BINARY_H_
#include "cbmp.h"
#include "otsusMethod.h"

// Bit fields inspiration from https://en.wikipedia.org/wiki/Bit_field

#define BINARY_H_

void loadParts(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], int startX, int startY, int resolution, int thresh);
void toBmp(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], char * output_file_path);
void binary(char * input_file_path, unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH]);
// void greyToBmp(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]);

#endif /* BINARY_H_ */