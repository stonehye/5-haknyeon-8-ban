#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitmap.c"
#include "hash.c"
#include "list.c"

#define max_num 11 /* Maximum number of lists, hash tables, and bitmaps */

struct list_data{
	struct list_elem elem;
	int data;
};

struct hash_data{
	struct hash_elem elem;
	int data;
};
int command(char **token, struct list *list, struct hash *hash);

int list_num = 0;
int hash_num = 0;
int bitmap_num = 0;
char list_name[max_num][100];
char hash_name[max_num][100];
char bitmap_name[max_num][100];

unsigned hash_hash_func (const struct hash_elem *e, void *aux);
bool hash_less_func (const struct hash_elem *a, const struct hash_elem *b, void *aux);
int command(char *token, struct list *list, struct hash *hash);
void print_list(struct list *list);
void print_hash(struct hash *hash);
void delete_list(struct list *list, int index);
void delete_hash(struct hash *hash, int index);
void delete_bitmap(struct bitmap *bitmap, i);
void hash_action_function_square(struct hash_elem *e, void *aux);
void hash_action_function_triple(struct hash_elem *e, void *aux);