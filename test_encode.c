#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char **argv)
{
		if(argc<2)
		{

				printf("ERROR : Invalid arguments\n");
				printf("FOR ENOCDING :  ./a.out -e source.bmp secrete_message.txt\n");
				printf("FOR DECODING: ./a.out -d destination.bmp output\n");
				return e_failure;

		}
		else
		{
				if (check_operation_type(argv) == e_encode)         // encoding function
				{
						if(argc >3)
						{
								EncodeInfo encInfo;
								if((read_and_validate_encode_args(argv,&encInfo))== e_success)
								{
										printf("INFO : Read and validate is done Successfully\n");
								}
								else
								{
										printf("ERROR : Failed to read and validate args\n");
										return e_failure;
								}
								if((do_encoding(&encInfo))== e_success)
								{
										printf("Encoding is successfull !!\n");
								}
								else
								{
										printf("ERROR : Failed to do the Encoding function\n");
										return e_failure;
								}
						}
						else
						{
								printf("ERROR : Invalid arguments\n");
								printf("FOR ENOCDING :  ./a.out -e source.bmp secrete_message.txt\n");
								printf("FOR DECODING: ./a.out -d destination.bmp output\n");
								return e_failure;
						}
				}

				else if (check_operation_type(argv) == e_decode)          // decoding function
				{
						if (argc >2)
						{
								DecodeInfo decInfo;
								if((read_and_validate_decode_args(argv,&decInfo)) == e_success)
								{
										printf("INFO : Read and validate is done Successfully in decoding\n");
								}
								else
								{
										printf("ERROR : Failed to read and validate while decoding\n");
										return e_failure;
								}
								if((do_decoding(&decInfo)) == e_success)
								{
										printf("Decoding done successfully !!\n");
								}
								else
								{
										printf("ERROR : Failed to do the decode \n");
										return e_failure;
								}
						}
						else
						{
								printf("ERROR : Invalid arguments\n");
								printf("FOR ENOCDING : ./a.out -e source.bmp secrete_message.txt\n");
								printf("FOR DECODING : ./a.out -d destination.bmp output\n");
								return e_failure;
						}
				}
				else
				{
						printf("ERROR : Invalid arguments\n");
						printf("FOR ENOCDING : ./a.out -e source.bmp secrete_message.txt\n");
						printf("FOR DECODING : ./a.out -d destination.bmp output\n");
						return e_failure;
				}
		}
}	



OperationType check_operation_type(char **argv) // checks operation to be done 
{
		if((!strcmp(argv[1],"-e")))
		{
				return e_encode;
		}
		if((!strcmp(argv[1],"-d")))
		{
				return e_decode;
		}
		else
		{
				return e_unsupported;
		}


}

