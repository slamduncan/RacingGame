#ifndef __OBJLIST_H
#define __OBJLIST_H

typedef struct
{
	int item_count;
	int current_max_size;
	char growable;

	void **items;
	char **names;	
} objlist;

void list_make(objlist *listo, int size, char growable);
int list_add_item(objlist *listo, void *item, char *name);
char* list_print_items(objlist *listo);
void* list_get_name(objlist *listo, char *name);
void* list_get_index(objlist *listo, int indx);
void* list_get_item(objlist *listo, void *item_to_find);
int list_find(objlist *listo, char *name_to_find);
void list_delete_index(objlist *listo, int indx);
void list_delete_name(objlist *listo, char *name);
void list_delete_item(objlist *listo, void *item);
void list_delete_all(objlist *listo);
void list_print_list(objlist *listo);
void list_free(objlist *listo);

void test_list();
#endif
