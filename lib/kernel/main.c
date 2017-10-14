#include "main.h"

int main(){
	char string[100] = {0};
	char *token[100];
	int i;
	struct list list[max_num];
	struct hash hash[max_num];

	while(1){
		gets(string);


		if (strcmp(string, "quit")==0){
			return;
		}
	
		token[0] = strtok(string, " ");
		for (i=0 ; token[i]!=NULL ; i++)
			token[i+1] = strtok(NULL, " ");
		
		command(token, list, hash);
 	}

	return 0;
}

int command (char *token, struct list *list, struct hash *hash){
	// variables
	int i. j;
	struct list_data *data;
	struct list_elem *before, *front, *last;

	struct hash_elem *h_elem;
	struct hash_data *h_data;

	struct bitmap *bitmap[max_num];

	
	//// create ////
	if (strcmp(token[0], "create")==0){
		if (strcmp(token[1], "list") == 0){
			if (list_num >= max_num){
				// exception handling
				return;
			}
			strcpy(list_name[list_num], token[2]);
			list_init(&list[list_num++]);
		}
		else if (strcmp(token[1], "hashtable") == 0){
			if (hash_num >= max_num){
                                // exception handling
                                return;
                        }
			strcpy(hash_name[hash_num], token[2]);
			hash_init(&hash[hash_num++], hash_hash_func, hash_less_function, NULL);
		}
		else if (strcmp(token[1], "bitmap") == 0){
			if (bitmap_num >= max_num){
                // exception handling
				return;
            }
			strcpy(bitmap_name[bitmap_num], token[2]);
			bitmap[bitnum++] = bitmap_create((size_t)atoi(token[3]));
		}
	}
	//// dumpdata ////
	else if (strcmp(token[0], "dumpdata") == 0){
		for (i=0 ; i<max_num ; i++){
			if (strcmp(list_name[i], token[1]) == 0){
				if (list_empty(&list[i])!=1){
					print_list(&list[i]);
				}
				break;
			}
			else if (strcmp(hash_name[i], token[1]) == 0){
                if (hash_empty(&hash[i])!=1){
                    print_hash(&hash[i]);
                }
                break;
            }
			else if (strcmp(bitmap_name[i], token[1]) == 0){
				for(j=0 ; j<(int)bitmap_size(bitmap[i]) ; j++){
					print("%d", bitmap_test(bitmap[i], j));
                }
				print("\n");
				break;
			}
		}
	}
	//// delete ////
	else if (strcmp(token[0], "delete") == 0){
		for (i=0 ; i<max_num ; i++){
			if (strcmp(list_name[i], token[1]) == 0){
				delete_list(&list[i], i);
				break;
			}
			else if (strcmp(hash_name[i], token[1]) == 0) {
				delete_hash(&hash[i], i);
				break;
			}
			else if (strcmp(bitmap_name[i], token[1]) == 0){
				delete_bitmap(&bitmap[i], i);
				break;
			}
		}
	}
	//////////////////////////////////////// LIST ////////////////////////////////////////
	//// list_insert ////
	else if (strcmp(token[0], "list_insert") == 0){
		for(i=0 ; i<list_num ; i++){
			if (strcmp(list_name[i], token[1]) == 0){
				data = (struct list_data*)malloc(sizeof(struct list_data*));
				data->data = atoi(token[3]);
				before = list_begin(&list[i]);

				for(j=0 ; j<atoi(token[2]) ; j++)
					before = list_next(before);
				list_insert(before, &data->elem);
				break;
			}
		}
	}
	//// list_splice ////
	else if (strcmp(token[0], "list_splice") == 0){
		for (i=0 ; i<list_num ; i++){
			if (strcmp(list_name[i], token[1]) == 0){
				// 붙일 리스트가 저장되어있는 리스트라면 추가할 부분을 새로 가리킨다.
				before = list_begin(&list[i]);
				for (j=0 ; j<atoi(token[2]) ; j++)
					before = list_next(before);

				for (i=0 ; i<list_num ; i++){
					if (strcmp(list_name[i], token[3]) == 0){
						// 떼어낼 리스트가 저장되어있는 리스트라면 떼어낼 부분의 처음과 끝을 새로 가리킨다.
						front = list_begin(&list[i]);
						last = list_begin(&list[i]);
						// front
						for (j=0 ; j<atoi(token[4]) ; j++)
							front = list_next(front);
						// last
						for (j=0 ; j<atoi(token[5]) ; j++)
							last = list_next(last);

						list_splice(before, front, last);
						break;
					}
				}
				break;
			}
		}
	}
	//// list_push_front ////
	else if (strcmp(token[0], "list_push_front") == 0){
		for(i=0 ; i<list_num ; i++){
			if (strcmp(list_name[i], token[1]) == 0){
				data = (struct list_data*)malloc(sizeof(struct list_data*));
				data->data = atoi(token[2]);
				list_push_front(&list[i], &data->elem);
				break;
			}
		}
	}
	//// list_push_back ////
	else if (strcmp(token[0], "list_push_back") == 0){
		for(i=0 ; i<list_num ; i++){
			if (strcmp(list_name[i], token[1]) == 0){
				data = (struct list_data*)malloc(sizeof(struct list_data*));
				data->data = atoi(token[2]);
				list_push_back(&list[i], &data->elem);
				break;
			}
		}
	}
	//// list_remove ////
	else if (strcmp(token[0], "list_remove") == 0){
		for(i=0 ; i<list_num ; i++){
			if (strcmp(list_name[i], token[1]) == 0){
				begin = list_begin(&list[i]);
				for (j=0 ; j<token[2] ;j++){
					begin = list_next(begin);
				}
				list_remove(begin);
				break;
			}
		}
	}
	//// list_pop_front ////
	else if (strcmp(token[0], "list_pop_front") == 0){
		for(i=0;i<list_num;i++){
			if(strcmp(list_name[i], token[1])==0){
				list_pop_front(&list[i]);
			break;
		}			        
	}
	//// list_pop_back ////
	else if (strcmp(token[0], "list_pop_back") == 0){
		for(i=0;i<list_num;i++){
			if(strcmp(list_name[i], token[1])==0){
				list_pop_back(&list[i]);
			break;
		}
	}
	//// list_front ////
	else if (strcmp(token[0], "list_front") == 0){
		for (i=0 ; i<list_num ; i++){
			if (strcmp(listn_name[i], token[1]) == 0){
				print("%d\n", list_entry(list_front(&list[i]), struct list_data, elem)->data);
				break;
			}
		}
	}
	//// list_back ////
	else if(strcmp(token[0], "list_back")==0){
		for(i=0;i<list_num;i++){
			if(strcmp(list_name[i], token[1])==0){
				printf("%d\n",list_entry(list_back(&list[i]),struct list_data, elem)->data);
				break;
			}
		}
	}
	//// list_size ////
	else if(strcmp(token[0], "list_size")==0){
		for(i=0;i<list_num;i++){
			if(strcmp(list_name[i], token[1])==0){
				printf("%d\n",list_size(&list[i]));
				break;
			}
		}
	}
	//// list_empty ////
	else if(strcmp(token[0], "list_empty")==0){
		for(i=0;i<list_num;i++){
			if(strcmp(list_name[i], token[1])==0){
				if(list_empty(&list[i])==1)
					printf("true\n");
				else
					printf("false\n");
				break;
			}
		}
	}
	//// list_reverse ////
	else if(strcmp(token[0], "list_reverse")==0){
		for(i=0;i<list_num;i++){
			if(strcmp(list_name[i], token[1])==0)
				list_reverse(&list[i]);
		}
	}
	//// list_sort ////
	else if(strcmp(token[0], "list_sort")==0){
		for(i=0;i<list_num;i++){
			if(strcmp(list_name[i], token[1])==0)
				list_reverse(&list[i]);
		}
	}
	//// list_insert_ordered ////
	else if (strcmp(token[0], "list_insert_ordered") ==0){
		for (i = 0; i<list_num; i++){
			if (strcmp(list_name[i], token[1]) == 0){
				data = (struct list_data *)malloc(sizeof(struct list_data *));
				data->data = atoi(token[2]);
				list_insert_ordered(&list[i], &data->elem, list_less_function, NULL);
				break;
			}
		}
	}
	//// list_unique ////
	else if (strcmp(token[0], "list_unique") == 0){
		for (i = 0; i<list_num; i++){
			if (strcmp(list_name[i], token[1]) == 0){
				if (token[2] != NULL){
					for (j = 0; j<list_num; j++){
						if (strcmp(list_name[j], token[2]) == 0){
							list_unique(&list[i], &list[j], list_less_function, NULL);
						}
					}
				}
				else{
					list_unique(&list[i], NULL, list_less_function, NULL);
				}
			}
		}
	}
	//// list_max ////
	else if (strcmp(token[0], "list_max") == 0){
		for (i = 0; i<list_num; i++){
			if (strcmp(list_name[i], token[1]) == 0){
				printf("%d\n", list_entry(list_max(&list[i], list_less_function, NULL), struct list_data, elem)->data);
				break;
			}
		}
	}
	//// list_min ////
	else if (strcmp(token[0], "list_min") == 0){
		for (i = 0; i<list_num; i++){
			if (strcmp(list_name[i], token[1]) == 0){
				printf("%d\n", list_entry(list_min(&list[i], list_less_function, NULL), struct list_data, elem)->data);
				break;
			}
		}
	}

	//////////////////////////////////////// HASHTABLE ////////////////////////////////////////
	//// hash_insert ////
	else if (strcmp(token[0], "hash_insert") == 0){
		for (i = 0; i<hash_num; i++){
			if (strcmp(hash_name[i], token[1]) == 0){
				h_data = (struct hash_data *)malloc(sizeof(struct hash_data *));
				h_data->data = atoi(token[2]);
				hash_insert(&hash[i], &h_data->elem);
			}
		}
	}
	//// hash_replace ////
	else if (strcmp(token[0], "hash_replace") == 0){
		for (i = 0; i<hash_num; i++){
			if (strcmp(hash_name[i], token[1]) == 0){
				h_data = (struct hash_data *)malloc(sizeof(struct hash_data *));
				h_data->data = atoi(token[2]);
				hash_replace(&hash[i], &h_data->elem);
			}
		}
	}
	//// hash_find ////
	else if (strcmp(token[0], "hash_find") == 0){
		for (i = 0; i<hash_num; i++){
			if (strcmp(hash_name[i], token[1]) == 0){
				h_data = (struct hash_data *)malloc(sizeof(struct hash_data *));
				h_data->data = atoi(token[2]);
				h_elem = hash_find(&hash[i], &h_data->elem);
				if (hash_elem != NULL)
					printf("%d\n", hash_entry(hash_elem, struct hash_data, elem)->data);
			} 
		}
	}
	else if (strcmp(token[0], "hash_delete") == 0){
		for (i = 0; i<hash_num; i++){
			if (strcmp(hash_name[i], token[1]) == 0){
				h_data = (struct hash_data *)malloc(sizeof(struct hash_data *));
				h_data->data = atoi(token[2]);
				hash_delete(&hash[i], &h_data->elem);
			}
		}
	}
	else if (strcmp(token[0], "hash_clear") == 0){
		for (i = 0; i<hash_num; i++){
			if (strcmp(hash_name[i], token[1]) == 0){
				hash_clear(&hash[i], NULL);
			}
		}
	}
	else if (strcmp(token[0], "hash_size") == 0){
		for (i = 0; i<hash_num; i++){
			if (strcmp(hash_name[i], token[1]) == 0){
				printf("%d\n", hash_size(&hash[i]));
			}
		}
	}
	// 수정이 필요합니다!
	else if (strcmp(token[0], "hash_apply") == 0){
		for (i = 0; i<hash_num; i++){
			if (strcmp(hash_name[i], token[1]) == 0){
				if (strcmp(token[2], "square") == 0){
					hash_apply(&hash[i], hash_action_function_square);
				}
				else if (strcmp(token[2], "triple") == 0){
					hash_apply(&hash[i], hash_action_function_triple);
				}
			}
		}
	}
	
	//////////////////////////////////////// BITMAP ////////////////////////////////////////
	else if (strcmp(token[0], "bitmap_mark") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				bitmap_mark(bitmap[i], (size_t)atoi(token[2]));
			}
		}
	}
	else if (strcmp(token[0], "bitmap_expand") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				bitmap_expand(bitmap[i], atoi(token[2]));
			}
		}
	}
	else if (strcmp(token[0], "bitmap_size") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				printf("%u\n", bitmap_size(bitmap[i]));
			}
		}
	}
	else if (strcmp(token[0], "bitmap_any") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				if (bitmap_any(bitmap[i], atoi(token[2]), atoi(token[3])) == 1){
					printf("true\n");
				}
				else{
					printf("false\n");
				}
			}
		}
	}
	else if (strcmp(token[0], "bitmap_all") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				if (bitmap_all(bitmap[i], atoi(token[2]), atoi(token[3])) == 1){
					printf("true\n");
				}
				else{
					printf("false\n");
				}
			}
		}
	}
	else if (strcmp(token[0], "bitmap_test") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				if (bitmap_test(bitmap[i], atoi(token[2])) == 1){
					printf("true\n");
				}
				else{
					printf("false\n");
				}
			}
		}
	}
	else if (strcmp(token[0], "bitmap_none") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				if (bitmap_none(bitmap[i], atoi(token[2]), atoi(token[3])) == 1){
					printf("true\n");
				}
				else{
					printf("false\n");
				}
			}
		}
	}
	else if (strcmp(token[0], "bitmap_dump") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				bitmap_dump(bitmap[i]);
				break;
			}
		}
	}
	else if (strcmp(token[0], "bitmap_count") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				if (strcmp(token[4], "true") == 0){
					printf("%d\n", bitmap_count(bitmap[i], atoi(token[2]),
						atoi(token[3]), 1));
				}
				else{
					printf("%d\n", bitmap_count(bitmap[i], atoi(token[2]),
						atoi(token[3]), 0));
				}
				break;
			}
		}
	}
	else if (strcmp(token[0], "bitmap_scan") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				if (strcmp(token[4], "true") == 0){
					printf("%u\n", bitmap_scan(bitmap[i], atoi(token[2]),
						atoi(token[3]), 1));
				}
				else{
					printf("%u\n", bitmap_scan(bitmap[i], atoi(token[2]),
						atoi(token[3]), 0));
				}
				break;
			}
		}
	}
	else if (strcmp(token[0], "bitmap_set") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				if (strcmp(token[3], "true") == 0){
					bitmap_set(bitmap[i], atoi(token[2]), 1);
				}
				else{
					bitmap_set(bitmap[i], atoi(token[2]), 0);
				}
				break;
			}
		}
	}
	else if (strcmp(token[0], "bitmap_set_multiple") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				if (strcmp(token[4], "true") == 0){
					bitmap_set_multiple(bitmap[i], atoi(token[2]), atoi(token[3]), 1);
				}
				else{
					bitmap_set_multiple(bitmap[i], atoi(token[2]), atoi(token[3]), 0);
				}
				break;
			}
		}
	}
	else if (strcmp(token[0], "bitmap_set_all") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				if (strcmp(token[2], "true") == 0){
					bitmap_set_all(bitmap[i], 1);
				}
				else{
					bitmap_set_all(bitmap[i], 0);
				}
				break;
			}
		}
	}
	else if (strcmp(token[0], "bitmap_scan_and_flip") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				if (strcmp(token[4], "true") == 0){
					printf("%u\n", bitmap_scan_and_flip(bitmap[i], atoi(token[2]),
						atoi(token[3]), 1));
				}
				else{
					printf("%u\n", bitmap_scan_and_flip(bitmap[i], atoi(token[2]),
						atoi(token[3]), 0));
				}
				break;
			}
		}
	}
	else if (strcmp(token[0], "bitmap_contains") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				if (strcmp(token[4], "true") == 0){
					if (bitmap_contains(bitmap[i], atoi(token[2]),
						atoi(token[3]), 1) == 1){
						printf("true\n");
					}
					else{
						printf("false\n");
					}
				}
				else{
					if (bitmap_contains(bitmap[i], atoi(token[2]),
						atoi(token[3]), 0) == 1){
						printf("true\n");
					}
					else{
						printf("false\n");
					}
				}
				break;
			}
		}
	}
	else if (strcmp(token[0], "bitmap_flip") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				bitmap_flip(bitmap[i], atoi(token[2]));
				break;
			}
		}
	}
	else if (strcmp(token[0], "bitmap_reset") == 0){
		for (i = 0; i<bit_num; i++){
			if (strcmp(bit_name[i], token[1]) == 0){
				bitmap_reset(bitmap[i], atoi(token[2]));
				break;
			}
		}
	}

	return 0;
}
/*********************************************************************************************/

