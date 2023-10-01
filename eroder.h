// eroder.h
// header for eroder.c
#ifndef ERODER_H_
#include "cbmp.h"

#define ERODER_H_

int shouldErode(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], int x, int y);
void erosion(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], unsigned int *pixelsLeft);

#endif /* ERODER_H_ */