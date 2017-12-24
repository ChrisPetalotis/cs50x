/**
 * resize.c
 *
 * Christos Petalotis
 * 
 * Computer Science 50
 * Problem Set 4
 *
 * Resizes a bmp file by a given by the user number 
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    //convert insterted factor from string to integer
    int n = atoi(argv[1]);

    if(n < 1 || n > 100)
    {
        printf("'n' must be a positive number less than or equal to 100\n");
        return 2;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }
    
    // Determin new height
    int Old_Height = bi.biHeight;
    int New_Height = Old_Height * n;
    bi.biHeight = New_Height;
    
    // Determine new width
    int Old_Width = bi.biWidth;
    int New_Width = Old_Width * n;
    bi.biWidth = New_Width;
    
    // determine padding for infile's scanliness
    int padding =  (4 - (Old_Width * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // padding for the outfile
    int new_padding =  (4 - (New_Width * sizeof(RGBTRIPLE)) % 4) % 4;
    
    //change size of image
    bi.biSizeImage = ((New_Width * sizeof(RGBTRIPLE) + new_padding)) * abs(New_Height);

    // change size of bmp
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(Old_Height); i < biHeight; ++i)
    {   
        // resize vertically - add each scanline to outfile n times
        for(int m = 0; m < n; m++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < Old_Width; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
                // write RGB triple to outfile n times
                for(int k = 0; k < n; k++)
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            
            // go to the start of the line in order to resize verticaly
            if(m < n - 1)
                fseek (inptr, ( -((long int)((sizeof(RGBTRIPLE) * Old_Width)))), SEEK_CUR);

            // add padding
            for (int l = 0; l < new_padding; l++)
            {
                fputc(0x00, outptr);
            }
        }
        
        // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
