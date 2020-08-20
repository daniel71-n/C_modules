#include <stdbool.h>

typedef struct linked_list LinkedList;
typedef struct linked_list_node LinkedList_node;

struct linked_list{
LinkedList_node * head_ptr;
LinkedList_node * tail_ptr;
unsigned int number_of_items;
};

struct linked_list_node{
char data;
struct linked_list_node *previous;
struct linked_list_node *next;
};


void LL_init(LinkedList *target_linked_list);
unsigned int LL_get_num_items(LinkedList *linked_list_ptr);
bool LL_is_empty(LinkedList *target_linked_list);
void LL_destroy(LinkedList *target_linked_list);
void LL_destroy_node(LinkedList_node *node);
void LL_append(LinkedList *target_linked_list, char val);
void LL_prepend(LinkedList *target_linked_list, char val);
char LL_head_pop(LinkedList *target_linked_list);
char LL_tail_pop(LinkedList *target_linked_list);
void LL_destroy(LinkedList *tail_ptr);

