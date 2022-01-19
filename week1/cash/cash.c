#include <cs50.h>
#include <stdio.h>
#include <math.h>

// This is mario more

int main(void)
{
    float n;
    do
    {
        n = get_float("How many coins will you need? ");
    }
    while (n <= 0);
    int wholenumber = round(n * 100); // Create a whole number
    int quarters = wholenumber / 25; // Divide input by 25 it can produce
    int dimes = (wholenumber % 25) / 10; // Divide by 10 to get number of dimes it can produce
    int nickels = ((wholenumber % 25) % 10) / 5; // Divide by 5 to get number of nickels
    int pennies = (((wholenumber % 25) % 10) % 5) / 1; // Divide the remainder by 1 to get pennies
    int total = quarters + dimes + nickels + pennies;
    printf("%i\n", total);
}



// Ask the user how much changed is owed in coins
// Take input and then divide input first by 0.25, count the number of times
// Take remainder and divide by 0.10, count the number of times it works
// Take remainder and divide by 0.05, count the number of times it works
// Take remainder and divide by 0.01, count the number of tmes it works
// Sum up the amount of times it took to reach 0, and then print the number of times