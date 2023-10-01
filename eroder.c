// eroder.c
#include <stdlib.h>
#include <stdio.h>
#include "eroder.h"

// Helper function to erosion used to determine whether pixel should erode
int shouldErode(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], int x, int y) {
    // Plus sign
    if (temp_image[x][y+1] == 0) return 1;
    if (temp_image[x][y-1] == 0) return 1;
    if (temp_image[x-1][y] == 0) return 1;
    if (temp_image[x+1][y] == 0) return 1;

    // X sign
    if (temp_image[x+1][y+1] == 0) return 1;
    if (temp_image[x+1][y-1] == 0) return 1;
    if (temp_image[x-1][y+1] == 0) return 1;
    if (temp_image[x-1][y-1] == 0) return 1;
    return 0;
}

// Performs the erosion algorithm which erodes pixels at the edge of a cell
void erosion(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], unsigned int *pixelsLeft) {
    unsigned char eroded[BMP_WIDTH][BMP_HEIGTH];
    *pixelsLeft = 0;

    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
            if(x == 0 || x == BMP_WIDTH-1 || y == 0 || y == BMP_HEIGTH-1) {
                eroded[x][y] = 0;
            } else {
                if(temp_image[x][y] == 1 && !shouldErode(temp_image,x,y)) {
                    *pixelsLeft += 1;
                    eroded[x][y] = 1;
                } else {
                    eroded[x][y] = 0;
                }
            }
        }
    }

    for(int x = 0; x < BMP_WIDTH; x++) {
        for(int y = 0; y < BMP_HEIGTH; y++) {
            temp_image[x][y] = eroded[x][y];
        }
    }
}