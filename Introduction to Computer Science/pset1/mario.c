/**
*mario.c
*
*Christos Petalotis
*
*Computer Science 50
*Problem Set 1
*
*Prompts for pyramid height and returns a half pyramid with the respective height.
*/

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    // User input(0<=height<=23) h= height 
    do
    {
        printf("Height: ");
        height = GetInt();
    }
    while(height < 0 || height > 23);
    
    // Print half-pyramid
    for(int i = 0; i < height; i++)
    {
        // Spaces
        for(int j = 0; j < height - i - 1; j++)
        printf(" ");
        
        // Hashes
        for(int k = 0; k < i + 2 ; k++ )
        printf("#");
        
        // New Line
        printf("\n");
    }

    return 0;    
}