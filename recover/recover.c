#include <cs50.h> // argc, argv[]
#include <stdio.h> //fprintf(), fread(), fopen(), fwrite()
// #include <stdlib.h>

int main(int argc, char *argv[]) //argc --> cmd line arg, argv[] is the array which is 0 indexed
{
    // the program must be executed with exactly 2 cmd line args --> the file name and the raw file
    if (argc != 2) // so if the argc is not those 2
    {
        fprintf(stderr, "Not proper usage, correct usage is: ./recover imagefile.raw\n"); //inform user of correct usage
        return 1; //1 for error
    }

    // with correct input set char *infile equal to the start of the file card.raw

    // *infile is now = to argv[1], starting point of card.raw
    char *infile = argv[1];

    // FILE *inptr uses the *infile pointer as a starting point to open the entire card.raw file for reading
    // "The *fopen\*() function opens the file whose name is the string pointed to by path and associates a stream with it."
    FILE *inptr = fopen(infile, "r");

    // if the card.raw file cannot be opened (unable to be read per permissions), it is NULL
    if (inptr == NULL)
    {
        fprintf(stderr, "Error: could not open %s due to file restrictions \n",
                infile); //error message telling user what file could not be opened
        return 2; //2 for 2nd possible error
    }

    // initialize a buffer that is equal to 512 bytes, (cameras write only in sets of 512 Bytes, so we only handle reading sizes of 512 Bytes of less)
    // the buffer is used to hold the file info as we read it
    unsigned char buffer[512];

    // allocate space for holding a file name that will not exceed 9 chars
    char imageName[9]; //nameHolder

    // set the 8th char in the tempName equal to the end char '\0'
    imageName[8] = '\0';

    // use FILE pointer to set inital value of *img to NULL
    FILE *img = NULL;

    // the jpeg counter, will be used in process of giving individual names to jpeg files
    // also used to navigate through logic until image retrieval process complete
    int jpegCounter = 0;

    // determine if the input is in 512 byte blocks before proceeding
    while (fread(buffer, 1, 512, inptr) == 512)
    {
        // if input is in 512 byte blocks per above
        // check if the first 4 bytes in the buffer are = to the start of a jpeg file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (jpegCounter == 0) // which it will on the first run through this loop
            {
                // give each jpeg file a unique name - the file name is a string so used sprintf() is used
                sprintf(imageName, "%03i.jpg", jpegCounter);

                //open the newly named jpeg image file for writing and set = to the ptr, img
                img = fopen(imageName, "w");

                // write the image file contents from the pointer img
                fwrite(buffer, 1, 512,
                        img); // fwrite() writes elements of data, each size bytes long, to the stream pointed to by stream, obtaining them from the location given by pointer

                //increase the image counter by 1
                jpegCounter++;
            }
            else // when jpegName does not == 0, so everything after 0
            {
                fclose(img); // flush the current image file at the img pointer
                sprintf(imageName, "%03i.jpg", jpegCounter); // print unique file name string
                img = fopen(imageName, "w"); // open the newly named jpeg file with its unique name
                fwrite(buffer, 1, 512, img); //write the image contents
                jpegCounter++; // increase the image counter by 1
            }
        }
        else
        {
            if (img != NULL) // as long as the img file is not NULL
            {
                fwrite(buffer, 1, 512, img); // write the image contents
            }
        }
    };
    // fclose() only flushes the user-space buffers provided by the C library
    fclose(img); // flush the *img pointer
    return 0;
}

