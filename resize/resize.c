// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4) //program + 3 cmd line args
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    int resize = atoi(argv[1]); //assign the int (to multiply by) to enlarge
    char *infile = argv[2];
    char *outfile = argv[3];

    // error checking for resize
    if (resize < 1 || resize > 100)
    {
        fprintf(stderr, "Resize needs to be in the range of 1 to 100");
        return 1;
    }

    // open input file supplied by cmd line arg --> unchanged
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file supplied by cmd line arg --> unchanged
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER --> unchanged
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER --> unchanged
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format. Only accept 24-bit BMP\n");
        return 4;
    }

    int startingWidth = bi.biWidth; //assign starting img width to an int var
    int startingHeight = bi.biHeight; //assign starting img height to an int var

    // multiply width and height times the resize int
    bi.biWidth *= resize; // (image) width = width * resize
    bi.biHeight *= resize; // (image) height = width * resize

    // use given input to decide the padding for each scanline
    int startingPadding = (4 - (startingWidth * sizeof(RGBTRIPLE)) % 4) % 4; //input image, starting
    int newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4; //output image, new

    // update image size --> unchanged
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + newPadding) * abs(bi.biHeight);

    // update new file size, add both headers to the size of the actual image file contents --> unchanged
    bf.bfSize  = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER --> unchanged
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER --> unchanged
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    //need to temporarily allocate storage to hold the scanlines in RGB triple * image width
    //"malloc() function allocates size bytes" (the product of RGBTRIPlE and the image width) "and returns a pointer to the allocated memory"
    RGBTRIPLE *buffer = malloc(sizeof(RGBTRIPLE) * (bi.biWidth));

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(startingHeight); i < biHeight; i++)
    {
        int tracker = 0; // an int to hold the place when iterating through buffer for loop

        // iterate over pixels in scanline
        for (int j = 0; j < startingWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write pixel to buffer the number of times to resize by
            for (int k = 0; k < resize; k++)
            {
                *(buffer + (tracker)) = triple; //set the product = to the RGBTRIPLE temp storage
                tracker++; //keep going through until k no longer is < resize
            }
        }

        // skip over padding, if any by moving the 'cursor'
        fseek(inptr, startingPadding, SEEK_CUR);

        // write RGB triple to outfile
        for (int l = 0; l < resize; l++)
        {
            fwrite((buffer), sizeof(RGBTRIPLE), bi.biWidth, outptr);

            // write padding to outfile
            for (int k = 0; k < newPadding; k++) // --> unchanged
            {
                fputc(0x00, outptr);
            }
        }
    }

    // free memory from the buffer now that process is complete
    // free() comes from stdlib.h "allocate and free dynamic memory"
    free(buffer);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
