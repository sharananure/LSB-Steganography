/*
Name : Sharan
Date : 20/10/2024
Description : Implementation of LSB image Steganography project
*/

#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h" // Contains user-defined types

/* 
 * Structure to store information required for
 * decoding secret file from a source Image
 * Info about output and intermediate data is
 * also stored
 */

typedef struct _DecodeInfo
{
    char magic_string[20];

    /* Secret File Info */
    char secret_fname[30];        // Name of the secret file
    FILE *fptr_secret;            // File pointer for the secret file

    int size_ext_file;            // Size of the secret file extension
    long size_secret_file;        // Size of the secret file

    /* Stego Image Info */
    char *stego_image_fname;     // Name of the stego image file
    FILE *fptr_stego_image;      // File pointer for the stego image

} DecodeInfo;

/* Function prototypes */
Status do_decoding(DecodeInfo *decInfo);           // Decode operation

Status open_file(DecodeInfo *decInfo);              // Open file for decoding

Status skip_header(FILE *fptr);                      // Skip BMP header

Status decode_magic_string(DecodeInfo *decInfo);    // Decode the magic string

int decode_len(DecodeInfo *decInfo);                // Decode the length of the data

Status decode_string(int len,char string[],DecodeInfo *decInfo);      //decode the string

Status decode_secret_file_extn(DecodeInfo *decInfo); // Decode the secret file extension

Status decode_secret_file_data(DecodeInfo *decInfo); // Decode the secret file data

Status add_secrate_data_to_file(char str[],DecodeInfo *decInfo);

#endif
