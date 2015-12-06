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
static char tree_hash[255];

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

static void make_tree_hash() {
	int i;
	for(i = 0; i < tree_data_len; i++)
		tree_hash[tree_data[i].ch] = i;
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
			tree_data_len++;
		}
	}

	qsort(tree_data, tree_data_len, sizeof(tree_t), comp);
	make_tree_hash();
}


static void print_tree() {
	int i;
	int k;
	for(i = 0; i < tree_data_len; i++) {
		printf("%d. char: '%c', frequency: %3d, code: ", i, tree_data[i].ch, tree_data[i].freq);
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

static char get_bit(unsigned char ch, char i) {
	return (((unsigned char)255 >> i) & ch) == 0 ? 0 : 1;
}

static unsigned char set_bit(unsigned char ch, char bit_num, char value) {
	unsigned char mask = ((unsigned char)128) >> bit_num;
	return value ? (mask | ch) : (~mask & ch);
}

static int encode_header(char *output) {
	int len = 0;
	int i;
	int k;
	unsigned char byte = 0;
	int bit_ctr = 0;

	*output++ = tree_data_len;
	for(i = 0; i < tree_data_len; i++) {
		*output++ = tree_data[i].ch;
		*output++ = tree_data[i].code_len;
		len += 2;
		for(k = 0; k < tree_data[i].code_len; k++) {
			set_bit(byte, bit_ctr, tree_data[i].code[k]);
			if(bit_ctr == 7) {
				*output++ = byte;
				len++;
				bit_ctr = 0;
				byte = 0;
			} else
				bit_ctr++;
		}
		if(bit_ctr < 7) {
			*output++ = byte;
			len++;
		}
	}

	return len;
}

static int gen_tree_bit_sequence(char *input, char *output) {
	char ch;
	int k;
	unsigned char byte = 0;
	int bit_cnt = 0;
	int byte_ctr = 0;
	int input_len = strlen(input);

	while(ch = *input++) {
		int i = tree_hash[ch];
		printf("Processing char '%c' tree index %d\n", ch, i);

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

static void dec_to_binary(unsigned char c, char *str) {
	char i = 0;
	for(i = 0; i < 8; i++)
		str[i] = (c & (((unsigned char)0x80) >> i)) ? '1' : '0';
	str[8] = 0;
}

static void print_bit_sequence(char *data, int len) {
	int i;
	char str[10];

	for(i = 0; i < len; i++) {
		dec_to_binary(data[i], str);
		printf("%s", str);
	}
	printf("\n");
}

/*---- encoder ----*/

int encode_Huffman(char *input, char *output) {
	int len_header;
	int len_data;

	count_freq(input);
	init_tree();
	assign_tree_codes();
	print_tree();
	len_header = encode_header(output);
	len_data = gen_tree_bit_sequence(input, output + len_header);
	print_bit_sequence(output, len_header + len_data);

	return len_header + len_data;
}

/*---- decoder ----*/

void decode_Huffman(char *input, char *output) {
	int i;
	int k;
	tree_data_len = *input++;
	unsigned char byte = 0;
	char bit_cnt = 0;
	
	for(i = 0; i < tree_data_len; i++) {
		tree_data[i].ch = *input++;
		tree_data[i].code_len = *input++;
		byte = *input++;
		for(k = 0; k < tree_data[i].code_len; k++) {
			tree_data[i].code[k] = get_bit(byte, bit_cnt++);
		}
	}

}












