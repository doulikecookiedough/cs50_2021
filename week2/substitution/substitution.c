#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // Check for single argument
    if (argc == 2)
    {
        int keyCheckStatus = 0;
        string cipherKeyCheck = argv[1];

        // Check that it is a group of letters
        for (int i = 0, len = strlen(cipherKeyCheck); i < len; i++)
        {
            if (isdigit(cipherKeyCheck[i]))
            {
                keyCheckStatus += 1;
            }
        }

        // Check for repeated letter
        for (int i = 0, len = strlen(cipherKeyCheck); i < len - 1; i++)
        {
            for (int j = i + 1; j < len; j++) // Begin with
            {
                if (cipherKeyCheck[i] == cipherKeyCheck[j])
                {
                    keyCheckStatus += 1;
                }
            }
        }

        // Check for key length = 26
        int keyLength = strlen(argv[1]);
        if (keyLength != 26) {
            keyCheckStatus += 1;
        }

        if (keyCheckStatus == 0)
        {
            // Place argument into variable
            string cipherKey = argv[1];

            // Get user plaintext
            string plainText = get_string("plaintext: ");

            // Print the cypher static text
            printf("ciphertext: ");

            // Process and print each plainText character
            for (int i = 0, len = strlen(plainText); i < len; i++)
            {
                if (plainText[i] >= 'a' && plainText[i] <= 'z')
                {
                    // Calculate the corresponding array value
                    // - 'a' = 97, 'z' = 112
                    int indexValue = 25 - ('z' - plainText[i]); // 25 - 15 (112 - 97) = 10
                    char cipheredValue = cipherKey[indexValue];
                    printf("%c", tolower(cipheredValue));
                }
                else if (plainText[i] >= 'A' && plainText[i] <= 'Z')
                {
                    // Calculate the corresponding array value
                    // - 'A' = 65, 'Z' = 90
                    int indexValue = 25 - ('Z' - plainText[i]);
                    char cipheredValue = cipherKey[indexValue];
                    printf("%c", toupper(cipheredValue));
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
            printf("Invalid key, please check that you do not use numbers, do not have duplicates and/or you have 26 characters in your key.\n"); // Error: Multiple arguments
            // End program by returning 1 (error)
            return 1;
        }
    }
    else
    {
        printf("Usage: ./substitution key\n"); // Error: Multiple arguments
        // End program by returning 1 (error)
        return 1;
    }
}