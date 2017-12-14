#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "imagecipher2.h"

#define MAX_FILE_PATH_LENGTH 512

/*
usage:
(follow these steps in the terminal in the root folder of the imagecipher2)
1. generate bytes file from PNG (= file with int representations of ARGB PNG files)
    python imageToBytes.py generatedimages/12x12image.png RGBA
(optional make main): make
2. generate encrypted bytes file from original bytes file
    ./main generatedimages/12x12image.pngbytes.txt 1
3. generate decrypted bytes file from encrypted bytes file
    ./main generatedimages/12x12image.pngbytes.txt.encrypted.txt 2
4. generate PNG from bytes TXT file
    python bytesToImage.py generatedimages/12x12image.pngbytes.txt.encrypted.txt.decrypted.txt 12 12
*/

int main(int argc, char *argv[])
{
    // -- reading input parameters
    if (argc < 3)
    {
        printf("no file path provided and/or mode 1 = encryption, 2 = decryption, usage <path> <mode>\n");
        exit(1);
    }

    char filePath[MAX_FILE_PATH_LENGTH];

    if (strlen(argv[1]) >= MAX_FILE_PATH_LENGTH)
    {
        fprintf(stderr, "%s is too long!\n", argv[1]);
        exit(1);
    }
    else
        strcpy(filePath, argv[1]);

    int mode = atoi(argv[2]);

    /*int rounds = 5;
    if(argc == 4)
        rounds = atoi(argv[3]);*/

    PTF_IMPT("\nusing file: %s\n", filePath);

    char convFilePath[MAX_FILE_PATH_LENGTH + 15];
    strcpy(convFilePath, filePath);

    if (mode == DEC_MODE)
    {
        strcat(convFilePath, ".decrypted.txt");
        PTF_IMPT("running decryption\n");
    }
    else if (mode == ENC_MODE)
    {
        strcat(convFilePath, ".encrypted.txt");
        PTF_IMPT("running encryption\n");
    }

    // --- done reading input params

    // open input file and output file
    FILE *origFile = fopen(filePath, "r");
    if (origFile == NULL)
    {
        fprintf(stderr, "Can't open input file!\n");
        exit(1);
    }

    FILE *convFile = fopen(convFilePath, "w");
    if (convFile == NULL)
    {
        fprintf(stderr, "Can't open output file %s!\n", convFilePath);
        exit(1);
    }

    // use same key for tests, TODO prod use key from command line
    unsigned char *key = "manuelmanuelmanuelmanuelmanuelma";
    //unsigned char *key = "cryptocryptocrypocryptocryptocry";

    PTF_IMPT("using Key: ");
    for (int i = 0; i < KEY_SIZE; i++)
    {
        PTF_IMPT("%u ", key[i]);
    }
    PTF_IMPT("\n")

    PTF("------ READING: ");

    double timeTakenAlgorithmOnly = 0.0;

    AlgorithmParameter params = generateInitialContitions(key);

    PTF_IMPT("---- using parameters: X= %0.15f, C= %d \n", params.X, params.C);

    // measure overall time
    clock_t begin = clock();

    fseek(origFile, 0, SEEK_END);
    long fsize = ftell(origFile);
    fseek(origFile, 0, SEEK_SET);

    unsigned char *myBuffer = malloc(fsize);

    int bufferPos = 0;
    int value;

    do
    {
        fscanf(origFile, "%d", &value);
        myBuffer[bufferPos] = (unsigned char)value;
        bufferPos++;
    } while (bufferPos < fsize && !feof(origFile));

    fclose(origFile);

    clock_t begin_algo = clock();

    if (mode == DEC_MODE)
    {
        decrypt(&params, myBuffer, bufferPos, key);
    }
    else if (mode == ENC_MODE)
    {
        encrypt(&params, myBuffer, bufferPos, key);
    }

    clock_t end_algo = clock();
    timeTakenAlgorithmOnly += (double)(end_algo - begin_algo) / CLOCKS_PER_SEC;

    for (int i = 0; i < bufferPos; i++)
    {
        PTF("%u ", myBuffer[i]);
        fprintf(convFile, "%d ", (int)myBuffer[i]);
    }

    PTF("\n------ DONE ------- \n");

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    PTF_IMPT("\ntook total %0.15f seconds\n", time_spent);
    PTF_IMPT("\ntook algo only %0.15f seconds\n", timeTakenAlgorithmOnly);

    PTF_IMPT("created file: %s\n", convFilePath);

    fclose(convFile);

    return 0;
}
