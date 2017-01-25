#include <stdio.h>
#include <stdlib.h>

#include "lwt.h"
#include "lwt_dispatch.h"

/* Global variable */
int thread_initiated = 0;
static lwt_context schedule_context;
static linked_list thread_queue;
static lwt_t * current_thread = NULL;

/** extern function declaration */
void __lwt_schedule (void);
lwt_t *  __get_next_thread (linked_list *);
int __add_thread_to_list (lwt_t * thread, linked_list * list);
int __delete_thread_to_list (lwt_t * thread, linked_list * list);
static void __initiate(void);

int
__add_thread_to_list (lwt_t * thread, linked_list * list)
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
        node->prev = list->tail;
        list->tail = node;
    }
    
    ++ list->node_count;
    return list->node_count - 1;
}

int
__delete_thread_to_list (lwt_t * thread, linked_list * list)
{
    linked_list_node * curr = list->head;
    //linked_list_node * temp = NULL;
    while (curr)
    {
        if (thread->lwt_id == curr->data->lwt_id)
        {
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            /* Free or not? */
            free(curr);
            return 1;
        }
    }
    return 0;
}

lwt_t *
__get_next_thread (linked_list * list)
{
    linked_list_node * curr = list->tail;
    // TODO scheduling
    return curr->data;
}

void
__lwt_schedule ()
{
    while (1)
    {
        lwt_t * p_thread;
        p_thread=__get_next_thread(&thread_queue);
        if (p_thread)
            __lwt_dispatch(&current_thread->context, &p_thread->context);
    }
}

static void
__initiate()
{
    thread_initiated = 1;
    
    /* Add main thread to TCB */
    current_thread = (lwt_t * ) malloc (sizeof(lwt_t));
    
    /* Add to TCB */
    __add_thread_to_list(current_thread, &thread_queue);
    
    /* Initiate schedule_context */
    uint _sp = (uint) malloc(100);
    _sp += (100 - sizeof(uint));
    *((uint *)_sp) = (uint)__lwt_schedule;
    schedule_context.sp = _sp;
    schedule_context.ip = (uint) __lwt_schedule;

}

lwt_t *
lwt_create(lwt_fn_t fn, void * data)
{
    if(!thread_initiated) __initiate();
    lwt_t * next_thread = (lwt_t *) malloc (sizeof(lwt_t));
    
    /* Default return schedule */
    uint _sp = (uint) malloc(100);
    _sp += (100 - sizeof(uint));
    *((uint *)_sp) = (uint)__lwt_schedule;
    next_thread->context.sp = _sp;
    next_thread->context.ip = (uint) fn;
    
    
    __add_thread_to_list(next_thread, &thread_queue);
    
    __lwt_dispatch(&current_thread->context, &schedule_context);
    
    return next_thread;
}
