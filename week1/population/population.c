#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for start size
    int sizeStarting = 0;
    do
    {
        sizeStarting = get_int("What is the starting size (Min. 9)?");
    }
    while (sizeStarting < 9);

    /// Prompt for end size
    int sizeEnding = 0;
    do
    {
        sizeEnding = get_int("What is the ending size?");
    }
    while (sizeEnding < sizeStarting);

    // Calculate number of years until we reach threshold
    int yearsAccumulated = 0;
    int llamasRequired = sizeEnding - sizeStarting;

    while (sizeStarting < sizeEnding)
    {
        sizeStarting = sizeStarting + (sizeStarting / 3) - (sizeStarting / 4);
        yearsAccumulated++;
    }

    // TODO: Print number of years
    printf("Years: %d\n", yearsAccumulated);
}