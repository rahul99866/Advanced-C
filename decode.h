#ifndef DECODE_H
#define DECODE_H
#include"types.h"
#include"common.h"
typedef struct _decodeInfo
{
    char *stego_image_fname;	//source  inmage filename argv[2]
    FILE *fptr_stego_image;    //source imageqs   `xxxxxxxxxxxxxxxxxxxxxx file pointer

    char output_file_name[25];  //source secrete filename argv[3]
	int extension_size;
    FILE *fptr_output;  //source secrete file pointer
	char file_extn[4];
	int file_size;


} DecodeInfo;

Status read_and_validate_decode_args(char **argv, DecodeInfo *decInfo);
Status do_decoding(DecodeInfo *decInfo);
Status open_file(DecodeInfo *decInfo);
Status decode_magic_string(char *magic_string , DecodeInfo *decInfo);
Status lsb_data_decode( char *ch ,int s, char *buffer);
Status decode_extn_size(DecodeInfo *decInfo);
Status decode_int(char *buffer);
Status decode_file_extn( DecodeInfo *decInfo);
Status string_cat(char *s, char *s2);
Status open_output_file(DecodeInfo *decInfo);
Status decode_file_size(DecodeInfo *decInfo);
Status decode_data(DecodeInfo *decInfo);
char decode_buffer(char *buffer);


#endif
