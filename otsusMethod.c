// otsusMethod.c
#include <stdlib.h>
#include <stdio.h>
#include "otsusMethod.h"

// Function which finds best threshold value
float otsu(unsigned char temp_image[BMP_WIDTH][BMP_HEIGTH], int startX, int startY, int resolution) {
    
    // Total amount of pixels
    int nPixels = resolution*resolution;
    // The histogram will be set up in an array containing the amount of every value
    unsigned int *histogramCurve = calloc(256, sizeof(unsigned int));
    
    // Here we calculate the amount of values
    for (int x = startX; x < startX+resolution; x++) {
        for (int y = startY; y < startY+resolution; y++) {
            histogramCurve[temp_image[x][y]]++;
        }
    }

    // This is for finding the probabilities
    float prob[256];
    for (int i = 0; i < 256; i++) {
        prob[i] = ((float) histogramCurve[i])/nPixels;
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