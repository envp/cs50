// Calculates optimal change splitting among
// quarters (25¢), dimes (10¢), nickels (5¢), and pennies (1¢)
#include <stdio.h>
#include "cs50.h"

int main(void)
{
    float change;
    int cents, coins = 0;
    printf("How much change is owed?\n");
    change = GetFloat();

    if(change > 0.00)
    {
        // Type casting seems to be causing some issues
        // 4.2 * 100.00 -> 419, floating point error issues
        // instead of casting / rounding up, switch * 100 for * 1000 / 10
        cents = ((int) (change * 1000)) / 10;
        
        // Could do this with a loop, but we know what all change we have
        // 25c first
        coins += (cents / 25);
        cents = cents % 25;
        
        // 10c
        coins += (cents / 10);
        cents = cents % 10;
        
        // 5c
        coins += (cents / 5);
        cents = cents % 5;

        // 1c
        coins += cents;
        cents = 0;
 
    }
    else
    {
        main();
    }
    printf("%d\n", coins);
    return 0;
}
