// gcc ent.c -o ent -lm
// objcopy --dump-section .text=ent.bin ent
// ./ent <binary_file> <hex_byte>
// ./ent ent.bin 48

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv)
{
    unsigned int targetByte;
    sscanf(argv[2], "%x", &targetByte);

    FILE * file = fopen(argv[1], "rb");

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char * buffer = malloc(fileSize);

    fread(buffer, 1, fileSize, file);
    fclose(file);

    long byteFrequency[256] = {0};
    for (long x = 0; x < fileSize; x++) 
    {
        byteFrequency[buffer[x]]++;
    }

    printf("\nByte Frequencies:\n");
    printf("----------------\n");
    long totalBytes = 0;
    for (int y = 0; y < 256; y++) 
    {
        printf("0x%02X : %ld\n", y, byteFrequency[y]);
        totalBytes += byteFrequency[y];
    }
    printf("\nTotal Bytes Analyzed: %ld\n", totalBytes);

    double shannonEntropy = 0.0;
    for (int z = 0; z < 256; z++) 
    {
        if (byteFrequency[z]) 
        {
            shannonEntropy = shannonEntropy - (double)byteFrequency[z]/fileSize * log2((double)byteFrequency[z]/fileSize);
        }
    }
    printf("\nShannon Entropy (zero-order): %.3f bits per byte\n", shannonEntropy);

    long bigramFrequency[256][256] = {{0}};
    long numBigrams = fileSize - 1;

    for (long a = 0; a < numBigrams; a++) 
    {
        bigramFrequency[buffer[a]][buffer[a+1]]++;
    }

    double bigramJointEntropy = 0.0;
    for (int b = 0; b < 256; b++) 
    {
        for (int c = 0; c < 256; c++) 
        {
            if (bigramFrequency[b][c]) 
            {
                bigramJointEntropy = bigramJointEntropy - (double)bigramFrequency[b][c] / numBigrams * log2((double)bigramFrequency[b][c] / numBigrams);
            }
        }
    }

    double bigramEntropyPerByte = bigramJointEntropy / 2.0;

    printf("\nBigram entropy: %.3f bits per byte\n", bigramEntropyPerByte);
    printf("Difference (zero-order - bigram): %.3f bits\n\n", shannonEntropy - bigramEntropyPerByte);

    long followOnByte[256] = {0};
    long count = 0;

    for (long d = 0; d < fileSize - 1; d++) 
    {
        if (buffer[d] == (unsigned char)targetByte) 
        {
            followOnByte[buffer[d+1]]++;
            count++;
        }
    }

    printf("Total 0x%02X occurrences: %ld\n", targetByte, count);

    if (count > 0) 
    {
        printf("Following Byte | Count | Percentage\n");
        printf("------------------------------------\n");
        for (int e = 0; e < 256; e++) 
        {
            if (followOnByte[e] > 0) 
            {
                double percent = (double)followOnByte[e] * 100.0 / count;
                printf("     0x%02X     | %6ld | %6.2f%%\n", e, followOnByte[e], percent);
            }
        }
    } 
    else 
    {
        printf("No 0x%02X bytes found.\n", targetByte);
    }

    free(buffer);
    return 0;
}