#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int main(void)
{

    string text = get_string("Text: ");

    // Calculate letters
    int letters = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isalpha(text[i])) // Checks whether a number is a letter or not
        {
            letters++;
        }

    }

    // Calculate words
    int words = 1; // Test below always ignores the first word
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if ((text[i]) == ' ') // Checks whether a number is a space
        {
            words++;
        }

    }

    // Calculate sentences
    int sentences = 0; // Test below always ignores the first word
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if ((text[i]) == '.')
        {
            sentences++;
        }
        if ((text[i]) == '!')
        {
            sentences++;
        }
        if ((text[i]) == '?')
        {
            sentences++;
        }
    }

    // Calculate grade level
    float getAverageL = (100 * letters) / words;
    float getAverageS = (100 * sentences) / words;
    float processAverageL = 0.0588 * getAverageL;
    float processAverageS = 0.296 * getAverageS;
    float gradeIndex = processAverageL - processAverageS - 15.8;
    int temporaryIndex = (int)(gradeIndex * 100);
    int decimalPoint = temporaryIndex % 100;

    // Process index for precise grading
    float processedIndex = gradeIndex;

    if (decimalPoint < 55)
    {
        processedIndex = floor(gradeIndex);
    }
    else
    {
        processedIndex = round(gradeIndex);
    }

    // Output grade level
    if (processedIndex < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (processedIndex >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.0f\n", processedIndex);
    }
}