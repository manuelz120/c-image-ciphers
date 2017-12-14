#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// when DEV is defined we get all output, useful for debugging
//#define DEV 1
// when TEST is defined we get minimum output, to verify expectations
#define TEST 1

// use floating point standard for precision
#ifndef __STDC_IEC_559__
#define __STDC_IEC_559__ 1
#endif

#define BUFFER_SIZE 32
#define KEY_SIZE 32 // as unsigned char array = 256 bit

#ifdef DEV
#define PTF(A, ...) printf(A, ##__VA_ARGS__);
#else
#define PTF(A, ...) ;
#endif

#ifdef TEST
#define PTF_IMPT(A, ...) printf(A, ##__VA_ARGS__);
#else
#define PTF_IMPT(A, ...) ;
#endif

#define ENC_MODE 1
#define DEC_MODE 2

typedef struct AlgorithmParameters
{
    double X;
    unsigned char C;
} AlgorithmParameter;

AlgorithmParameter generateInitialContitions(unsigned char *key);
void encrypt(AlgorithmParameter *params, unsigned char *imageBytes, int numberOfImageBytes, unsigned char *key);
void decrypt(AlgorithmParameter *params, unsigned char *imageBytes, int numberOfImageBytes, unsigned char *key);
