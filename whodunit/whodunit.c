// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    //CMD LINE ERROR TRAPPING

    // ensure proper usage
    if (argc != 3) //if the argc isn't correct
    {
        fprintf(stderr, "Usage: ./whodunit infile outfile\n");
        return 1;
    }

    // remember filenames/assign filenames to a char*
    char *infile = argv[1];
    char *outfile = argv[2];

    // 1.) open input file for reading
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL) // if infile cannot be opened for reading
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // 2.) open output file for writing
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL) // if outfile cannot be opened for writing
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // the input + output files are now open
    // 3.) read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // 4.) read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // if the input file is not a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr); // close outfile
        fclose(inptr); // close infile
        fprintf(stderr, "Unsupported file format.\n"); //tell the user
        return 4;
    }

    // if the input file is correct, (like it should be in this example!)
    // 5.) write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // 6.) write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // 7.) determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // 8.) iterate over infile's scanlines first
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // 9.) then iterate over pixels in the scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // 10.) read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            //THIS IS WHERE TO ADD CODE
            //change the pixels to a color that lets you read the hidden message
            // if the red bye of the pixel is == to red
            if (triple.rgbtRed == 0xff)
           // turn both green and blue to white
            {
                triple.rgbtGreen = 0x00;
                triple.rgbtBlue = 0x00;
            }
            //let the red message shine through
            //all pixels that have red stay that way
            //blue and green get removed

            // 11.) write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // 12.) skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // 13.) then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // 14.) close infile
    fclose(inptr);

    // 15.) close outfile
    fclose(outptr);

    // if made it to this point, return 0 for success as the file has been copied (with changes)
    return 0;
}
