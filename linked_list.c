#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "stdlib.h"

#include "linked_list.h"

unsigned int LL_get_num_items(LinkedList *target_linked_list){
    /* Return how many items there are in the list  */
    unsigned int res = target_linked_list->number_of_items;
    return res;
}

void LL_init(LinkedList *target_linked_list){
    /* Initialize target_linked_list by setting all of its members
       to 0 and/or NUL 
    */
    target_linked_list->number_of_items = 0;
    target_linked_list->head_ptr = NULL;
    target_linked_list->tail_ptr = NULL;
}

bool LL_is_empty(LinkedList *target_linked_list){
    /* Return True if the list is empty and False otherwise  */
    bool res;
    res = (target_linked_list->number_of_items) ?  true : false;
    return res;
}


LinkedList_node *LL_build_node(void){
    /* Malloc'ates memory for a LinkedList_node and returns a pointer to it */
    LinkedList_node *node;
    void *temp = malloc(sizeof(LinkedList_node));
    assert(temp != NULL);   // raise an error if temp is null;
    node = temp; 
    node->previous = NULL;
    node->next = NULL;
    return node;
}

void LL_append(LinkedList *target_linked_list, char val){
    /* Append val to the list, i.e. make it the new tail  */
    LinkedList_node *node = LL_build_node();    // build a new node;

    if (target_linked_list->number_of_items > 0){   // if the list isn't empty
        target_linked_list->tail_ptr->next = node;  // make the current tail point to the soon-to-be new tail
        node->previous = target_linked_list->tail_ptr;  // make node's prev pointer point to the current tail;
        target_linked_list->tail_ptr = node;        // make node the new tail
        target_linked_list->tail_ptr->data = val;
        target_linked_list->number_of_items++;
    }
    else{   // if the list is empty, there being 0 items
        target_linked_list->head_ptr = node;    // make node both the tail and pointer;
        target_linked_list->tail_ptr = node;
        target_linked_list->number_of_items++;
        target_linked_list->tail_ptr->data = val;
    }
}


void LL_prepend(LinkedList *target_linked_list, char val){
    /* Prepend val to the list, i.e. make it the new head */
    assert(target_linked_list->number_of_items > 0);    // can't prepend if there are no items - raise an exception if attempted;
    LinkedList_node *node = LL_build_node();    // build a new node;
    if (target_linked_list->number_of_items > 0){   // if the list isn't empty
        target_linked_list->head_ptr->previous = node;  // make the current head point to the soon-to-be new head 
        node->next= target_linked_list->head_ptr;  // make node's next pointer point to the current tail;
        target_linked_list->head_ptr= node;        // make node the new head 
        target_linked_list->head_ptr->data = val;
        target_linked_list->number_of_items++;
    }
}



void LL_destroy(LinkedList *target_linked_list){
    /* Sets all the next and prev pointers to null so that the soace can
       be automatically reclaimed */
    LinkedList_node *current_ptr = target_linked_list->head_ptr; 
    LinkedList_node *last_node_standing = target_linked_list->tail_ptr;
    while(target_linked_list->number_of_items != 1){    // one node will be left standing after the loop -- the tail -- 
                                                        // since thre's no node that has a 'prev' pointing to it
        current_ptr = current_ptr->next;    // keep destroying the object pointed to by 'previous'
        LL_destroy_node(current_ptr->previous);
        target_linked_list->number_of_items--;
    }
    LL_destroy_node(last_node_standing);
    target_linked_list->number_of_items--;
    target_linked_list->head_ptr = NULL;
    target_linked_list->tail_ptr=NULL;
}


void LL_destroy_node(LinkedList_node *node){
    /* Call free and deallocate the memory for *node  
       Not meant to be called directly.
    */
    free(node);
}


char LL_head_pop(LinkedList *target_linked_list){
    /* Remove and return the value of the current head */
    char val = target_linked_list->head_ptr->data; 
    LinkedList_node *former_head = target_linked_list->head_ptr;
    target_linked_list->head_ptr = target_linked_list->head_ptr->next;
    target_linked_list->head_ptr->previous = NULL;
    LL_destroy_node(former_head);
    target_linked_list->number_of_items--;

    return val;
}


char LL_tail_pop(LinkedList *target_linked_list){
    /* Remove and return the current tail */
    char val = target_linked_list->tail_ptr->data; 
    LinkedList_node *former_tail = target_linked_list->tail_ptr;
    target_linked_list->tail_ptr = target_linked_list->tail_ptr->previous;
    target_linked_list->tail_ptr->next= NULL;
    LL_destroy_node(former_tail);
    target_linked_list->number_of_items--;

    return val;
}





