// spotCells.c
#include <stdlib.h>
#include <stdio.h>
// #include <time.h>
#include "spotCells.h"

#define CAPTURE_WINDOW_SIZE 16

// Looks through the binary image to see spot cells
void checkImage(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], coordinates_t cellCenters[301], unsigned int *cellCount) {
    for(int x = 0; x < BMP_WIDTH; x++) {
        for(int y = 0; y < BMP_HEIGTH; y++) {
            if(temp_image[x][y] == 1) {
                // int captureWindow = 16;
                unsigned char captureWindow = CAPTURE_WINDOW_SIZE / 2;
                int edge = 1;
                for(int i = -captureWindow; i <= captureWindow+1; i++) {  
                  if(temp_image[x+i][y-captureWindow] == 1 || temp_image[x+i][y+captureWindow+1] == 1 || temp_image[x-captureWindow][y+i] == 1 || temp_image[x+captureWindow+1][y+i] == 1) {
                    edge = 0;
                    break;
                  }  
                }
                
                if (edge) {
                    // coordinates_t center;
                    // center.x = x-2;
                    // center.y = y-2;
                    // cellCenters[*cellCount] = center;
                    cellCenters[*cellCount].x = x-2;
                    cellCenters[*cellCount].y = y-2;
                    *cellCount += 1;
                    
                    for(int i = -captureWindow+1; i <= captureWindow; i++) {
                        for(int j = -captureWindow+1; j <= captureWindow; j++) {
                            temp_image[x+i][y+j] = 0;
                        }
                    }
                } 
            }
        }
    }
}


// Function that marks the cells in the image
void markCells(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], coordinates_t cellCenters[301], unsigned int cellCount) {
    // Sets the output image equal to the input
    for(int x = 0; x < BMP_WIDTH; x++) {
        for(int y = 0; y < BMP_HEIGTH; y++) {
            for(int c = 0; c < BMP_CHANNELS; c++) {
                output_image[x][y][c] = input_image[x][y][c];
            }
        }
    }

    // Overwrites the input image with red crosses
    for(int i = 0; i < cellCount; i++) {
        int x = cellCenters[i].x;
        int y = cellCenters[i].y;
        for(int j = -8; j < 9; j++) {
            if (x+j < 0 || x+j >= BMP_HEIGTH || y+j < 0 || y+j >= BMP_HEIGTH) continue;
            output_image[x+j][y][0] = 255;
            output_image[x+j][y][1] = 0;
            output_image[x+j][y][2] = 0;

            output_image[x][y+j][0] = 255;
            output_image[x][y+j][1] = 0;
            output_image[x][y+j][2] = 0;
        }
    }
}