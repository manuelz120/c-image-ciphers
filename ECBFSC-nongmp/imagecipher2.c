#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "imagecipher2.h"

// constant for running logistic map
static double LOGISTIC_R = 3.712345;

// used to cut of incorrect precision values on double values
// used (int)(value/PRECISION) / PRECISION
static double PRECISION = 1000000000000.0;

// convert a value from the logistic map to the range of an image byte = 0-255
unsigned char convertM2(double x) {
    return ((char)round(x)) * 255;
}

// convert a value from image bytes (as double because calculations could have happenend before) to logistic map range 0-1
double convertM1(double x) {
    static double DIVISOR_M1 = 256.0;
    return x / DIVISOR_M1;
}

AlgorithmParameter generateInitialContitions(unsigned char key[KEY_SIZE]) {
    AlgorithmParameter param;

    double r = 0;
    param.C = 0;

    for(int i = 0; i < KEY_SIZE; i++) {
        r += convertM1((double)key[i]);
        param.C += key[i];
    }

    param.C = param.C % 256;
    param.X = r - floor(r);

    return param;
}

void encrypt(AlgorithmParameter *params, unsigned char *imageBytes, int numberOfImageBytes, unsigned char key[KEY_SIZE]) {

    if(numberOfImageBytes > BUFFER_SIZE)
        exit(1);

    double lastXi = params->X;
    unsigned char lastEncryptedByte = params->C;

    for(int l = 0; l < numberOfImageBytes; l++) {
        // start at key pos 0 again after reaching end of key
        char nextKeyPos = (l+1) % KEY_SIZE;
        int numberOfLogisticMapRepititions = ((int)key[nextKeyPos]) + lastEncryptedByte;

        double xi = convertM1(lastXi + lastEncryptedByte + key[l % KEY_SIZE]);
        double logisticSum = 0.0;

        for(int i = 0; i < numberOfLogisticMapRepititions; i++) {
            logisticSum += LOGISTIC_R * xi * (1.0 - xi);
        }

        imageBytes[l] = (char)((imageBytes[l] + convertM2(logisticSum)) % 256);
        lastEncryptedByte = imageBytes[l];
        lastXi = xi;
    }
}

// same as encryption, except convertedBytes = origBytes - convertM2(logisticSum)
// copied, to avoid if in every iteration = better performance
void decrypt(AlgorithmParameter *params, unsigned char *imageBytes, int numberOfImageBytes, unsigned char key[KEY_SIZE]) {

    if(numberOfImageBytes > BUFFER_SIZE)
        exit(1);

    double lastXi = params->X;
    unsigned char lastEncryptedByte = params->C;

    for(int l = 0; l < numberOfImageBytes; l++) {
        // start at key pos 0 again after reaching end of key
        char nextKeyPos = (l+1) % KEY_SIZE;
        int numberOfLogisticMapRepititions = ((int)key[nextKeyPos]) + lastEncryptedByte;

        double xi = convertM1(lastXi + lastEncryptedByte + key[l % KEY_SIZE]);
        double logisticSum = 0.0;

        for(int i = 0; i < numberOfLogisticMapRepititions; i++) {
            logisticSum -= LOGISTIC_R * xi * (1.0 - xi);
        }

        lastEncryptedByte = imageBytes[l];
        imageBytes[l] = (char)((imageBytes[l] + convertM2(logisticSum)) % 256);
        lastXi = xi;
    }
}
