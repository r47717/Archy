#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "archy.h"
#include "dict.h"
#include "rle.h"
#include "huffman.h"

void make_zip_name(char *name, char *zip_name);

int main(int argc, char ** argv) {
 
  char input[MAX_LEN + 1] = "A Marcy woman working as a teacher assistant at Oneida-Herkimer-Madison BOCES was arrested Thursday on charges involving endangering the welfare of a child, according the Oneida County Child Advocacy Center."
"Sheriff’s Investigator Daniel Brown said that on several occasions between Sept. 25 and 29 through Facebook messenger, Lisa M. Hughes, 43, of Marcy, allegedly engaged in sexual chats/messages with a person younger than 17, officials said."
"She was charged with misdemeanor endangering the welfare of a child, officials said.";
  char output[MAX_LEN + 1];

  FILE *fp1;
  FILE *fp2;

  if(argc < 2 || argc > 4) {
    printf("Usage: archy [-a | -x] <input file> [<output file>]");
    return;
  }
  
  if(strcmp(argv[1], "-a") == 0) {
    
    fp1 = fopen(argv[2], "r");
	if(argc == 3) {
		char str[255];
		make_zip_name(argv[2], str);
		fp2 = fopen(str, "w");
	} else {
		fp2 = fopen(argv[3], "w");
	}

  } else if(strcmp(argv[1], "-x") == 0) {
		
	/* tbd */
    
  } else {
    
  }

  fread(fp1, input, MAX_LEN);
  fwrite(fp2, output, MAX_LEN);


  encode_DICT(input, output);
  encode_RLE(input, output);
  encode_Huffman(input, output);
  encode_LZ77(input, output);

  return 0;

}

void make_zip_name(char *name, char *zip_name) {
	int len = strlen(name);
	int i;
	
	for(i = 1; i <= 5 && i <= len; i++) {   /* suppose extention may be 5 symbols long, not more */
		if(name[len - i] == '.')
			break;
	}
	
	if(i == 1) { /* file name ends with '.' - just add 'azip' */
		strcpy(zip_name, name);
		strcat(zip_name, "azip");
	} else if(i == 6 || (i == 5 && len == 4)) { /* no extention */
		strcpy(zip_name, name);
		strcat(zip_name, ".azip");
	} else { /*extention found */
		strncpy(zip_name, name, len - i);
		zip_name[len - i] = 0;
		strcat(zip_name, ".azip");
	}
}











