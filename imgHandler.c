//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

//Function to invert pixels of an image (negative)
void greyscaleAndBinary(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH]){
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            if((input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2])/3 >= 90) {
                temp_image[x][y] = 1;
            } else {
                temp_image[x][y] = 0;
            }
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

void erosion(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], unsigned char eroded[BMP_WIDTH][BMP_HEIGTH]) {
    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
            if(x == 0 || x == BMP_WIDTH-1 || y == 0 || y == BMP_HEIGTH-1) {
                eroded[x][y] = 0;
            } else {
                if(temp_image[x][y] == 1) {
                    eroded[x][y] = 1;
                    if(temp_image[x-1][y] == 0 || temp_image[x+1][y] == 0 || temp_image[x][y-1] == 0 || temp_image[x][y+1] == 0 || temp_image[x-1][y-1] == 0 || temp_image[x-1][y+1] == 0 || temp_image[x+1][y+1] == 0 || temp_image[x+1][y-1] == 0) {
                        eroded[x][y] = 0;
                    }
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

void checkImage(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], unsigned char cross_coordinates[BMP_WIDTH][BMP_HEIGTH]) {
    for(int x = 0; x < BMP_WIDTH; x++) {
        for(int y = 0; y < BMP_HEIGTH; y++) {
            if(temp_image[x][y] == 1) {
                int edge = 0;
                for(int i = -6; i <= 7; i++) {
                    for(int j = -6; j <= 7; j++) {
                        if(temp_image[x+i][y-6] == 1 || temp_image[x+i][y+7] == 1 || temp_image[x-6][y+j] == 1 || temp_image[x+7][y+j] == 1) {
                            edge++;
                        }

                    }
                }
                if(edge == 0) {
                    cross_coordinates[x][y] = 1;
                    for(int i = -6; i <= 7; i++) {
                        for(int j = -6; j <= 7; j++) {
                            temp_image[x+i][y+j] = 0;
                        }
                    }
                }
            }
        }
    }
}

void markCells(unsigned char cross_coordinates[BMP_WIDTH][BMP_HEIGTH], unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]) {
    for(int x = 0; x < BMP_WIDTH; x++) {
        for(int y = 0; y < BMP_HEIGTH; y++) {
            for(int c = 0; c < BMP_CHANNELS; c++) {
                output_image[x][y][c] = input_image[x][y][c];
            }
        }
    }
    for(int x = 0; x < BMP_WIDTH; x++) {
        for(int y = 0; y < BMP_HEIGTH; y++) {
            if(cross_coordinates[x][y] == 1) {
                for(int i = 0; i < 6; i++) {
                    for(int j = 0; j < 6; j++) {
                        output_image[x+i][y+j][0] = 255;
                        output_image[x+i][y+j][1] = 0;
                        output_image[x+i][y+j][2] = 0;
                    }
                }
            }
        }
    }
}




//Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH];
unsigned char cross_coordinates[BMP_WIDTH][BMP_HEIGTH];
unsigned char eroded[BMP_WIDTH][BMP_HEIGTH];
unsigned char circleArr[BMP_WIDTH][BMP_HEIGTH];

//Main function
int main(int argc, char** argv)
{
  //argc counts how may arguments are passed
  //argv[0] is a string with the name of the program
  //argv[1] is the first command line argument (input image)
  //argv[2] is the second command line argument (output image)

  //Checking that 2 arguments are passed
  if (argc != 3)
  {
      fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
      exit(1);
  }

  printf("Example program - 02132 - A1\n");

  //Load image from file
  read_bitmap(argv[1], input_image);

  //Run inversion
  greyscaleAndBinary(input_image,temp_image);

  greyToBmp(temp_image, output_image);

  write_bitmap(output_image, "grayscaleTest.bmp");

  char str[20];

  //circular(temp_image, circleArr);


  for(int i = 0; i < 15; i++) {
    printf("%d iteration of erosion done \n", i);
    erosion(temp_image, eroded);
    greyToBmp(temp_image, output_image);
    write_bitmap(output_image, "erode.bmp");
    printf("%d iteration of check image done \n", i);
    checkImage(temp_image, cross_coordinates);
    printf("\n");
  }
  

int count = 0;
for(int x = 0; x < BMP_WIDTH; x++) {
    for(int y = 0; y < BMP_HEIGTH; y++) {
        if(cross_coordinates[x][y] == 1) {
            count++;
        }
    }
}
printf("Amount of detected cells: %d \n", count);

 markCells(cross_coordinates, input_image, output_image);

  //Save image to file
  write_bitmap(output_image, argv[2]);

  printf("Done!\n");
  return 0;
}
