//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "cbmp.h"

// Function which finds best threshold value
float otsu(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned int channel) {
    // The "histogram" will be set up in an array containing the amount of every value
    unsigned int histogramCurve[256];

    // Here we calculate the amount of values
    // Histogram is full of ERRORS! sometimes values are waaaay too large 
    // (a given histogram[x] can at most be = 902.500 if every other histgram[y] = 0, but here values are in multiple millions)
    // Maybe cause we have to use "malloc" or the likes to allocate more memory
    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_WIDTH; y++) {
            unsigned int val = input_image[x][y][channel];
            histogramCurve[val]++;
        }
    }

    // This is for finding the probabilities
    float prob[256];
    for (int i = 0; i < 256; i++) {
        //printf("histo[%i]: %i\n", i, histogramCurve[i]);
        prob[i] = ((float) histogramCurve[i])/(BMP_WIDTH*BMP_HEIGTH);
        //printf("Prob[%i]: %f\n", i, prob[i]);

    }

    // Find the cumulative probabilities
    float cumulProb[256];
    cumulProb[0] = 0;
    for (int i = 1; i < 256; i++) {
        cumulProb[i] = cumulProb[i-1]+prob[i];
    }

    //printf("cumulProb[255]: %f", cumulProb[255]);

    // Gotta understand this part a little better
    int bestThreshold = 0;
    float maxBetweenClassVar = 0;

    for (int threshold = 0; threshold < 256; threshold++) {
        float meanFore = 0;
        float meanBack = 0;

        for (int i = 0; i < threshold; i++) {
            meanBack += i*prob[i];
        }

        for (int i = threshold; i < 256; i++) {
            meanFore += i*prob[i];
        }

        float betweenClassVar = cumulProb[threshold] * (1-cumulProb[threshold]) * pow((meanBack-meanFore), 2);

        if (betweenClassVar > maxBetweenClassVar) {
            maxBetweenClassVar = betweenClassVar;
            bestThreshold = threshold;
        }

    }

    return bestThreshold;

}


//Function to invert pixels of an image (negative)
void greyscaleAndBinary(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH]){
    int threshRed = otsu(input_image,0);
    int threshGreen = otsu(input_image,1);
    int threshBlue = otsu(input_image,2);

    printf("red: %f\n green: %f\n blue: %f\n", threshRed, threshGreen, threshBlue);

    int thresh = (int) (threshRed+threshGreen+threshBlue)/3;
    

    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
            if((input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2])/3 >= thresh) {
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

int shouldErode(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], int x, int y) {
  for (int i = -1; i <= 1; i++) {
    if (temp_image[x][y+i] == 0) return 1;
    if (temp_image[x+i][y] == 0) return 1;
    if (temp_image[x+i][y+i] == 0) return 1;
    return 0;
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
                    if(shouldErode(temp_image,x,y)) {
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
                  if(temp_image[x+i][y-6] == 1 || temp_image[x+i][y+7] == 1 || temp_image[x-6][y+i] == 1 || temp_image[x+7][y+i] == 1) {
                      edge++;
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
                for(int i = -5; i < 6; i++) {
                  for (int j = -1; j <= 1; j++) {
                    output_image[x+i][y+j][0] = 255;
                    output_image[x+i][y+j][1] = 0;
                    output_image[x+i][y+j][2] = 0;

                    output_image[x+j][y+i][0] = 255;
                    output_image[x+j][y+i][1] = 0;
                    output_image[x+j][y+i][2] = 0;
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

  clock_t start, end;
  double cpu_time_used;
  start = clock();

  //Checking that 2 arguments are passed
  if (argc != 3)
  {
      fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
      exit(1);
  }

  //Load image from file
  read_bitmap(argv[1], input_image);

  //Run inversion
  greyscaleAndBinary(input_image,temp_image);

  greyToBmp(temp_image, output_image);

  write_bitmap(output_image, "grayscaleTest.bmp");

  char str[20];


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

end = clock();
cpu_time_used = end - start;
printf("Total time: %f ms\n", cpu_time_used * 1000.0 /CLOCKS_PER_SEC);
  return 0;
}