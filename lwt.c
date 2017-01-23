#include <stdio.h>
#include <stdlib.h>

#include "lwt.h"

/* LinkedListNode definiation */
typedef struct _linked_list_node
{
    lwt_t * data;
    struct _linked_list_node * next;
    struct _linked_list_node * prev;
}
linked_list_node;



/* LinkedList definiation */
typedef struct _linked_list
{
    linked_list_node *head, *tail;
    int node_count;
}
linked_list;

int
add_thread_to_list (lwt_t * thread, linked_list * list)
{
    linked_list_node * node = (linked_list_node *) malloc (sizeof (linked_list_node));
    node->data = thread;
    node->next = NULL;
    
    if (!list->node_count)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;
        list->tail = node;
    }
    
    ++ list->node_count;
    return list->node_count - 1;
}

int
delete_thread_to_list (lwt_t * thread, linked_list * list)
{
    linked_list_node * curr = list->head;
    //linked_list_node * temp = NULL;
    while (curr)
    {
        if (thread->lwt_id == curr->data->lwt_id)
        {
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            free(curr);
            return 1;
        }
    }
    return 0;
}



void damn(){
	printf("fuck\n");
}

static void initiate()
{


}

lwt_t *
lwt_create(lwt_fn_t fn, void * data)
{



}
