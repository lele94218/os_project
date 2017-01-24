#include <stdio.h>
#include <stdlib.h>

#include "lwt.h"
#include "lwt_dispatch.h"


/* Global variable */
int thread_initiated = 0;
linked_list * thread_queue = NULL;
lwt_t * current_thread = NULL;
struct lwt_context * p_schedule_context = NULL;

/** extern function declaration */
void __lwt_schedule (void);
void __get_next_thread (lwt_t *, linked_list *);
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
            free(curr);
            return 1;
        }
    }
    return 0;
}

void
__get_next_thread (lwt_t * p_thread, linked_list * list)
{
    linked_list_node * curr = list->tail->prev;
    // TODO scheduling
    p_thread = curr->data;
}

void
__lwt_schedule ()
{
    while (1)
    {
        printf("schedule()!\n");
        lwt_t * p_thread = NULL;
        __get_next_thread(p_thread, thread_queue);
        if (!p_thread)
            __lwt_dispatch(current_thread->context, p_thread->context);
    }
}

static void
__initiate()
{
    thread_initiated = 1;
    thread_queue = (linked_list * ) malloc (sizeof(linked_list));
    current_thread = (lwt_t * ) malloc (sizeof(lwt_t));
    current_thread->context = (struct lwt_context *) malloc(sizeof(struct lwt_context));
    current_thread->context->sp = (uint)malloc(100);
    ///
    __add_thread_to_list(current_thread, thread_queue);
    
    // Init schedule context
    p_schedule_context = (struct lwt_context *) malloc(sizeof(struct lwt_context));
    p_schedule_context->ip = (uint) __lwt_schedule;
    uint _sp = (uint) malloc (100);
    _sp += (100 - sizeof(uint));
    *((uint *)_sp) = (uint)__lwt_schedule;
    p_schedule_context->sp = _sp;
}

lwt_t *
lwt_create(lwt_fn_t fn, void * data)
{
    if(!thread_initiated) __initiate();
    int return_value = 1;
    lwt_t * next_thread = (lwt_t *) malloc(sizeof(lwt_t));
    next_thread->context = (struct lwt_context *) malloc (sizeof(struct lwt_context));
    uint _sp = (uint) malloc(100);
    _sp += (100 - sizeof(uint));
    *((uint *)_sp) = (uint)__lwt_schedule;
    next_thread->context->sp = _sp;
    next_thread->context->ip = (uint) fn;
    __add_thread_to_list(next_thread, thread_queue);
    
    __lwt_dispatch(current_thread->context, p_schedule_context);
    
    return next_thread;
}