/* Converts data into hash element to use hash_int */
unsigned hash_hash_function(const struct hash_elem *e, void *aux){
    struct hash_data *temp = hash_entry(e, struct hash_data, elem);
    return hash_int(temp->data);
}

/* Compare the value of 2 hash elements a and b */
bool hash_less_function(const struct hash_elem *a,
                        const struct hash_elem *b,
                        void *aux){
    struct hash_data *a_temp, *b_temp;

    a_temp = hash_entry(a, struct hash_data, elem);
    b_temp = hash_entry(b, struct hash_data, elem);

    if(a_temp->data < b_temp->data)
        return true;
    else
        return false;
}

void print_list (struct list *list){
	struct list_elem *temp;
	struct list_data *data;

	for (temp = list_begin(list) ; temp != list_end(list) ; temp = list_next(temp)){
		data = list_entry(temp, struct list_data, elem);
		print("%d ", data->data);
	}
	print("\n");	
}

void print_hash (struct hash *hash){
	struct hash_iterator iterator;

    	hash_first(&iterator, hash);
    	while(hash_next(&iterator)){
        	printf("%d ",hash_entry(hash_cur(&iterator), struct hash_data, elem)->data);
    	}
    	printf("\n");
}

void delete_list(struct list *list, int index){
	int i;
	struct list_data *data;
	struct list_elem *temp, *del_temp;

	for (i=index ; i<list_num-1 ; i++)
		strcpy(list_name[i], list_name[i+1]);
	list_num--;

	
	temp = list_begin(list);
	while(temp!=list_end(list)){
		del_temp = temp;
		temp = list_next(temp);
		free(list_entry(del_temp, struct list_data, elem));
	}
}

void delete_hash(struct hash *hash, int index){
	int i;
	struct hash_iterator iterator;

	for (i=index ; i<hash_num-1 ; i++)
		strcpy(hash_name[i], hash_name[i+1]);
	hash_num--;

	hash_first(&iterator, hash);
	while (hash_next (&iterator)){
		free(hash_entry(hash_cur(&iterator), struct hash_data, elem));
	}
}

void delete_bitmap(struct bitmap *bitmap, i)
{
	int i;

	for (i=index ; i<bitmap_num-1 ; i++){
		strcpy(bitmap_name[i], bitmap_name[i+1]);
		// 수정이 필요합니다!
		bitmap[i]=bitmap[i+1];
	}
	bitmap_num--;
}
// 수정이 필요합니다!
void hash_action_function_square(struct hash_elem *e, void *aux){
	int data;

	data = hash_entry(e, struct hash_data, elem)->data;
	hash_entry(e, struct hash_data, elem)->data = data*data;

}
void hash_action_function_triple(struct hash_elem *e, void *aux){
	int data;

	data = hash_entry(e, struct hash_data, elem)->data;
	hash_entry(e, struct hash_data, elem)->data = data*data*data;

}