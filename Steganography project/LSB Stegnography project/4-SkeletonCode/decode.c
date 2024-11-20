/*
Name : Sharan
Date : 20/10/2024
Description : Implementation of LSB image Steganography project
*/


#include <stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include <string.h>

Status do_decoding(DecodeInfo *decInfo)                 
{
    // Open the necessary files for decoding (stego image and secret output file)
    open_file(decInfo);
    printf("INFO: Opened required files\n");

    // Skip the BMP header (first 54 bytes) in the stego image
    skip_header(decInfo->fptr_stego_image);
    printf("INFO: Skipped BMP header\n");

    // Decode the magic string
    if (decode_magic_string(decInfo) == e_failure)
    {
        return e_failure;  
    }
    printf("INFO: Magic String decoded successfully\n");       

    // Decode the secret file extension 
    decode_secret_file_extn(decInfo);
    printf("INFO: Output File Extension decoded successfully\n");

    // Data decoded  from the stego image
    decode_secret_file_data(decInfo);
    printf("INFO: Data decoded successfully and copied to file\n");

    return e_success;
}

Status open_file(DecodeInfo *decInfo)    // opening the required files                  
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }
    else
    {
        printf("INFO : %s file open\n", decInfo->stego_image_fname);
    }
    
    return e_success;
}

Status skip_header(FILE *fptr)                      
{
    fseek(fptr, 54, SEEK_SET);  // Skip the first 54 bytes of the BMP file (header)
    return e_success;
}


Status decode_secret_file_extn(DecodeInfo *decInfo)     // Decode secrete file extension        
{
    int len = decode_len(decInfo);

    char str[len + 1];  
    decode_string(len, str, decInfo);

    strcat(decInfo->secret_fname, str);

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)     
{
    
    int len = decode_len(decInfo);

    char str[len + 1];  // Buffer to store the decoded secret data
    decode_string(len, str, decInfo);

    add_secrate_data_to_file(str, decInfo);

    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)             
{
    
    int len = decode_len(decInfo);  // Decode the length of the magic string

    if (len >= 10)
    {
        printf("Magic String not matching!\n");
        return e_failure;
    }

    char str[len + 1];  // +1 for  the null character

    decode_string(len, str, decInfo);

    // Compare the decoded string with the expected magic string
    if (strcmp(str, decInfo->magic_string) != 0)
    {
        printf("Magic String not matching!\n");
        return e_failure;
    }

    return e_success;
}

int decode_len(DecodeInfo *decInfo)         // Decode the string length    
{
    int i, len = 0;
    char buffer[32];  

    fread(buffer, 32, 1, decInfo->fptr_stego_image);

    // Extract the length from LSB of each byte
    for (i = 0; i < 32; i++)
    {
        if (buffer[i] & 1)
        {
            len = (1 << i) | len;  // Set the corresponding bit in the length
        }
    }

    return len;  // Return the decoded length
}

Status decode_string(int len, char str[], DecodeInfo *decInfo)      // Decode the string          
{
    int i, j, k = 0;
    char buffer[8 * len];  

    fread(buffer, 8 * len, 1, decInfo->fptr_stego_image);

    // Decode each character by extracting its bits
    for (i = 0; i < len; i++)
    {
        str[i] = 0;  
        for (j = 0; j < 8; j++)
        {
            if (buffer[k] & 1)
            {
                str[i] = (1 << j) | str[i];  
            }
            k++;
        }
    }
    str[i] = '\0';  // Null-terminate the decoded string

    return e_success;
}

Status add_secrate_data_to_file(char str[], DecodeInfo *decInfo)             
{
    // Open the secret file for writing
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    
    if (decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);
        return e_failure;
    }
    else
    {
        printf("INFO : %s file open\n", decInfo->secret_fname);
    }

    // Write the decoded secret data to the file
    fprintf(decInfo->fptr_secret, "%s", str);

    return e_success;
}
