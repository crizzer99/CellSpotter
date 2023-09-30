//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe .\samples\easy\2EASY.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "cbmp.h"

struct coordinates{
    unsigned int x;
    unsigned int y;
};

// Function which finds best threshold value
float otsu(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], int startX, int startY, int resolution) {
    
    // Total amount of pixels
    int nPixels = resolution*resolution;
    // The histogram will be set up in an array containing the amount of every value
    unsigned int *histogramCurve = calloc(256, sizeof(unsigned int));
    
    // Here we calculate the amount of values
    for (int x = startX; x < startX+resolution; x++) {
        for (int y = startY; y < startY+resolution; y++) {
            unsigned int val = temp_image[x][y];
            histogramCurve[val]++;
        }
    }

    // This is for finding the probabilities
    float prob[256];
    for (int i = 0; i < 256; i++) {
        //printf("histo[%i]: %i\n", i, histogramCurve[i]);
        prob[i] = ((float) histogramCurve[i])/nPixels;
        //printf("Prob[%i]: %f\n", i, prob[i]);

    }

    int bestThreshold = 0;
    float maxBetweenClassVar = 0;

    for (int threshold = 0; threshold < 256; threshold++) {
        // amount of pixels in the foreground
        float pForeground = 0;
        // amount of pixels in the background
        float pBackground = 0;
        // Sum of all values in foreground. used to find mean from division by amount of values
        unsigned int sumFore = 0;
        // Sum of all values in background. used to find mean from division by amount of values
        unsigned int sumBack = 0;
        //float samples = 0;

        // Amount of pixels in background (?)
        for (int i = 0; i < threshold; i++) {
            pBackground += histogramCurve[i];
            sumBack += i*prob[i];
            //samples += histogramCurve[i];
        }

        // Amount of pixels in foreground (?)
        for (int i = threshold; i < 256; i++) {
            pForeground += histogramCurve[i];
            sumFore += i*histogramCurve[i];
        }

        // w0 and w1
        float weightBackground = pBackground/nPixels;
        float weightForeground = pForeground/nPixels;
        // mu0 and mu1
        float meanBackground = sumBack/pBackground;
        float meanForeground = sumFore/pForeground;

        // w0*w1*(mu0-mu1)^2
        float betweenClassVar = weightBackground*weightForeground*(meanBackground-meanForeground)*(meanBackground-meanForeground);

        if (betweenClassVar > maxBetweenClassVar) {
            maxBetweenClassVar = betweenClassVar;
            bestThreshold = threshold;
        }

    }

    return bestThreshold;

}

// Function which draws the smaller parts of the image in binary
void loadParts(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], int startX, int startY, int resolution, int thresh) {
    for (int x = startX; x < startX+resolution; x++) {
        for (int y = startY; y < startY+resolution; y++) {
            if(temp_image[x][y] >= thresh) {
                temp_image[x][y] = 1;
            } else {
                temp_image[x][y] = 0;
            }
        }
    }

}


//Function to create the binary image (black and white)
void binary(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH]) {

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

// Takes the binary image and translates to black and white .bmp file
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

// Looks through the binary image to see spot cells
void checkImage(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], struct coordinates cellCenters[301], unsigned int *cellCount) {
    for(int x = 0; x < BMP_WIDTH; x++) {
        for(int y = 0; y < BMP_HEIGTH; y++) {
            if(temp_image[x][y] == 1) {
                int captureWindow = 16;
                int edge = 1;
                for(int i = -captureWindow/2; i <= captureWindow/2+1; i++) {  
                  if(temp_image[x+i][y-captureWindow/2] == 1 || temp_image[x+i][y+captureWindow/2+1] == 1 || temp_image[x-captureWindow/2][y+i] == 1 || temp_image[x+captureWindow/2+1][y+i] == 1) {
                    edge = 0;
                    break;
                  }  
                }
                
                if (edge) {
                    struct coordinates center;
                    center.x = x;
                    center.y = y;
                    cellCenters[*cellCount] = center;
                    *cellCount += 1;
                    
                    for(int i = -captureWindow/2+1; i <= captureWindow/2; i++) {
                        for(int j = -captureWindow/2+1; j <= captureWindow/2; j++) {
                            temp_image[x+i][y+j] = 0;
                        }
                    }
                }
                
                
            }
        }
    }
}

// Performs the erosion algorithm which erodes pixels at the edge of a cell
void erosion(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], unsigned int *pixelsLeft) {
    /*unsigned char **eroded;
	eroded = calloc(BMP_WIDTH, sizeof(unsigned char*));
	for (int i = 0; i < BMP_WIDTH; i++) {
		eroded[i] = calloc(BMP_HEIGTH, sizeof(unsigned char*));
	}*/
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

// Function that marks the cells in the image
void markCells(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], struct coordinates cellCenters[301], unsigned int cellCount) {
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

//Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH];
struct coordinates cellCenters[301];

//Main function
int main(int argc, char** argv) {
    //argc counts how may arguments are passed
    //argv[0] is a string with the name of the program
    //argv[1] is the first command line argument (input image)
    //argv[2] is the second command line argument (output image)

    clock_t start, end;
    double cpu_time_used;
    start = clock();

    //Checking that 2 arguments are passed
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
        exit(1);
    }

    //Load image from file
    read_bitmap(argv[1], input_image);
    // Translate 3d bmp to 2d binary image
    binary(input_image,temp_image);

    greyToBmp(temp_image, output_image);
    // create a .bmp of the binary image
    write_bitmap(output_image, "grayscaleTest.bmp");
    // Counts amount of cells registered
    unsigned int cellCount = 0;
    // Counts if any pixels are left to erode
    unsigned int pixelsLeft = 0;

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
    markCells(input_image, output_image, cellCenters, cellCount);

    //Save image to file
    write_bitmap(output_image, argv[2]);

    end = clock();
    cpu_time_used = end - start;
    printf("Total time: %f ms\n", cpu_time_used * 1000.0 /CLOCKS_PER_SEC);
    return 0;
}