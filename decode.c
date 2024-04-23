#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "common.h"
#include "types.h"

/* Function Definitions */
/*collect the command line arguments
 *store them in the char array
 *create files for those CLI arguments
 */

Status read_and_validate_decode_args(char **argv, DecodeInfo *decInfo)   
{
		if(strstr(argv[2],"."))
		{

				if (!(strcmp(strstr(argv[2],"."),".bmp" )))
				{
						decInfo->stego_image_fname=argv[2];
				}
				else
				{
						printf("ERROR : The encoded file isn't passed\n"); 
						return e_failure;
				}
		}
		else
		{
				return e_failure;
		}
		if (argv[3] != NULL)
		{
				if(strstr(argv[3],"."))
				{
						strcpy(decInfo->output_file_name,argv[3]);
						*((char *)strstr(decInfo->output_file_name,"."))='\0';	
				}
				else
				{
						strcpy(decInfo->output_file_name,argv[3]);
				}
		}
		else
		{
				strcpy(decInfo->output_file_name, "Output_file");
		}
		return e_success;
}
Status do_decoding(DecodeInfo *decInfo)    //dcoding function defination
{
		if(open_file(decInfo) == e_success)
		{    
				printf("INFO : Files opened executed successfully\n");

		}
		else
		{
				printf("INFO : Open file function not executed successfully\n");
		} 
		if(decode_magic_string(MAGIC_STRING,decInfo) == e_success)
		{
				printf("INFO : Magic string decoded successfully\n");
		}
		else
		{
				printf("INFO : Failed to decoded Magic string\n");
				return e_failure;
		}
		if((decode_extn_size(decInfo)) == e_success)
		{
				printf("INFO : Extension size decoded successfully\n");
		}
		else
		{
				printf("INFO : Extension size decode Failed\n");
				return e_failure;
		}
		if((decode_file_extn(decInfo))== e_success)
		{
				printf("INFO : File Extension decoded successfully\n");
		}
		else
		{
				printf("INFO : File extension decode Failed\n");
				return e_failure;
		}
		if((string_cat(decInfo->output_file_name,decInfo->file_extn)) == e_success)
		{
				printf("INFO : Extension of the file is concatenated\n");
		}
		else
		{
				printf("INFO : Error to the concatenate file Extension\n");
				return e_failure;
		}
		if((open_output_file(decInfo)) == e_success)
		{
				printf("INFO : Output file opened\n");
		}
		else
		{
				printf("ERROR : Failed to open Output file\n");
				return e_failure;
		}
		if((decode_file_size(decInfo)) == e_success)
		{
				printf("INFO : Decoding file size successfull\n");
		}
		else
		{
				printf("ERROR : Failed to decode file size\n");
				return e_failure;

		}
		if((decode_data(decInfo)) == e_success)
		{
				printf("INFO : Decoding Data from the image file Successfull \n");
		}
		else
		{
				printf("ERROR : Failed to decode data from the file \n");
				return e_failure;

		}
		return e_success;
}

Status open_file(DecodeInfo *decInfo)  //opens file function
{
		decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
		if (decInfo->fptr_stego_image == NULL)
		{
				perror("fopen");
				fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

				return e_failure;	
		}
		return e_success;
}

Status decode_magic_string(char *magic_string , DecodeInfo *decInfo)  //decode the magicstring
{
		fseek(decInfo-> fptr_stego_image,54,SEEK_CUR);
		char buffer[8];
		char ch[3]={0};
		int i;
		for( i=0;i<2;i++)
		{
				fread(buffer,sizeof(char),8,decInfo->fptr_stego_image);
				lsb_data_decode(ch,i,buffer);
		}
		ch[2]='\0';
		if(!(strcmp(ch,magic_string)))
		{
				return e_success;
		}
}
Status lsb_data_decode( char *ch ,int s, char *buffer)   //decode the data from the lsb of the byte
{
		for(int j=0;j<8;j++)
		{
				char bit =buffer[j]&(0x1);
				ch[s]=ch[s] | (bit<<(7-j));
		}
}

Status decode_extn_size(DecodeInfo *decInfo)     // decode the size of the extention
{
		char buffer[32];
		fread(buffer,1,32,decInfo->fptr_stego_image);
		decInfo->extension_size = decode_int(buffer);
		//		printf("%d\n",decInfo->extension_size);
		return e_success;
}
Status decode_int(char *buffer)     // decode the lsb side for the 32 bits
{
		int num;
		for(int i=0;i<32;i++)
		{
				int bit =buffer[i]&(1);
				num = num & (~(1<<i));
				num=num | (bit<<i);
		}
		return num;
}
Status decode_file_extn( DecodeInfo *decInfo)   //decode the extention of the file
{
		int i;
		char ext_buffer[4]={0};
		for(i=0;i<decInfo->extension_size;i++)
		{
				char buffer[8];
				fread(buffer,1,8,decInfo->fptr_stego_image);
				lsb_data_decode(ext_buffer,i,buffer);
		}
		ext_buffer[i]= '\0';
		strcpy(decInfo->file_extn,ext_buffer);
		return e_success;
}

Status string_cat(char *s, char *s2)   // concatenate the extention of the secret file to the output file
{
		strcat(s,s2);
		return e_success;
}
Status open_output_file(DecodeInfo *decInfo)   //opens the output file to write 
{
		decInfo->fptr_output= fopen(decInfo->output_file_name, "w");
		return e_success;
}
Status decode_file_size(DecodeInfo *decInfo)   // decode the size of the secret file 
{
		char buffer[32];
		fread(buffer,1,32,decInfo->fptr_stego_image);
		decInfo->file_size= decode_int(buffer);
		return e_success;
}
Status decode_data(DecodeInfo *decInfo)   //decode the data from the stego.bmp and stores in to the output file
{
		char ch=0;
		int size = decInfo->file_size;
		char data[size];
		for(int i=0;i<size;i++)
		{
				char buffer[8];
				fread(buffer,1,8,decInfo->fptr_stego_image);
				ch = decode_buffer(buffer);
				fwrite(&ch,1,1,decInfo->fptr_output);
		}
		return e_success;
}
char decode_buffer(char *buffer)    // decoding each character function from the stego.bmp
{
		char ch;
		for(int i=0;i<8;i++)
		{
				char bit = buffer[i] & 1;
				ch = ch & (~(1<<(7-i)));
				ch = ch | (bit <<(7-i));
		}
		return ch;
}
