//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <time.h>

/*
typedef struct {
  unsigned int x;
  unsigned int y;
} Coordinate;*/

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char bin_image[BMP_WIDTH][BMP_HEIGTH];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

// Array of type coordinates which saves the center of every cell
unsigned int n = 0;
int N = 0;
// Coordinate centerCoords[3];

// Erases the cell from the image
void eraseCell(unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH], int x, int y) {
  for (int i = -5; i <= 6; i++ ) {
    for (int j = -5; j <= 6; j++ ) {
      eroded_image[x+i][y+j] = 0;
    }
  }  
}

void greyToBmp(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]) {
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
}

// Function that finds and stores the center of cells
void detectCells(unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH]) {

  int cellDetected = 0;

  // Could start at x=y=6 and end at x=y=BMP_WIDTH-7
  for(int x = 0; x < BMP_WIDTH; x++) {
    for(int y = 0; y < BMP_HEIGTH; y++) {

      cellDetected = 0;
      
      if (eroded_image[x][y] == 1) {
        for (int i = -6; i <= 7; i++ ) {
          if (x+i < 0 || x+i > BMP_WIDTH-1 || y+i < 0 || y+i > BMP_HEIGTH-1) continue;
          if (eroded_image[x+i][y-6] == 1 || eroded_image[x+i][y+7] == 1 || 
              eroded_image[x-6][y+i] == 1 || eroded_image[x+7][y+i] == 1 ) {
                cellDetected = 0;
                break;
          }      
              cellDetected = 1;    
        }
      }

      // need a definition for the center 
      // (currently using middle of detection area as center coordinates)
      if (cellDetected == 1) {
        // Stores the coordinates for the center
        // centerCoords[n] = Coordinate(x,y);
        n++;
        eraseCell(eroded_image, x, y);
      }
    }
  }
}

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
  printf("Iteration: %i \n", N);
  int pixelCount = 0;
  unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH];
  
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
  N++;
  printf("Start detect! \n");
  detectCells(eroded_image);
  printf("End detect! \n");
  greyToBmp(eroded_image, output_image);
  printf("Pixelcount is: %i \n", pixelCount);
  if (pixelCount > 0) erosion(eroded_image);
  // markCells();
  
}




/*
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

// Main function
int main(int argc, char** argv) {
  //argc counts how may arguments are passed
  //argv[0] is a string with the name of the program
  //argv[1] is the first command line argument (input image)
  //argv[2] is the second command line argument (output image)
  /*
  clock_t start, end;
    double cpu_time_used;
    start = clock();
  */
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
  printf("Amount of cells found: %i",n);
  //Save image to file
  write_bitmap(output_image, argv[2]);
  /*
  end = clock();
  cpu_time_used = end - start;
  printf("Total time: %f ms\n", cpu_time_used * 1000.0 /
  CLOCKS_PER_SEC);*/
  return 0;
}
