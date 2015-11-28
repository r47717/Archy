/*
 * search.c
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


/* returns number of occurrences of word in str */
static int occurrences(char *str, char *word) {
	char *p = str;
	int count = 0;
	int word_len = strlen(word);

	do {
		p = strstr(p, word);
		if(p != NULL) {
			count++;
			p += word_len;
		}
	} while(p != NULL && p[0] != 0);

	return count;
}

/* searchs for n-length repeating (>1) substrings in str
 * and adds results to the dictionary
 */
static void find_repeat_n(char * str, int n) {

	int len = strlen(str);
	int shift;
	char *p = str;
	char buf[MAX_TERM + 1];
	int count;

	for(shift = 0; shift <= len - n; shift++) {

		p = str + shift;
		strncpy(buf, p, n);
		buf[n] = 0;

		count = occurrences(p, buf);
		if(count > 2 && !dict_contains(buf))
			dict_add(buf, count);
	}
}

void build_dictionary(char *input) {
	int i;

	for(i = 3; i <= MAX_TERM; i++) {
		find_repeat_n(input, i);
	}
}

