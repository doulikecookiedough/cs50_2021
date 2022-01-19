// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r"); // Input represents the name of the original audio file
    // *input ~ pointer to the opened file, opening them in a "read" format
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w"); // Output represents the name of the new audio file that should be generated
    // *output ~ pointer to a new file, opening it in a "write" mode
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]); // Factor is the amount by which the volume of the original audio file should be scaled

    // Copy over the header
    uint8_t header[HEADER_SIZE];
    fread(&header, sizeof(header), 1, input);
        // ptr: &header ~ address of the first byte of memoery into which to read the data
        // size: sizeof(header) ~ size in bytes of the type of data to read, the header is 44 bytes
        // nmemb: 1 ~ the number of ^ to read at once
        // stream: input ~ pointer to a file returned by fopen
    fwrite(&header, sizeof(header), 1, output); // Write it to the header

    // Now copy over the samples
    int16_t buffer;
    while (fread(&buffer, sizeof(buffer), 1, input))
    {
        buffer *= factor;
        fwrite(&buffer, sizeof(buffer), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
