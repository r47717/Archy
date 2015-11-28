/*
 * dict.h
 *
 *  Created on: 27 нояб. 2015 г.
 *      Author: r47717
 */

#ifndef DICT_H_
#define DICT_H_

void dict_init();
void dict_destroy();
int  dict_add(char * term, unsigned occ);
int  dict_contains(char * term);
void dict_get_by_id(int id, char *buf);
int  dict_get_num();
void dict_print();
void dict_compact();
void dict_build_id_list(int id[]);

#endif /* DICT_H_ */
