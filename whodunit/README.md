# Questions

## What's `stdint.h`?

A library for that gives our program the ability to receive input.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

Those are C equivalants of Windows data types - the struct uses this information so the bytes with those names can be retrieved from memory more easily.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

BYTE - 1 byte
DWORD - 4 bytes
LONG - 4 bytes
WORD - 2 bytes

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

The first two bytes of a BMP file must be the type and size of the BMP file.

## What's the difference between `bfSize` and `biSize`?

For bfType, the data type is WORD
It tells the file type to expect; it must be BM.

For biSize, the data type is DWORD
It tells the number of bytes required by the structure

## What does it mean if `biHeight` is negative?

"If biHeight is negative, indicating a top-down DIB, biCompression must be either BI_RGB or BI_BITFIELDS. Top-down DIBs cannot be compressed."

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount specifies the bits per pixel of the BMP's color depth

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

fopen could return NULL if the function does not return a file pointer.

## Why is the third argument to `fread` always `1` in our code?

"On success, fread() returns the number of items read or written.
This number equals the number of bytes transferred only when size is 1.
If an error occurs, or the end of the file is reached, the return value is a short item count (or zero)."

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

It will return the value of the expression based on what the size of RGBTRIPLE is equal to.

## What does `fseek` do?

fseek tells the program to skip over the padding, if there is any padding.
If there is padding, the code will continue through fputc

## What is `SEEK_CUR`?

Per the documentation for fseek, SEEK-CUR is the current position indicator,
and by using SEEK-CUR as an argument, the the offset will be relative the current position.