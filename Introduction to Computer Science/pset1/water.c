/**
*water.c
*
*Christos Petalotis
*
*Computer Science 50
*Problem Set 1
*
*Computes the amount of bottles of water used for user's shower
*/

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //prompt user for length of shower in minutes
    printf("minutes: ");
    //gets user's input     
    int minutes = GetInt();
    //prints the equivalent number of bottles of water 
    printf("bottles:%d \n", minutes * 12);

    return 0;
}
