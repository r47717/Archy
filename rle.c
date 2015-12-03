/*
 * rle.c
 *
 *  Created on: 3 дек. 2015 г.
 *      Author: r47717
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "archy.h"
#include "rle.h"

#define SEQ_LEN 127


void encode_RLE(char *input, char *output) {

	/*
	 * 0 - start state
	 * 1 - collecting single characters (str)
	 * 2 - collecting sequence (seq_len, seq_sym)
	 * 3 -
	 * */
	int state = 0;
	char str[SEQ_LEN + 1] = "";
	char ch;
	char seq_ch;
	char seq_len = 0;
	char sing_len = 0;

	while(ch = *input++) {

		switch(state) {
		case 0:
			str[0] = ch;
			sing_len = 1;
			state = 1;
			break;
		case 1:
			if(ch == str[sing_len - 1]) {

			}
			break;
		case 2:
			break;
		}
	}

}
