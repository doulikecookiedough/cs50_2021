#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // Check for single argument and that it is a digit
    if (argc == 2 && isdigit(*argv[1]))
    {
        // Check to ensure key is valid
        if 
        
        // Convert key to integer
        int userKey = atoi(argv[1]);

        // Get user plaintext
        string plainText = get_string("plaintext: ");
        // Print the cypher static text
        printf("ciphertext: ");

        // Process and print each plainText character
        for (int i = 0, len = strlen(plainText); i < len; i++)
        {
            if (plainText[i] >= 'a' && plainText[i] <= 'z')
            {
                // If the character is lower case
                // - 'a' = 97
                // - 'z' = 112
                char cipheredValue = (((plainText[i] - 'a') + userKey) % 26) + 'a';
                printf("%c", cipheredValue);
            }
            else if (plainText[i] >= 'A' && plainText[i] <= 'Z')
            {
                // If character is upper case
                // - 'A' = 65
                // - 'Z' = 90
                char cipheredValue = (((plainText[i] - 'A') + userKey) % 26) + 'A';
                printf("%c", cipheredValue);
            }
            else
            {
                // Do not process a special character
                printf("%c", plainText[i]);
            }

        }
        printf("\n");
        // Exit program by returning 0 (success)
        return 0;
    }
    else
    {
        // Provide instructions if incorrect key passed
        printf("Usage: ./caesar key\n");
        // End program by returning 1 (error)
        return 1;
    }
}