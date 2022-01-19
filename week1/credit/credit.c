#include <cs50.h>
#include <stdio.h>
#include <math.h>

// This is CS50 Credit

int main(void)
{
// Create variable for checksum lateron
    int finalseconddig, finalfourthdig, finalsixdig, finaleightdig, finaltendig, finaltwedig, finalfourdig, finalsixthdig;
    long credit;
//Set the credit type to Invalid by default
    string type = "INVALID";
    do
    {
        credit = get_long("Validate your credit card number? ");
    }
// If the credit card is more than 16 digits, keep asking
    while (credit > 9999999999999999);
// Multiply every other digit by 2 started with the second last digit
// Also grab the digits of the numbers not needing to be multiplied by 2
    int firstdig = credit % 10;
    int seconddig = ((credit % 100) / 10) * 2;
    int thirddig = (credit % 1000) / 100;
    int fourthdig = ((credit % 10000) / 1000) * 2;
    int fifthdig = (credit % 100000) / 10000;
    int sixdig = ((credit % 1000000) / 100000) * 2;
    int sevendig = (credit % 10000000) / 1000000;
    int eightdig = ((credit % 100000000) / 10000000) * 2;
    int ninedig = (credit % 1000000000) / 100000000;
    int tendig = ((credit % 10000000000) / 1000000000) * 2;
    int eledig = (credit % 100000000000) / 10000000000;
    int twedig = ((credit % 1000000000000) / 100000000000) * 2;
    int thidig = (credit % 10000000000000) / 1000000000000;
    int fourdig = ((credit % 100000000000000) / 10000000000000) * 2;
    int typefourdig = (credit % 100000000000000) / 10000000000000;
    int fiftdig = (credit % 1000000000000000) / 100000000000000;
    int typefiftdig = (credit % 1000000000000000) / 100000000000000;
    int sixthdig = ((credit % 10000000000000000) / 1000000000000000) * 2;
    int typesixdig = (credit % 10000000000000000) / 1000000000000000;
// Get the final number for when the digit becomes a double digit (fixing the final data to use)
// If the product is greater than 9, do 1 + the last digit (remainder) of the product automatically
    // Start with the second digit from the right
    if (seconddig > 9)
    {
        finalseconddig = 1 + (seconddig % 10);
        // Print Check
        //printf("%i\n", finalseconddig);
    }
    else
    {
        finalseconddig = seconddig;
        // Print Check
        //printf("%i\n", finalseconddig);
    }
    // Now the fourth digit from the right
    if (fourthdig > 9)
    {
        finalfourthdig = 1 + (fourthdig % 10);
        // Print Check
        //printf("%i\n", finalfourthdig);
    }
    else
    {
        finalfourthdig = fourthdig;
        // Print Check
        //printf("%i\n", finalfourthdig);
    }
    // Now the sixth digit from the right
    if (sixdig > 9)
    {
        finalsixdig = 1 + (sixdig % 10);
        // Print Check
        //printf("%i\n", finalsixdig);
    }
    else
    {
        finalsixdig = sixdig;
        // Print Check
        //printf("%i\n", finalsixdig);
    }
    // Now the eigth digit from the right
    if (eightdig > 9)
    {
        finaleightdig = 1 + (eightdig % 10);
        // Print Check
        //printf("%i\n", finaleightdig);
    }
    else
    {
        finaleightdig = eightdig;
        // Print Check
        //printf("%i\n", finaleightdig);
    }
    // Now the tenth digit from the right
    if (tendig > 9)
    {
        finaltendig = 1 + (tendig % 10);
        // Print Check
        //printf("%i\n", finaltendig);
    }
    else
    {
        finaltendig = tendig;
        // Print Check
        //printf("%i\n", finaltendig);
    }
    // Now the 12th digit from the right
    if (twedig > 9)
    {
        finaltwedig = 1 + (twedig % 10);
        // Print Check
        //printf("%i\n", finaltwedig);
    }
    else
    {
        finaltwedig = twedig;
        // Print Check
        //printf("%i\n", finaltwedig);
    }
    // Now the 14th digit from the right
    if (fourdig > 9)
    {
        finalfourdig = 1 + (fourdig % 10);
        // Print Check
        //printf("%i\n", finalfourdig);
    }
    else
    {
        finalfourdig = fourdig;
        // Print Check
        //printf("%i\n", finalfourdig);
    }
    // Now the 16th digit from the right
    if (sixthdig > 9)
    {
        finalsixthdig = 1 + (sixthdig % 10);
        // Print Check
        //printf("%i\n", finalsixthdig);
    }
    else
    {
        finalsixthdig = sixthdig;
        // Print Check
        //printf("%i\n", finalsixthdig);
    }
// Now add all the fixed numbers together to get part 1
    int checksum = finalseconddig + finalfourthdig + finalsixdig + finaleightdig + finaltendig + finaltwedig + finalfourdig + finalsixthdig;
    // Print check to see the numbers, comment out when not in use
    //printf("%i,%i,%i,%i,%i,%i,%i,%i\n", finalseconddig, finalfourthdig, finalsixdig, finaleightdig, finaltendig, finaltwedig, finalfourdig, finalsixthdig);
// Now add all the numbers we ignored starting from the very last
    int checksumpart2 = firstdig + thirddig + fifthdig + sevendig + ninedig + eledig + thidig + fiftdig;
    // Print check to see the numbers, comment out when not in use
    //printf("%i,%i,%i,%i,%i,%i,%i,%i\n", firstdig, thirddig, fifthdig, sevendig, ninedig, eledig, thidig, fiftdig);
    int checktotal = checksum + checksumpart2;
    // Print check to see final checktotal
    //printf("%i\n", checktotal);
// Now we do the check to get AMEX (34 or 37), VISA (4), MASTERCARD (51, 52, 53, 54, or 55)
// Only change the type to AMEX/VISA/MASTERCARD when the checktotal remainder is 0 (aka < 1)
    if (checktotal % 10 < 1)
    {
    // For AMEX (Sum of first two digits should be 7 or 10)
        if (typefiftdig + typefourdig == 10)
        {
            type = "AMEX";
        }
        if (typefiftdig + typefourdig == 7)
        {
            type = "AMEX";
        }
    // For VISA (16 Digits)
        if (typesixdig == 4)
        {
            type = "VISA";
        }
    // For VISA (13 Digits)
        if (thidig == 4)
        {
            type = "VISA";
        }
    // For MASTERCARD (16 Digits)
        if (typesixdig == 5 && ((typefiftdig + typesixdig) < 11 && (typefiftdig + typesixdig) > 5))
        {
            type = "MASTERCARD";
        }
    // For MASTERCARD (15 Digits)
        if (typefiftdig == 5 && ((typefiftdig + typefourdig) < 11 && (typefiftdig + typefourdig) > 5))
        {
            type = "MASTERCARD";
        }
        printf("%s\n", type);
        // printf("15 Digit is %i\n", typefiftdig);
        // printf("14 Digit is %i\n", typefourdig);
        // printf("16 Digit is %i\n", typesixdig);
    }
    else
    {
        printf("INVALID\n");
    }
}