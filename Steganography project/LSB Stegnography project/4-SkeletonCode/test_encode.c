/*
Name : Sharan
Date : 20/10/2024
Description : Implementation of LSB image Steganography project
*/

#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char* argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    uint img_size;

    // Check if required line arguments are provided
    if (argc == 1) 
    {
        printf("./lsb_steg : Encoding : ./lsb_steg -e <.bmp file> <secret file> [optional : .bmp file]\n");
        printf("./lsb_steg : Encoding : ./lsb_steg -d <.bmp file> [optional : .txt file]\n");
        return 1; 
    }

    // Determine operation type (encoding or decoding)
    int ret = check_operation_type(argv);
    
    if (ret == e_encode) // Encoding operation
    {
        printf("----------Encoding operation selected----------\n");
        // Ensure sufficient command-line arguments for encoding
        if (argc < 4)
        {
            printf("Error! Invalid command line argument.\n");
            return 1;
        }

        // Validate encoding arguments
        if (valid_argv_encode(argc, argv) == e_failure)
        {
            printf("Error! Invalid command line argument.\n");
            return 1;
        }

        // Assign filenames for source image and secret file
        encInfo.src_image_fname = argv[2];
        encInfo.secret_fname = argv[3];
        
        // Set the stego image filename if provided, otherwise use default
        if (argc == 4)
        {
            encInfo.stego_image_fname = "default_stego_img.bmp"; // Default output filename
        }
        else
        {
            encInfo.stego_image_fname = argv[4];
        }

        // open necessary files
        if (open_files(&encInfo) == e_failure)
        {
            printf("ERROR: Failed to open files.\n");
            return 1;
        }
        else
        {
            printf("INFO: Files opened successfully.\n");
        }

        // Check if  source image can hold the secret data
        if (check_capacity(argv, &encInfo) == e_failure)
        {
            printf("Error! Source file size is below the required limit.\n");
            return 1;
        }

        // read magic string from user
        printf("Enter the Magic String: ");
        if (scanf("%s", encInfo.magic_string) != 1) 
        {
            printf("Error! Failed to read the magic string.\n");
            return 1;
        }

        // Reset file pointer 
        rewind(encInfo.fptr_src_image);
        do_encoding(&encInfo); // Function to encode secret data

        printf("----------Encoding secret data completed.----------\n");

        // Close all opened files
        fclose(encInfo.fptr_src_image);
        fclose(encInfo.fptr_secret);
        fclose(encInfo.fptr_stego_image);
    }
    else if (ret == e_decode) // Decoding operation
    {
        printf("----------Decoding operation selected----------\n");
        // Check if the correct number of arguments is passed for decoding
        if (argc < 3)
        {
            printf("Error! Invalid file argument.\n");
            return 1;
        }

        // Validate that the provided file is a .bmp file
        if (strcmp(strchr(argv[2], '.'), ".bmp") != 0)
        {
            printf("Error! Invalid file format. Only .bmp is supported.\n");
            return 1;
        }

        // Set the decoded secret filename, or use default if not provided
        if (argv[3] == NULL)
        {
            strcpy(decInfo.secret_fname, "default_decoded_secret"); // Default decoded file
        }
        else
        {
            int i;
            for (i = 0; argv[3][i] != '.'; i++)
            {
                decInfo.secret_fname[i] = argv[3][i];
            }
            decInfo.secret_fname[i] = '\0'; // Null-terminate the string
        }

        // Ask user for the magic string
        printf("Enter the Magic String: ");
        if (scanf("%s", decInfo.magic_string) != 1) // Error handling for input
        {
            printf("Error : Failed to read the magic string.\n");
            return 1;
        }

        // Set the stego image filename for decoding
        decInfo.stego_image_fname = argv[2];

        // Perform decoding operation
        if (do_decoding(&decInfo) == e_failure)
        {
            printf("Error! Decoding failed.\n");
            return 1;
        }

        printf("----------Decoding secret data completed.----------\n");

        // Close the stego image file
        fclose(decInfo.fptr_stego_image);
    }
    else // Invalid operation type
    {
        printf("Error! Unsupported operation. Use -e for encoding or -d for decoding.\n");
        return 1;
    }

    return 0; 
}

OperationType check_operation_type(char *argv[]) // Function to determine operation type
{
    if (argv[1][1] == 'e') // Check for encoding flag
    {
        return e_encode;
    }
    else if (argv[1][1] == 'd') // Check for decoding flag
    {
        return e_decode;
    }
    else
    {
        return e_unsupported; // Unsupported operation
    }
}
