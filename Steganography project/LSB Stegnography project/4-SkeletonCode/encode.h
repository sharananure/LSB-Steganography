/*
Name : Sharan
Date : 20/10/2024
Description : Implementation of LSB image Steganography project
*/

#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;

    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];    
    char magic_string[10];

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;

    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} EncodeInfo;


//validate validate argv in encode
Status valid_argv_encode(int argc,char* argv[]);

/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status check_capacity(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
//Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn(EncodeInfo *encInfo);

int secret_file_extn_len(EncodeInfo *encInfo);

/*Encode len and string*/
Status encode_length(int len,EncodeInfo *encInfo);

Status encode_string(int len,const char *str,EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif
