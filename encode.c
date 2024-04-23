#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "common.h"
#include "types.h"

/* Function Definitions */
Status read_and_validate_encode_args(char **argv, EncodeInfo *encInfo)
{
		if ((strstr(argv[2], ".")))
		{
				if (!(strcmp(strstr(argv[2],"."),".bmp" )))
				{
						encInfo->src_image_fname=argv[2];
				}
				else
				{
						printf("Error : invalid file\n");
						return e_failure;
				}
		}
		else
		{
				printf("Error : Invalid file \n");
				return e_failure;
		}
		if(argv[3] != NULL)
		{
				if ((strstr(argv[3], ".")))
						if (!(strcmp(strstr(argv[3],"."),".txt")))
						{
								encInfo->secret_fname= argv[3];
								strcpy(encInfo->extn_secret_file,strstr(argv[3],".txt"));
						}
						else
						{
								printf("Error : Invalid file \n");
								return e_failure;
						}
				else
				{
						printf("Error : Invalid file \n");
						return e_failure;
				}

		}
		else
		{
				printf("ERROR : Secret file not passed \n");
				return e_failure;
		}
		if (argv[4] != NULL )
		{
				if(strstr(argv[4],"."))
				{
						if (!(strcmp(strstr(argv[4] ,"."),".bmp")))
						{

								encInfo->stego_image_fname = argv[4];
						}
						else
						{
								printf("ERROR : Use correct extention for the destination file \n");
								return e_failure;
						}
				}
				else
				{

						printf("ERROR : Provide the destination name with extention \n");
						return e_failure;
						/*	encInfo->stego_image_fname=argv[4];
							strcat(encInfo->stego_image_fname,".bmp");*/
				}
		}
		else 
		{
				encInfo->stego_image_fname = "stego.bmp";

		}
		return e_success;
}

//argv[2] is .bmp file or not -> strstr(argv[2] , ".bmp") != '\0' 
// argv[3] is .txt file or not -> strstr(argv[3] , ".txt" )!= '\0' and store in the extn_secret_file
//argv[4] is passed or not if passed the ->store that name into stego_image_fname
// if  not passed -> create default name and stor into stego_image_fname
//if any one is failed return e_failure

