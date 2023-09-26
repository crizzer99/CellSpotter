// spotCells.h
// header for spotCells.c
#ifndef SPOTCELLS_H_
#include "cbmp.h"

#define SPOTCELLS_H_

typedef struct{
    unsigned int x;
    unsigned int y;
} coordinates_t;


void checkImage(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], coordinates_t cellCenters[301], unsigned int *cellCount);
void markCells(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], coordinates_t cellCenters[301], unsigned int cellCount);


#endif /* SPOTCELLS_H_ */