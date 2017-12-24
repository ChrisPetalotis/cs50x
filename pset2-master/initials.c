/**
*initials.c
*
*Christos Petalotis
*
*Computer Science 50
*Problem set 2
*
*Prompts for user's name and outputs the initials in uppercase with no space or period.
*/

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // Prompt user for name
    string name = GetString();
    
    if (name != NULL)
    {
        // Print out the first initial
        printf("%c", toupper(name[0]));
    
        // Print out any other initial 
        for(int i = 0, j = strlen(name); i < j; i++)
        {
            if(name[i] == ' ')
            {
                printf("%c", toupper(name[i + 1]));
            }
        }
    }
    // Print new line
    printf("\n");
    
    return 0;
}   