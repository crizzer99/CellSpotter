//To compile (win): gcc cbmp.c binary.c eroder.c otsusMethod.c spotCells.c main.c -o main.exe -std=c99
//To run (win): main.exe .\samples\easy\2EASY.bmp .\eroded_and_tests\example_marylou.bmp

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "binary.h"
#include "cbmp.h"
#include "eroder.h"
#include "otsusMethod.h"
#include "spotCells.h"


//Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH];
coordinates_t cellCenters[301];


//Main function
int main(int argc, char** argv) {
    //argc counts how may arguments are passed
    //argv[0] is a string with the name of the program
    //argv[1] is the first command line argument (input image)
    //argv[2] is the second command line argument (output image)
    
    unsigned int cellCount = 0; // Counts amount of cells registered
    unsigned int pixelsLeft = 0; // Counts if any pixels are left to erode

    clock_t start, end;
    double cpu_time_used;
    start = clock();

    //Checking that 2 arguments are passed
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
        exit(1);
    }
    
    // Translate 3d bmp to 2d binary image
    binary(argv[1], input_image,temp_image);

    toBmp(temp_image, output_image, ".\\eroded_and_tests\\grayscaleTest.bmp");
    

    // While loop that erodes until fully eroded
    do {
        erosion(temp_image, &pixelsLeft);
        checkImage(temp_image, cellCenters, &cellCount);
    } while (pixelsLeft > 0);

    for (int i = 0; i < cellCount; i++) {
      printf("cellCenters[%i] = (%i, %i)\n", i, cellCenters[i].x, cellCenters[i].y);
    }
    
    printf("Amount of detected cells: %i \n", cellCount);

    // Marks cells with red crosses
    markCells(input_image, output_image, cellCenters, cellCount, argv[2]);

    end = clock();
    cpu_time_used = end - start;
    printf("Total time: %f ms\n", cpu_time_used * 1000.0 /CLOCKS_PER_SEC);
    
    return 0;
}
