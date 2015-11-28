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








