// binary.c
#include <stdlib.h>
#include <stdio.h>
#include "binary.h"

// Function which draws the smaller parts of the image in binary
void loadParts(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], int startX, int startY, int resolution, int thresh) {
    for (int x = startX; x < startX+resolution; x++) {
        for (int y = startY; y < startY+resolution; y++) {
            temp_image[x][y] = (temp_image[x][y] >= thresh); 
            // if(temp_image[x][y] >= thresh) {
            //     temp_image[x][y] = 1;
            // } else {
            //     temp_image[x][y] = 0;
            // }
        }
    }
}

// Takes the binary image and translates to black and white .bmp file
void toBmp(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], char * output_file_path) {
    for(int x = 0; x < BMP_WIDTH; x++) {
        for(int y = 0; y < BMP_HEIGTH; y++) {
            if(temp_image[x][y] == 1) {
                output_image[x][y][0] = 255;
                output_image[x][y][1] = 255;
                output_image[x][y][2] = 255;
            } else {
                output_image[x][y][0] = 0;
                output_image[x][y][1] = 0;
                output_image[x][y][2] = 0;
            }
        }
    }
    // creates a .bmp of the binary image
    write_bitmap(output_image, output_file_path);
}

//Function to create the binary image (black and white)
void binary(char * input_file_path, unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH]) {

    //Load image from file
    read_bitmap(input_file_path, input_image);

    // greyscale image
    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
            temp_image[x][y] = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2])/3;
        }
    }

    // Binary image
    // the actual amount of parts is parts^2, since parts is the amount of parts each dimension is split into
    int parts = 2;
    int resolution = BMP_HEIGTH/parts;
    for (int i = 0; i < parts; i++) {
        for (int j = 0; j < parts; j++) {
            int thresh = (int) otsu(temp_image, i*resolution, j*resolution, resolution);
            //printf("Thresh: %i for pixels [%i;%i] x [%i;%i]\n", thresh, i*resolution, i*resolution+resolution, j*resolution, j*resolution+resolution);
            loadParts(temp_image, i*resolution, j*resolution, resolution, thresh);
        }    
    } 
     
}

