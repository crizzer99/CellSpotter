//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

/*struct Coordinate {
  unsigned int x;
  unsigned int y;
};*/

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

// Recursive function which erodes the picture until everything's black
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

  // checkCells(*eroded_image);
  for(int x = 0; x < BMP_WIDTH; x++) {
    for(int y = 0; y < BMP_HEIGTH; y++) {
      printf("%d ", eroded_image[y][x]);
    }
    printf("\n");
  }
  printf("\n");
  if (pixelCount > 0) erosion(eroded_image);
  // markCells();
  
}
/*

// Function which detects cells before erosion
// This makes the area to search for cells to be eroded much smaller
// Can be used to find the center coords as well?
void detectCell() {

}

// Function that finds and stores the center of cells
// Pointer to "eroded_image" so values are updated in "erosion()"
void checkCells(unsigned char *eroded_image[BMP_WIDTH][BMP_HEIGTH]) {
  for(int x = 0; x < BMP_WIDTH; x++) {
    for(int y = 0; y < BMP_HEIGTH; y++) {
      
      // need a definition for the center
      if (0) {
        // Stores the coordinates for the center
        centerCoords[n] = Coordinate(x,y);

        // Erases the cell from the image
        eraseCell(eroded_image);
      }

    }
  }
}

// Need to figure out how to use variables as fields
void markCells() {
  for(int i = 0; i < centerCoords.length; i++) {  
    unsigned int x = centerCoords[i].x;
    unsigned int y = centerCoords[i].y;

    for(int j = -7; j < 8; j++) {
        output_image[x+i][y+j][0] = 255;
        output_image[x+i][y+j][1] = 0;
        output_image[x+i][y+j][2] = 0;
    }
  }
}
*/

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char bin_image[BMP_WIDTH][BMP_HEIGTH];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

// Array of type coordinates which saves the center of every cell
//unsigned int n = 0;
//Coordinate centerCoords[3];


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
