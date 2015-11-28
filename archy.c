
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "archy.h"
#include "dict.h"
#include "search.h"
#include "encoder.h"



void pr_error(char * msg);
void archive(char * input, char * output);
void extract(char * input, char * output);

/****************** MAIN **********************/

int main(int argc, char ** argv) {
 
  char input[MAX_LEN + 1] = "A Marcy woman working as a teacher assistant at Oneida-Herkimer-Madison BOCES was arrested Thursday on charges involving endangering the welfare of a child, according the Oneida County Child Advocacy Center."
"Sheriff’s Investigator Daniel Brown said that on several occasions between Sept. 25 and 29 through Facebook messenger, Lisa M. Hughes, 43, of Marcy, allegedly engaged in sexual chats/messages with a person younger than 17, officials said."
"She was charged with misdemeanor endangering the welfare of a child, officials said.";

  char output[MAX_LEN + 1];
  int len1, len2;
  int i;

  archive(input, output);



/*
  if(argc < 4) {
    printf("Usage: archy [-a] [-x] <input file> <output file>");
    return;
  }
  
  if(strcmp(argv[1], "-a") == 0) {
    
    FILE * fp1 = fopen(argv[2], "r");
    FILE * fp2 = fopen(argv[3], "w");
    fread(fp1, input, MAX_LEN);
    archive(input, output);
    fwrite(fp2, output, MAX_LEN);


	  archive(input, output);

  } else if(strcmp(argv[1], "-x") == 0) {
    
    
  } else {
    
  }
*/

  return 0;

}




void archive(char * input, char * output) {
  int len1, len2;
  float cmpr;
  int id[MAX_ID];

  dict_init();
  build_dictionary(input);
  dict_compact();
  printf("\n\nCompact dictionary: \n");
  dict_print();

  dict_build_id_list(id);
  encode(input, output, id);
  len1 = strlen(input);
  len2 = strlen(output);
  cmpr = ((float)len2)/len1*100;
  printf("\n\nInput string (%d): %s", len1, input);
  printf("\nOutput string (%d): %s", len2, output);
  printf("\nCompression: %.0f%%", cmpr);

  dict_destroy();
}




/****************** EXTRACT **********************/

void extract(char * input, char * output) {
  

}


/****************** UTILS **********************/

void pr_error(char * msg) {
	printf("Error; %s", msg);
	dict_destroy();
	exit(1);
}














