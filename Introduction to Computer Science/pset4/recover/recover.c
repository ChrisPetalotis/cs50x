/**
 * recover.c
 *
 * Christos Petalotis
 * 
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;

int main(int argc, char* argv[]) 
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./recover card.raw\n");
        return 1;
    }
    
    //open memory card file
    FILE* card = fopen(argv[1], "r");
    if (card == NULL)
    {
        fprintf(stderr, "Could not open card.\n");
        return 2;
    }
    
    //go to the beginning of the file
    fseek(card, 0, SEEK_SET);
    
    // declare buffer
    BYTE buffer[512];
    
    //create an array of files
    FILE* img = NULL;
    
    //initialize counter of images created
    int c = 0;
    
    //repeat until the end of the card
    while(fread(buffer, sizeof(buffer), 1, card) != 0)
    {
    //compare buffer to the needed sequences of bytes to determine if it is the beginning of a new jpg
    if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3] >= 0xe0 && buffer[3] <= 0xef)
    {   
        //close current jpg file (if there is one open)
        if(c > 0)
        fclose(img);
        
        //make a char array
        char jpg[8];
          
        //create a new jpg file in the current directory 
        sprintf(jpg, "%03i.jpg", c);
        
        //open jpg file
        img = fopen(jpg, "w");
        if (img == NULL)
            {
                printf("Could not create %s\n", jpg);
                return 2;
            }
        
        //write the first 512 bytes to the new image
        fwrite(buffer, sizeof(buffer), 1, img);
        //update counter 
        c++;  
    }
    else if(c > 0)
    //write into the previous opened jpg
    fwrite(buffer, sizeof(buffer), 1, img);
    }    
    
    //at the end of the file(card) close any remaining files
    fclose(img);
    fclose(card);
    
    return 0;
}
