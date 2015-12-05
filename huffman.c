#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "huffman.h"

#define MAX_CODE_LEN 100



/*---- build tree ----*/

static char freq[255];
static int freq_sum;

typedef struct tree {
	unsigned char ch;
	int freq;
	char code[MAX_CODE_LEN];
	char code_len;
} tree_t;

static tree_t tree_data[255];
static int tree_data_len = 0;


static void count_freq(char *input) {
	unsigned char c;
	for(c = 0; c < 255; c++)
		freq[c] = 0;

	freq_sum = strlen(input);

	while(c = *input++)
		freq[c]++;
}

static int comp(const void* elem1, const void *elem2) {
	tree_t *one = (tree_t*)elem1;
	tree_t *two = (tree_t*)elem2;
	return (one->freq == two->freq) ? 0 : ((one->freq > two->freq) ? -1 : 1);
}

static void init_tree() {
	unsigned char c;

	tree_data_len = 0;
	for(c = 0; c < 255; c++) {
		if(freq[c]) {
			tree_data[tree_data_len].ch = c;
			tree_data[tree_data_len].freq = freq[c];
			tree_data[tree_data_len].code[0] = 0;
			tree_data[tree_data_len].code_len = 0;
			freq[c] = tree_data_len;
			tree_data_len++;
		}
	}

	qsort(tree_data, tree_data_len, sizeof(tree_t), comp);
}

static void print_tree() {
	int i;
	int k;
	for(i = 0; i < tree_data_len; i++) {
		printf("char: '%c', frequency: %3d, code: ", tree_data[i].ch, tree_data[i].freq);
		for(k = 0; k < tree_data[i].code_len; k++)
			printf("%1d", tree_data[i].code[k]);
		printf("\n");
	}
}

static void assign_codes_seq(int first, int num, int depth, int sum) {
	int i = first;
	int k;
	int subsum = 0;

	do {
		subsum += tree_data[i].freq;
		tree_data[i].code[depth] = 0;
		tree_data[i].code_len++;
		i++;
	} while(subsum < sum / 2);

	if(i > first + 1)
		assign_codes_seq(first, i - first, depth + 1, subsum);

	subsum = 0;
	for(k = i; k < first + num; k++) {
		tree_data[k].code[depth] = 1;
		tree_data[k].code_len++;
		subsum += tree_data[k].freq;
	}

	if(k > i + 1)
		assign_codes_seq(i, k - i, depth + 1, subsum);
}

static void assign_tree_codes() {
	assign_codes_seq(0, tree_data_len, 0, freq_sum);
}

/*---- bit operations ----*/

char get_bit(unsigned char ch, char i) {
	return (((unsigned char)255 >> i) & ch) == 0 ? 0 : 1;
}

unsigned char set_bit(unsigned char ch, char bit_num, char value) {
	unsigned char mask = ((unsigned char)128) >> bit_num;
	return value ? (mask | ch) : (~mask & ch);
}

int gen_tree_bit_sequence(char *input, char *output) {
	char ch;
	int k;
	unsigned char byte = 0;
	int bit_cnt = 0;
	int byte_ctr = 0;
	int input_len = strlen(input);

	while(ch = *input++) {
		int i = freq[ch];
		for(k = 0; k < tree_data[i].code_len; k++) {
			byte = set_bit(byte, bit_cnt++, tree_data[i].code[k]);
			if(bit_cnt == 8) {
				output++[0] = byte;
				byte_ctr++;
				bit_cnt = 0;
				byte = 0;
			}
		}
	}
	if(bit_cnt != 8) {
		output++[0] = byte;
		byte_ctr++;
	}

	return byte_ctr;
}


/*---- encoder ----*/

int encode_Huffman(char *input, char *output) {
	count_freq(input);
	init_tree();
	assign_tree_codes();
	return gen_tree_bit_sequence(input, output);
/*	print_tree();*/
}

/*---- decoder ----*/

void decode_Huffman(char *input, char *output) {
	
}
