// otsusMethod.h
// header for otsusMethod.c
#ifndef OTSUSMETHOD_H_
#include "cbmp.h"

#define OTSUSMETHOD_H_

float otsu(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], int startX, int startY, int resolution);

#endif /* OTSUSMETHOD_H_ */