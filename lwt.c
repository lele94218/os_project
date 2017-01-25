#include <stdio.h>
#include <stdlib.h>

#include "lwt.h"
#include "lwt_dispatch.h"

/* Global variable */
int lwt_counter = 0;
int thread_initiated = 0;

lwt_context schedule_context;
extern linked_list thread_queue;
lwt_t * current_thread;


/** extern function declaration */
void __lwt_schedule (void);
lwt_t *  __get_next_thread ();
int __add_thread_to_list (lwt_t ** thread);
int __delete_thread_to_list (lwt_t * thread, linked_list * list);
static void __initiate(void);

int
__add_thread_to_list (lwt_t ** thread)
{
    
    linked_list_node * node = (linked_list_node *) malloc (sizeof (linked_list_node));
    node->data = *thread;
    node->next = NULL;
    
    if (!thread_queue.node_count)
    {
        thread_queue.head = node;
        thread_queue.tail = node;
    }
    else
    {
        thread_queue.tail->next = node;
        node->prev = thread_queue.tail;
        thread_queue.tail = node;
    }
    
    ++ thread_queue.node_count;
    return thread_queue.node_count - 1;
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
__get_next_thread ()
{
    linked_list_node * curr = thread_queue.tail;
    // TODO scheduling
    while (curr && curr != thread_queue.head)
    {
        if (curr->data->status == LWT_INFO_NTHD_RUNNABLE){
            return curr->data;
        }
        curr = curr->prev;
    }
    return NULL;
}

void
__lwt_schedule ()
{
    while (1)
    {
        current_thread = __get_next_thread(&thread_queue);
        if (current_thread)
            __lwt_dispatch(&schedule_context, &current_thread->context);
    }
}

static void
__initiate()
{
    thread_initiated = 1;
    
    /* Add main thread to TCB */
    current_thread = (lwt_t * ) malloc (sizeof(lwt_t));
    current_thread->lwt_id = lwt_counter ++;
    current_thread->status = LWT_INFO_NTHD_RUNNABLE;
    
    /* Add to TCB */
    __add_thread_to_list(&current_thread);
    
    /* Initiate schedule_context */
    uint _sp = (uint) malloc(MAX_STACK_SIZE);
    _sp += (MAX_STACK_SIZE - sizeof(uint));
    *((uint *)_sp) = (uint)__lwt_schedule;
    schedule_context.sp = _sp;
    schedule_context.ip = (uint) __lwt_schedule;

}

lwt_t *
lwt_create(lwt_fn_t fn, void * data)
{
    if(!thread_initiated) __initiate();
    lwt_t * next_thread = (lwt_t *) malloc (sizeof(lwt_t));
    
    /* Return lwt_die */
    uint _sp = (uint) malloc(MAX_STACK_SIZE);
    _sp += (MAX_STACK_SIZE - sizeof(uint));
    *((uint *)_sp) = (uint)NULL;
    _sp -= (sizeof(uint));
    *((uint *)_sp) = (uint)lwt_die;
    
    
    
    /* Construct new thread */
    next_thread->lwt_id = lwt_counter ++;
    next_thread->status = LWT_INFO_NTHD_RUNNABLE;
    next_thread->context.sp = _sp;
    next_thread->context.ip = (uint) fn;
    
    
    __add_thread_to_list(&next_thread);
    
    __lwt_dispatch(&current_thread->context, &schedule_context);
    
    return next_thread;
}

void
lwt_die(void * p_thread)
{
    if (!p_thread)
    {
        /* lwt_die(NULL) */
        current_thread->status = LWT_INFO_NTHD_ZOMBIES;
        
    }
    else
    {
        /* TODO kill specialfic thread */
    }
    
    //thread_queue.node_count --;
    
    __lwt_schedule();
}
