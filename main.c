//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

//Function to convert pixels of an image to gray-scale
void binaryImg(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char bin_image[BMP_WIDTH][BMP_HEIGTH]) {
  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {
      int gray = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2])/3;
        if (gray <= 90) {
          bin_image[x][y] = 0;
        } else {
          bin_image[x][y] = 1;
        }
    }
  }
}

void erosion(unsigned char bin_image[BMP_WIDTH][BMP_HEIGTH]) {

  int pixelCount = 0;
  unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH] = {0};
  
  for (int x = 0; x < BMP_WIDTH; x++) {
    for (int y = 0; y < BMP_HEIGTH; y++) {
      if(x == 0 || x == BMP_WIDTH-1 || y==0 || y == BMP_HEIGTH-1) eroded_image[x][y] = 0;
      if (bin_image[x][y] == 0) {
        eroded_image[x][y] = 0;
        continue;
      }  
      if (bin_image[x-1][y] == 1 && bin_image[x+1][y] == 1 && bin_image[x][y+1] == 1 && bin_image[x][y-1] == 1) {
        eroded_image[x][y] = 1;
        pixelCount++;
      } else {
        eroded_image[x][y] = 0;
      }
    }
  }

  // coordinateList(eroded_image);
  for(int x = 0; x < BMP_WIDTH; x++) {
        for(int y = 0; y < BMP_HEIGTH; y++) {
            printf("%d ", eroded_image[y][x]);
        }
        printf("\n");
    }
    printf("\n");
  if (pixelCount > 0) erosion(eroded_image);
  
}

  // Declaring the array to store the image (unsigned char = unsigned 8 bit)
  unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char bin_image[BMP_WIDTH][BMP_HEIGTH];
  unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

// Main function
int main(int argc, char** argv) {
  //argc counts how may arguments are passed
  //argv[0] is a string with the name of the program
  //argv[1] is the first command line argument (input image)
  //argv[2] is the second command line argument (output image)

  //Checking that 2 arguments are passed
  if (argc != 3) {
      fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
      exit(1);
  }

  printf("Example program - 02132 - A1\n");

  //Load image from file
  read_bitmap(argv[1], input_image);

  //Run inversion
  binaryImg(input_image,bin_image);
  erosion(bin_image);

  //Save image to file
  //write_bitmap(output_image, argv[2]);

  printf("Done!\n");
  return 0;
}