Status do_encoding(EncodeInfo *encInfo)
{
		if(open_files(encInfo) == e_success)
		{       
				printf("INFO : Files opened successfully !\n");

		}
		else
		{
				printf("INFO : Open file function not executed successfully\n");
		}
		if(check_capacity(encInfo) == e_success)
		{
				printf("INFO : Check capacity is done, We have enough space to encode\n");
		}
		else
		{
				printf("INFO : Check capacity is done, We don't have enough space to encode\n");
				return e_failure;
		}
		if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)== e_success)
		{
				printf("INFO : Copied .bmp header files\n");
		}
		else
		{
				printf("INFO : Failed to copy .bmp header files\n");
				return e_failure;
		}
		if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)
		{
				printf("INFO : Magic string encoded successfully\n");
		}
		else
		{
				printf("INFO : Failed to encode Magic string\n");
				return e_failure;
		}
		if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo) == e_success)
		{
				printf("INFO : Encoded secret file extention size\n");
		}
		else
		{
				printf("INFO : Failed to Encode secret file extention size\n");
				return e_failure;
		}
		if((encode_secret_file_extn(encInfo->extn_secret_file,encInfo)) == e_success)
		{
				printf("INFO : Encoded secret file extention\n");

		}
		else
		{
				printf("INFO : Error ! failed to Encode secret file extention\n");
				return e_failure;
		}
		if((encode_secret_file_size(encInfo->size_secret_file,encInfo))== e_success)
		{
				printf("INFO : Encoded Secret file size\n");
		}
		else
		{
				printf("INFO : Failed to Encode Secret file size\n");
				return e_failure;
		}
		if(encode_secret_file_data(encInfo)== e_success)
		{
				printf("INFO : Encoded Secret file data\n");
		}
		else
		{
				printf("INFO : Failed to Encode Secret file data\n");
				return e_failure;
		}
		if((copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)) == e_success)
		{
				printf("INFO : Copied remaining image data\n");
		}
		else
		{
				printf("INFO : Failed to copy image data \n");
				return e_failure;
		}
		return e_success;
		//step1 : call openfiles()
		//step2 : if success -> goto step3
		//else -> print error ,return failure.
		//step3: call check_capacity()
		//step4: if success -> got step 5
		//else -> print error ,return failure.
		//step5 : copy _bmp_header()
		//else -> printf error ,return failure.
		//step6 :if  success -> goto step 7i
		//else -> printf error ,return failure.
}
Status check_capacity(EncodeInfo *encInfo)
{
		encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
		fseek(encInfo->fptr_secret,0,SEEK_END);
		encInfo->size_secret_file = ftell(encInfo->fptr_secret);
		if((16 + 32 + (strlen(encInfo->extn_secret_file) * 8) + sizeof(encInfo->size_secret_file) + (encInfo->size_secret_file *8)) < encInfo->image_capacity)
		{
				return e_success;
		}
		//step1 -> find src_image_file size  store in image capacity
		//step2 -> find secret file size store in size_secret_file
		//step3 -> caluclate the value of how much bytes needed for encoding (16(magic_string)+32(size_of_extn)+32(ext)+32(size_of_secret_size))+secrete_size file X 8;
		//step4 -> compare image capacity > step3 if true return e_success else return e_failure

}
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_stego_image)
{
		char buffer[54];
		rewind(fptr_src_image);			
		fread(buffer,1,54,fptr_src_image);
		int write = fwrite(buffer ,1,54,fptr_stego_image);		
		return e_success;
		//step0: Rewind fptr_src_image 
		//step1: read 54 bytes from fptr_src_image fread(buffer,54,1,fptr_stego_image)
		//step2: write 54 bytes to fptr_stego_image .return success:
}
Status encode_magic_string(char *magic_string , EncodeInfo *encInfo)
{
		if(encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
		{
				return e_success;
		}
}
Status	encode_data_to_image(char *data,int size ,FILE *fptr_src_image,FILE *fptr_stego_image)
{
		char image_buffer[8];
		for(int i=0;i<size;i++)
		{
				fread(image_buffer,1,8,fptr_src_image);
				encode_byte_to_lsb(data[i],image_buffer);
				fwrite(image_buffer,1,8,fptr_stego_image);
		}
		return e_success;
		//step1 : read 8 byters from source image
		//call encode_byte_to_lsb();
		// write 8 bytes from image_buffer to fptr_stego_image 
		// step 4 repeat this upto the size times
}
Status encode_byte_to_lsb(char ch, char *image_buffer)
{
		for(int i=0;i<8;i++)
		{
				char c = ch&(1<<(7-i));

				image_buffer[i] = image_buffer[i]&(~(1));
				image_buffer[i] = image_buffer[i] | (c>>(7-i));
		}
		//step 1: run the loop 8 time;
		//step 2: clear LSB of image_buffer[i];
		//step 3: get 1 bit from ch
		// Replace
		//repeat it 8times
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
		char image_buffer[32];

		fread(image_buffer,1,32,encInfo->fptr_src_image);
		encode_int_to_image(size,image_buffer);
		/*	for(int i=0;i<32;i++)
			{
			int bit =size & (1<<i);
			image_buffer[i] = image_buffer[i]&~(1);
			image_buffer[i]= image_buffer[i] | (bit >>i);
		//				printf("%d",image_buffer[i]);
		}*/
		fwrite(image_buffer,1,32,encInfo->fptr_stego_image);

		return e_success;

}
Status encode_int_to_image(int s, char *buffer)
{
		for(int i=0;i<32;i++)
		{
				int bit = s & (1<<i);
				buffer[i] = buffer[i]&~(1);
				buffer[i] = buffer[i] | (bit >>i);
				//				printf("%d",buffer[i]);
		}
}
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
		if((encode_data_to_image(encInfo->extn_secret_file,strlen(encInfo->extn_secret_file),encInfo->fptr_src_image,encInfo->fptr_stego_image)) == e_success)
		{
				return e_success;
		}
}

Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
		char image_buffer[32];

		fread(image_buffer,1,32,encInfo->fptr_src_image);
		encode_int_to_image(file_size,image_buffer);
		fwrite(image_buffer,1,32,encInfo->fptr_stego_image);
		return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
		char buffer[1];
		rewind(encInfo->fptr_secret);
		for(int i=0;i<encInfo->size_secret_file;i++)
		{
				fread(buffer,1,1,encInfo->fptr_secret);
				encode_data_to_image(buffer,1,encInfo->fptr_src_image,encInfo->fptr_stego_image);
		}
		return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
		char buffer[1];
		while(	fread(buffer,1,1,fptr_src))
		{

				fwrite(buffer,1,1,fptr_dest);
		}
		return e_success;

}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
		uint width, height;
		// Seek to 18th byte
		fseek(fptr_image, 18, SEEK_SET);

		// Read the width (an int)
		fread(&width, sizeof(int), 1, fptr_image);
		printf("width = %u\n", width);

		// Read the height (an int)
		fread(&height, sizeof(int), 1, fptr_image);
		printf("height = %u\n", height);

		// Return image capacity
		return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
		// Src Image file
		encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
		// Do Error handling
		if (encInfo->fptr_src_image == NULL)
		{
				perror("fopen");
				fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

				return e_failure;
		}

		// Secret file
		encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
		// Do Error handling
		if (encInfo->fptr_secret == NULL)
		{
				perror("fopen");
				fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

				return e_failure;
		}

		// Stego Image file
		encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
		// Do Error handling
		if (encInfo->fptr_stego_image == NULL)
		{
				perror("fopen");
				fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

				return e_failure;
		}

		// No failure return e_success
		return e_success;
}
