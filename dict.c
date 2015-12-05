/*
 * dict.c
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

static void pr_error(char * msg);

static int occurrences(char *str, char *word);
static void find_repeat_n(char * str, int n);
static void build_dictionary(char *input);


/*==================================== Dictionary Operations =========================================*/

typedef struct dict {
	int id;
	char term[MAX_TERM + 1]; /* repeated word in input file */
	unsigned occ;            /* number of occurrences in the file */
	int factor;				 /* should be > 0 for effective compression */
	struct dict * next;
	struct dict * prev;
} dict_t;

static int last_id = 0;
static dict_t * head = NULL;
static dict_t * tail = NULL;
static unsigned no_elems = 0;


/* initialize dict structure */
void dict_init() {
}

/* frees dict list structure memory */
void dict_destroy() {
	dict_t * tmp = head;

	while(head != NULL) {
		tmp = tmp->prev;
		free(head);
		head = tmp;
	}
}

/* add new element to the dictionary */
int dict_add(char * term, unsigned occ) {
	dict_t * tmp = malloc(sizeof(dict_t));
	if(tmp == NULL)
		pr_error("Error: cannot allocate memory");

	strcpy(tmp->term, term);
	tmp->occ = occ;
	tmp->id = ++last_id;
	tmp->factor = occ * (strlen(term) - 2) - strlen(term) - 3;

	if(head == NULL) { /* first element */
		head = tail = tmp;
		tmp->next = tmp->prev = NULL;
	} else {
		tail->prev = tmp;
		tmp->prev = NULL;
		tmp->next = tail;
		tail = tmp;
	}


	no_elems++;
	return last_id;
}

static void dict_remove(dict_t *item) {
	if(item->next == NULL) { /* head element */
		head = item->prev;
	} else {
		item->next->prev = item->prev;
	}

	if(item->prev == NULL) { /* tail element */
		tail = item->next;
	} else {
		item->prev->next = item->next;
	}

	free(item);
}

void dict_get_by_id(int id, char *term) {
	dict_t * tmp = head;

	while(tmp != NULL) {
		if(tmp->id == id) {
			strcpy(term, tmp->term);
			break;
		}
		tmp = tmp->prev;
	}
}

int  dict_get_num() {
	return no_elems;
}


/* search for this term in dictionary */
int dict_contains(char * term) {
	dict_t * tmp = head;

	while(tmp != NULL) {
		if(strcmp(tmp->term, term) == 0)
			return 1;
		tmp = tmp->prev;
	}
	return 0;
}

/* print dictionary content */
void dict_print() {
	dict_t * tmp = head;

	while(tmp != NULL) {
		printf("Id: %d, term: '%s', occurrences: %d, factor: %d\n", tmp->id, tmp->term,
				tmp->occ, tmp->factor);
		tmp = tmp->prev;
	}
}

void dict_clean_duplicates() {
	dict_t *roll;

	start: roll = tail;
	while(roll != head) {
		dict_t *tmp = roll->next;
		while(tmp) {
			if(strstr(roll->term, tmp->term) && roll->occ >= tmp->occ) {
				dict_remove(tmp);
				goto start;
			}
			tmp = tmp->next;
		}
		roll = roll->next;
	}
}

void dict_compact() {
	dict_clean_duplicates();

}

void dict_build_id_list(int id[]) {
	dict_t * tmp = tail;
	int i = 0;

	while(tmp != NULL) {
		if(tmp->factor >= 0)
			id[i++] = tmp->id;
		tmp = tmp->next;
	}
	id[i] = 0;
}

/*==================================== Encoder/Decoder =========================================*/

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

void encode_DICT(char * input, char * output) {
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

void decode_DICT(char * input, char * output) {

}

/*==================================== String Search Operations =========================================*/


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

static void build_dictionary(char *input) {
	int i;

	for(i = 3; i <= MAX_TERM; i++) {
		find_repeat_n(input, i);
	}
}




static void pr_error(char * msg) {
	printf("Error; %s", msg);
	dict_destroy();
	exit(1);
}


