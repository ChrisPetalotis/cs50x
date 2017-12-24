/**
*vigenere.c
*
*Christos Petalotis
*
*Computer Science 50
*Problem Set 2
*
*Prompts user for a string and then encipher it using Vigenère cipher.
*/

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc,string argv[])
{
    //make sure the user inputed only 1 command-line argument
    if(argc != 2)
    {
        printf("Usage: ./caesar keywork\n");
        return 1;
    }
    
    string k = argv[1];
    
    //check that keyword includes only alphabetical characters
    for(int i = 0, n = strlen(k); i < n; i++)
    {
        if(!isalpha(k[i]))
        {
            printf("You must input only alphabetical characters\n");
            return 1;
        }
    }
    
    //Get input from the user
    string p = GetString();
    
    if(p != NULL)
    { 
        //Convert to ciphertext
        for(int i = 0, j = 0, n = strlen(p), h = strlen(k); i < n; i++, j++)
        { 
            if(isalpha(p[i]))
            {   
                //Wrap the keyword index around back to the begining of the keyword
                j = j % h;
                
                //Convert from ASCII to alphabetical index for capital letters in the plaintext
                if(isupper(p[i]))
                {
                    if(isupper(k[j]))
                    p[i] = ((p[i] - 'A' + k[j] - 'A') % 26) + 'A';
                    else
                    p[i] = ((p[i] - 'A' + k[j] - 'a') % 26) + 'A';
                }
                //Convert from ASCII to alphabetical index for lower case letters in the plaintext
                else
                {   
                    if(isupper(k[j]))
                    p[i] = ((p[i] - 'a' + k[j] - 'A') % 26) + 'a';
                    else
                    p[i] = ((p[i] - 'a' + k[j] - 'a') % 26) + 'a';
                }    
            }
            //Do not apply the shift to non alphabetical characters
            else
            {
                j--;
            }
            
        }
        //Return ciphertext
        printf("%s\n", p);
    }
    
    return 0;
    
}