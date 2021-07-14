/**
*caesar.c
*
*Christos Petalotis
*
*Computer Science 50
*Problem Set 2
*
*Prompts user for a string and then encipher it using Caesar cipher.
*/

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc,string argv[])
{
    //make sure the user inputed only 1 command-line argument
    if(argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    
    //convert inserted key from string to integer
    int k = atoi(argv[1]);
    
     //check that key >= 0
    if(k < 0)
    {
        printf("key must be a non-negative number\n");
        return 1;
    }
    
    //Get input from the user 
    string p = GetString();
    
    if(p != NULL)
    {
        //Convert to ciphertext
        for(int i = 0, n = strlen(p); i < n; i++)
        {
            if(isalpha(p[i]))
            {   
                //Convert from ASCII to alphabetical index for capital letters
                if(isupper(p[i]))
                {
                    p[i] = ((p[i] - 'A' + k % 26) % 26) + 'A';
                }
                //Convert from ASCII to alphabetical index for lower case letters
                else
                {
                    p[i] =((p[i] - 'a' + k % 26) % 26) + 'a';
                }
                    
            }
           
        }
        //Return ciphertext
        printf("%s\n", p);
    }    
        
        return 0;
        
}