/*
 * encoder.c
 *
 *  Created on: 27 нояб. 2015 г.
 *      Author: r47717
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "archy.h"
#include "dict.h"
#include "search.h"
#include "encoder.h"

#define MARKER ('\002')

static char* encode_dict1(int id, char *term, char *buf);
static char* encode_dict_table(char *buf, int id[]);


void encode(char *input, char *output, int id[]) {
	int i;

	output = encode_dict_table(output, id);

	for(i = 0; i < MAX_ID && id[i] > 0; i++) {

		char *inp = input;
		char *out = output;
		char *tmp = input;
		char term[MAX_TERM + 1] = "";
		int term_id = id[i];
		int term_len;

		dict_get_by_id(term_id, term);
		if(term[0] == 0)
			return;

		term_len = strlen(term);

		/* algorithm:
		 * 1. build dict table (done)
		 * 2. find next occ of term in input
		 * 3. copy all characters before occ to output
		 * 4. encode occ to <MARKER,id>
		 * 5. goto 2 if not end of input
		 * 6. copy the rest of the string
		 */

		out = output;
		do {
			tmp = strstr(inp, term);
			if(tmp) {
				char *p = inp;
				while(p != tmp) {
					out++[0] = p++[0];
				}
				inp = tmp;
				out++[0] = MARKER;
				out++[0] = (char) term_id;
				out[0] = 0;
				inp += term_len;
			} else {
				strcpy(out, inp);
			}

		} while(tmp && inp[0]);

		input = output;

	} /* for */
}

static char* encode_dict1(int id, char *term, char *buf) {

	char len = strlen(term);
	buf[0] = (char) id;
	buf[1] = len;
	strcpy(buf + 2, term);
	return buf + len + 2;
}

static char* encode_dict_table(char *buf, int id[]) {

	int i = 0;
	char term[MAX_TERM + 1];

	for(i = 0; i < MAX_ID && id[i] > 0; i++) {
		dict_get_by_id(id[i], term);
		buf = encode_dict1(id[i], term, buf);
	}

	buf++[0] = MARKER;
	return buf;
}


