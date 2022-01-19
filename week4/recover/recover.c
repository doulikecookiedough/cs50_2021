#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Define the BYTE struct
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Confirm proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open memory card provided file in argv[1]
    FILE *input = fopen(argv[1], "r"); // If file doesn't exist in the directory, it will create one
    // *input ~ pointer to the opened file, opening them in a "read" format
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Declare variables to work with
    BYTE buffer[512]; // Chunk to read files in
    char fileName[8]; // Allocate space for string name
    int counter = 0; // Counter to check which file is currently active, starting at 0 as per specifications
    FILE *tempFileStorage = NULL;
    int jpgFlag = 0; // Indicator for program to begin writing

    // Read the file until there is no data
    // - For every block, read 512 bytes into a buffer
    while (fread(&buffer, 512, 1, input) != 0) // fread will return null if there is nothing left to read
    {
        // If the first four bytes match the JPEG signature
        // ~ First Three Bytes: 0xff 0xd8 0xff
        // ~ Last Byte: 0xe0, 0xe1, 0xe2, ... , 0xef
        // ~ Check for the first 4 bits of the last byte, matching 0xe0 (Bitwise Arithmetic)
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (tempFileStorage == NULL) // Check to see whether a Jpeg is open or not
            {
                sprintf(fileName, "%03i.jpg", counter); // Create the fileName
                tempFileStorage = fopen(fileName, "w"); // Open the new file in write mode

                // Increase the counter to confirm a file is currently generating
                counter++;
            }
            else // Close the current file and start a new one
            {
                fclose(tempFileStorage);
                sprintf(fileName, "%03i.jpg", counter);
                tempFileStorage = fopen(fileName, "a");
                counter++;
                jpgFlag = 0;
            }

            // Write to the newly opened file
            fwrite(&buffer, 512, 1, tempFileStorage);

            // Indicate that we are now writing
            jpgFlag = 1;

        }
        else
        {
            // If the file is larger than 512 bits, keep writing
            if (jpgFlag == 1)
            {
                fwrite(&buffer, 512, 1, tempFileStorage);
            }
        }
    }

    // Close all open files at the end
    fclose(input);
    fclose(tempFileStorage);

    // End program
    return 0;
}