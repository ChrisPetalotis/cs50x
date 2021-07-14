/**
*greedy.c
*
*Christos Petalotis
*
*Computer Science 50
*Problem set 1
*
*Prompts user for an amount of change and returns the  minimum number of coins needed
*/

#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    float change;
    
    //Prompt user for the amount of change that is owed
    do
    {
        printf("How much change is owed?\n");
        change = GetFloat();
    }
    while(change < 0);
    
    //Convert dollars value to cents
    change = change * 100;
    
    //Round the number given
    change = round(change);

    //Declare coin counter 
    int c;
    int f = change;
    
    // Count the quarters
    c = f / 25;
    f = f % 25;
    
     //Count the dimes 
    c = c + f / 10;
    f = f % 10;

    //Count the pennies
    c = c + f / 5;
    f = f % 5;
    
    //Count the cents
    c = c + f / 1; 
    f = f % 1;
    
    //Print number of coins
    printf("%d\n", c);
    
    return 0;
}