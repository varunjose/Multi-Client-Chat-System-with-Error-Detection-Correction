#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h> 

// Constants for CRC32
const uint32_t CRC32_POLYNOMIAL = 0xEDB88320;
const uint32_t CRC32_INITIAL_VALUE = 0xFFFFFFFF;

// File paths
const char* INPUT_FILE_PATH = "/home/varunjose/Desktop/OS/intext.txt";
const char* OUTPUT_FILE_PATH = "/home/varunjose/Desktop/OS/resultcrc.txt";

// Function to print a 32-bit integer in binary format
void printBinary(FILE *file, uint32_t value) {
    for (int i = 31; i >= 0; i--) {
        fprintf(file, "%d", (value & (1u << i)) ? 1 : 0);
    }
}

// Function to correct an error in the data
void correctError(unsigned char *data, size_t position) {
    // Attempt to correct the error by flipping the bit back
    data[position] ^= 0x01;
}

// Function to find the position of the first differing bit
size_t findFirstDifferingBit(uint32_t originalCRC, uint32_t receivedCRC) {
    uint32_t xorResult = originalCRC ^ receivedCRC;

    // Find the position of the first differing bit
    size_t position = 0;
    while ((xorResult & (1u << position)) == 0 && position < 32) {
        position++;
    }

    return position;
}

// Function to calculate CRC32 checksum
uint32_t calculateCRC32(const unsigned char *data, size_t length, uint32_t initial) {
    uint32_t crc = initial;
    size_t i, j;

    for (i = 0; i < length; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ CRC32_POLYNOMIAL;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}

int main() {
    // Open input file
    FILE *inputFile = fopen(INPUT_FILE_PATH, "rb");
    if (!inputFile) {
        perror("Error opening input file");
        return 1;
    }

 
    FILE *outputFile = fopen(OUTPUT_FILE_PATH, "wb");
    if (!outputFile) {
        perror("Error opening output file");
        fclose(inputFile);
        return 1;
    }

    fseek(inputFile, 0, SEEK_END);
    size_t length = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    unsigned char *data = malloc(length);
    fread(data, 1, length, inputFile);

    uint32_t originalCRC = calculateCRC32(data, length, CRC32_INITIAL_VALUE);

    size_t errorPosition = rand() % length;
    data[errorPosition] ^= 0x01;


    uint32_t receivedCRC = calculateCRC32(data, length, CRC32_INITIAL_VALUE);

    fprintf(outputFile, "Original CRC32: ");
    printBinary(outputFile, originalCRC);
    fprintf(outputFile, "\n\nReceived CRC32: ");
    printBinary(outputFile, receivedCRC);
    fprintf(outputFile, "\n\n");

    if (originalCRC == receivedCRC) {
        fprintf(outputFile, "No errors detected.\n\n");
    } else {
        fprintf(outputFile, "Error detected at position %zu.\n", errorPosition);

        correctError(data, errorPosition);

        uint32_t correctedCRC = calculateCRC32(data, length, CRC32_INITIAL_VALUE);

        fprintf(outputFile, "Error corrected at position %zu.\n", errorPosition);
        fprintf(outputFile, "Corrected bit: %d\n", data[errorPosition]);
        fprintf(outputFile, "Corrected CRC32: ");
        printBinary(outputFile, correctedCRC);
        fprintf(outputFile, "\n\n");


        fwrite(data, 1, length, outputFile);
    }

    free(data);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
