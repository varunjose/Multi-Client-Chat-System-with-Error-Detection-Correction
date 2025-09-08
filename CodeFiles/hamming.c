#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void charToBinary(char c, char *binary) {
    int i;
    for (i = 7; i >= 0; --i) {
        binary[i] = (c % 2) + '0';
        c /= 2;
    }
    binary[8] = '\0';  
}

void calculateHammingCode(char *data, char *hamming) {
    hamming[0] = (data[0] + data[1] + data[3]) % 2 + '0';
    hamming[1] = (data[0] + data[2] + data[3]) % 2 + '0';
    hamming[2] = data[0];
    hamming[3] = (data[1] + data[2] + data[3]) % 2 + '0';
    hamming[4] = data[1];
    hamming[5] = data[2];
    hamming[6] = data[3];
}

void correctError(char *hamming, int errorPos) {
    hamming[errorPos - 1] = (hamming[errorPos - 1] == '0') ? '1' : '0';
}

int detectError(char *hamming) {
    int errorPosition = 0;
    errorPosition += (hamming[0] + hamming[2] + hamming[4] + hamming[6]) % 2;
    errorPosition += (hamming[1] + hamming[2] + hamming[5] + hamming[6]) % 2 * 2;
    return errorPosition;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file_path> <output_file_path>\n", argv[0]);
        return 1;
    }

    FILE *inputFile, *outputFile;
    char inputBuffer[1000];
    char binaryData[9];
    char hammingCode[7];

    inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        printf("Error opening the input file.\n");
        return 1;
    }

    outputFile = fopen(argv[2], "w");
    if (outputFile == NULL) {
        printf("Error creating the output file.\n");
        fclose(inputFile);
        return 1;
    }

    while (fgets(inputBuffer, sizeof(inputBuffer), inputFile) != NULL) {
        int i;
        for (i = 0; i < strlen(inputBuffer); ++i) {
            charToBinary(inputBuffer[i], binaryData);
            calculateHammingCode(binaryData, hammingCode);
            fprintf(outputFile, "%s", hammingCode);
            if ((i + 1) % 4 == 0) {
                fprintf(outputFile, "\n");  
            }
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    outputFile = fopen(argv[2], "r");
    if (outputFile == NULL) {
        printf("Error opening the output file.\n");
        return 1;
    }

    printf("\nDetected and Corrected Errors:\n");

    while (fgets(inputBuffer, sizeof(inputBuffer), outputFile) != NULL) {
        int errorPos = detectError(inputBuffer);
        if (errorPos > 0) {
            printf("Error at position %d\n", errorPos);
            correctError(inputBuffer, errorPos);
            printf("Corrected: %s\n", inputBuffer);
        } else {
            printf("No errors\n");
        }
    }

    fclose(outputFile);

    return 0;
}
