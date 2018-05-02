#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) //argc --> cmd line arg, argv[] is the array --> 0 indexed
{
    // error trap for cmd line args
    // the program must be executed with exactly 1 cmd line arg, the file name
    if (argc != 2)
    {
        frpintf(stderr, "Not proper usage, correct use is: ./recover file.raw\n"); //inform user of correct usage
        return 1; //1 for error
    }
    char *infile = argv[1]; //set the infile equal to card.raw

    // "The *fopen\*() function opens the file whose name is the string pointed to by path and associates a stream with it."
    FILE *inptr = fopen(infile, "r"); // see if the infile, argv[1] is capable of being read --> include stdio.h

    // error trap for infile that cannot be opened
    if (inptr == NULL) //NULL meaning not a valid file
    {
        fprintf(stderr, "Error: could not open %s.\n, infile "); //error message telling user what file could not be opened
        return 2; //2 for 2nd possible error
    }

    unsigned char buffer[512];
    char tempName[8];
    tempName[7] = '\0';
    FILE *img = NULL;
    int jpegName = 0;




}