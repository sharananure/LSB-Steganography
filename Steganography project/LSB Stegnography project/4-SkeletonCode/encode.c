/*
Name : Sharan
Date : 20/10/2024
Description : Implementation of LSB image Steganography project
*/

#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include <string.h>

/* Function Definitions */

/* 
 * Get image size for BMP 
 * Input: Image file pointer
 * Output: Returns the size of the image (width * height * 3)
 * Description: Reads the width and height from the BMP header (at offset 18 and 22) 
 * and calculates the total image size in bytes.
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte in BMP to read width
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image size in bytes (width * height * 3 for 24-bit image)
    return width * height * 3;
}

Status open_files(EncodeInfo *encInfo)
{
    // Open source image in binary read mode
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
    	return e_failure;
    }
    else
    {
        printf("INFO : %s file open\n", encInfo->src_image_fname);
    }

    // Open secret file in read mode
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
    	return e_failure;
    }
    else
    {
        printf("INFO : %s file open\n", encInfo->secret_fname);
    }

    // Open stego image in binary write mode
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
    	return e_failure;
    }
    else
    {
        printf("INFO : %s file open\n", encInfo->stego_image_fname);
    }

    return e_success;
}


Status do_encoding(EncodeInfo *encInfo)                 
{
    printf("INFO: Rewinding the source image file for encoding.\n");    
    rewind(encInfo->fptr_src_image);                                    // Rewind the source image file to the beginning

    printf("INFO: Starting to copy the BMP header.\n");
    copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image);    // Copy the BMP header (first 54 bytes) to the stego image
    printf("INFO: BMP header copied successfully.\n");

    printf("INFO: Encoding the Magic String signature.\n");
    encode_magic_string(encInfo->magic_string, encInfo);                // Encode the magic string 
    printf("INFO: Magic String encoded successfully.\n");

    printf("INFO: Encoding the secret file extension size.\n");
    encode_secret_file_extn(encInfo);                                   // Encode the secret file extension into stego image
    printf("INFO: Secret file extension encoded successfully.\n");

    printf("INFO: Encoding the secret file data.\n");
    encode_secret_file_data(encInfo);                                   // Encode the secret file data
    printf("INFO: Secret file data encoded successfully.\n");

    
    printf("INFO: Copying the remaining image data after encoding.\n");
    copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image);        // Copy any remaining data from the source image to the stego image
    printf("INFO: Remaining image data copied successfully.\n");

    return e_success;
}

uint get_file_size(FILE *fptr)  // get the size of the file
{
    fseek(fptr, 0, SEEK_END);  
    return ftell(fptr);  
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)     // Copy 54 bytes of BMP header 
{
    unsigned char ch[54];  
    rewind(fptr_src_image);  
    fread(ch, 54, 1, fptr_src_image);  
    fwrite(ch, 54, 1, fptr_dest_image);  

    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    int len = strlen(magic_string);
    encode_length(len, encInfo);  // Encode length 
    encode_string(len, magic_string, encInfo);  // Encode the string
    return e_success;
}

Status encode_length(int len, EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer, 32, 1, encInfo->fptr_src_image);  // Read 32 bytes from source image

    // Modify the LSB of each byte to store the length
    for (int i = 0; i < 32; i++)
    {
        buffer[i] = (~1) & buffer[i];  // Clear LSB
        if ((1 << i) & len)
        {
            buffer[i] = buffer[i] | 1;  // Set LSB if needed
        }
    }

    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);  // Write modified bytes to stego image
    return e_success;
}

Status encode_string(int len, const char *str, EncodeInfo *encInfo)
{
    char buffer[8 * len];
    fread(buffer, 8 * len, 1, encInfo->fptr_src_image);  // Read 8 * length bytes from source

    int k = 0;
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            buffer[k] = buffer[k] & (~1);  // Clear LSB
            if ((1 << j) & str[i])
            {
                buffer[k] = buffer[k] | 1;  // Set LSB if needed
            }
            k++;
        }
    }

    fwrite(buffer, 8 * len, 1, encInfo->fptr_stego_image);  // Write to stego image
    return e_success;
}

int secret_file_extn_len(EncodeInfo *encInfo)
{
    int i, size;
    for (i = 0; encInfo->secret_fname[i] != '.'; i++);  // Find the position of the dot
    for (size = 0; encInfo->secret_fname[i + size] != '\0'; size++);  // Calculate the extension length
    return size;
}

Status encode_secret_file_extn(EncodeInfo *encInfo)
{
    int len = secret_file_extn_len(encInfo);  // Get the length of the extension
    encode_length(len, encInfo);  // Encode length 
    int i;
    for ( i = 0; encInfo->secret_fname[i] != '.'; i++);  // Find the starting point of the extension
    encode_string(len, encInfo->secret_fname + i, encInfo);  // Encode the extension
    return e_success;
}


Status encode_secret_file_data(EncodeInfo *encInfo)
{
    int len = get_file_size(encInfo->fptr_secret);  // Get the size of the secret file
    encode_length(len, encInfo);  // Encode the length of the file

    char str[len];
    rewind(encInfo->fptr_secret);  // Rewind to start of secret file

    for (int i = 0; i < len; i++)
    {
        char ch = getc(encInfo->fptr_secret);  // Read each character from secret file
        str[i] = ch;
    }

    encode_string(len, str, encInfo);  // Encode the secret file data
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char buffer;
    int temp;

    temp = fread(&buffer, 1, 1, fptr_src);
    while (temp != 0)
    {
        fwrite(&buffer, 1, 1, fptr_dest);  // Write the byte to the output file
        temp = fread(&buffer, 1, 1, fptr_src);  // Read the next byte
    }
    return e_success;
}

Status check_capacity(char *argv[], EncodeInfo *encInfo)
{
    int magic_string_length; 
    int file_ext_length;
    int secret_file_len; 
    int src_file_length;

    fseek(encInfo->fptr_src_image, 0, SEEK_END);  // Move to the end of the source image
    src_file_length = ftell(encInfo->fptr_src_image);  // Get the size of the source file

    magic_string_length = strlen(MAGIC_STRING);  // Get the length of the magic string

    file_ext_length = secret_file_extn_len(encInfo);  // Get the length of the secret file extension

    fseek(encInfo->fptr_secret, 0, SEEK_END);  // Move to the end of the secret file
    secret_file_len = ftell(encInfo->fptr_secret);  // Get the size of the secret file

    
    if ((src_file_length - 54) < (magic_string_length + file_ext_length + secret_file_len) * 8)  // Check if the source image has enough capacity to hold the encoded data
    {
        return e_failure;  // Fail if the image doesn't have enough space
    }
    return e_success;
}

Status valid_argv_encode(int argc, char *argv[])
{
    if (strcmp(strchr(argv[2], '.'), ".bmp") != 0)      // Validate source image is a .bmp file
    {
        printf("Error :  Pass <.BMP file>\n");
        return e_failure;
    }
    
    if (argv[4] != NULL)                // Validate output image is a .bmp file
    {
        if (strchr(argv[4], '.') != 0)
        {
            if (strcmp(strchr(argv[4], '.'), ".bmp") != 0)
            {
                printf("Error : Pass <.BMP file>\n");
                return e_failure;
            }
        }
        else
        {
            printf("Error : Pass <.BMP file>\n");
            return e_failure;
        }
    }
}
