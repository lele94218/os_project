#include <stdio.h>
#include <stdlib.h>

#include "lwt.h"
#include "lwt_dispatch.h"


/* Global variable */
int thread_initiated = 0;
linked_list * thread_queue;

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
	printf("this is a test damn\n");
}

static void initiate()
{
	thread_initiated = 1;
	thread_queue = (linked_list * ) malloc (sizeof(linked_list));
    lwt_t * main_thread = (lwt_t * ) malloc (sizeof(lwt_t));
    // TODO init thread
    add_thread_to_list(main_thread, thread_queue);
}

int
lwt_create(lwt_fn_t fn, void * data)
{
	if(!thread_initiated) initiate();
	int return_value = 1;
    lwt_t * next_thread = (lwt_t *) malloc(sizeof(lwt_t));
    next_thread->context = (struct lwt_context *) malloc (sizeof(struct lwt_context));
    next_thread->stack_addr = (void *) malloc(sizeof(100));
    next_thread->context->ip = (unsigned long) fn;
//    __lwt_dispatch(struct lwt_context *curr, <#struct lwt_context *next#>)

	return return_value;
}
