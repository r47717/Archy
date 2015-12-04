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

#define SEQ_MAX 127


void encode_RLE(char *input, char *output) {

	/*
	 * 0 - start state (no chars read)
	 * 1 - collecting single characters (str)
	 * 2 - collecting sequence (seq_len, seq_sym)
	 * */
	int state = 0;
	char ch;

	char seq_ch;
	char seq_len = 0;

	char sing[SEQ_MAX + 1] = "";
	char sing_len = 0;

	while(ch = *input++) {

		switch(state) {
		case 0:
			sing[0] = ch;
			sing_len = 1;
			state = 1;
			break;
		case 1: /* in the middle of different characters */
			if(ch == str[sing_len - 1]) {
				if(sing_len > 2) {
					sing_len--;
					output++[0] = (char) (-1)*(sing_len);
					strncpy(output, sing, sing_len);
					output += sing_len;
				}
				
				sing_len = 0;
				sing[0] = 0;
				state = 2;
				seq_len = 2;
				seq_ch = ch;
				
			} else {
				sing[sing_len++] = ch;
				if(sing_len == SEQ_MAX) {
					output++[0] = (char) (-1)*(sing_len);
					strncpy(output, sing, sing_len);
					output += sing_len;
					state = 0;
				}
			}
			break;
		case 2: /* in the middle of a sequence */
			if(ch == seq_ch) {
				seq_len++;
				if(seq_len == SEQ_MAX) {
					output++[0] = seq_len;
					output++[0] = seq_ch;
					state = 0;
				}
			} else {
				output++[0] = seq_len;
				output++[0] = seq_ch;
				sing[0] = ch;
				sing_len = 1;
				state = 1;
			}
			break;
		}
	}

}

void decode_RLE(char *input, char *output) {
	char ch;
	
	while(ch = *input++) {
		
		if(ch < 0) {
			/* different charecters */
			strncpy(output, input, (-1)*ch);
			input += ch;
			output += ch;
		} else {
			/* sequence with len ch */
			int c = *input++;
			while(ch--) {
				output++[0] = c;
			}
		}
	
	}
}



