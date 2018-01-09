#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "imagecipher2.h"

// constant for running logistic map
static double LOGISTIC_R = 3.712345;

// convert a value from the logistic map to the range of an image byte = 0-255
unsigned char M2(double x)
{
    return (unsigned char)round(x * 255);
}

// convert a value from image bytes (as double because calculations could have happenend before) to logistic map range 0-1
double M1(double x)
{
    static double DIVISOR_M1 = 255.0;
    return x / DIVISOR_M1;
}

AlgorithmParameter generateInitialContitions(unsigned char key[KEY_SIZE])
{
    AlgorithmParameter param;

    double r = 0;
    param.C = 0;

    for (int i = 0; i < KEY_SIZE; i++)
    {
        r += M1((double)key[i]);
        param.C += key[i];
    }

    param.C = param.C % 256;
    param.X = r - floor(r);

    return param;
}

void encrypt(AlgorithmParameter *params, unsigned char *imageBytes, int numberOfImageBytes, unsigned char key[KEY_SIZE])
{
    double lastXi = params->X;
    unsigned char lastEncryptedByte = params->C;

    for (int i = 0; i < numberOfImageBytes; i++)
    {
        // start at key pos 0 again after reaching end of key
        int nextKeyPos = (i + 1) % KEY_SIZE;
        int numberOfLogisticMapRepititions = key[nextKeyPos] + lastEncryptedByte;

        double xi = M1(lastXi + lastEncryptedByte + key[i % KEY_SIZE]);
        double logisticSum = 0.0;

        for (int j = 0; j < numberOfLogisticMapRepititions; j++)
        {
            logisticSum += LOGISTIC_R * xi * (1.0 - xi);
        }

        lastXi = M2(logisticSum);
        imageBytes[i] = (unsigned char)((int)(imageBytes[i] + lastXi) % 256);
        lastEncryptedByte = imageBytes[i];
    }
}

// same as encryption, except convertedBytes = origBytes - convertM2(logisticSum)
// copied, to avoid if in every iteration = better performance
void decrypt(AlgorithmParameter *params, unsigned char *imageBytes, int numberOfImageBytes, unsigned char key[KEY_SIZE])
{

    double lastXi = params->X;
    unsigned char lastEncryptedByte = params->C;

    for (int l = 0; l < numberOfImageBytes; l++)
    {
        // start at key pos 0 again after reaching end of key
        int nextKeyPos = (l + 1) % KEY_SIZE;
        int numberOfLogisticMapRepititions = key[nextKeyPos] + lastEncryptedByte;

        double xi = M1(lastXi + lastEncryptedByte + key[l % KEY_SIZE]);
        double logisticSum = 0.0;

        for (int i = 0; i < numberOfLogisticMapRepititions; i++)
        {
            logisticSum += LOGISTIC_R * xi * (1.0 - xi);
        }

        lastXi = M2(logisticSum);
        lastEncryptedByte = imageBytes[l];
        imageBytes[l] = (char)((int)(imageBytes[l] - lastXi) % 256);
    }
}
