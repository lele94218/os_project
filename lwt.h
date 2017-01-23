#ifndef LWT_H
#define LWT_H

/* Data redefinition */

/* Thread id */
typedef int t_id;

/* define a function pointer */
typedef void * (*lwt_fn_t)(void *);

/* Thread state.  */
typedef enum _lwt_info_t
{
    /* The thread is running. */
    LWT_INFO_NTHD_RUNNABLE= 0,
    /* The thread is blocked. */
    LWT_INFO_NTHD_BLOCKED,
    /* This is a zombie thread. */
    LWT_INFO_NTHD_ZOMBIES
}
lwt_info_t;

/* This structure describes a LWT thread. */
typedef struct _lwt_t
{
    /* Thread id */
    t_id lwt_id;
    
    /* Thread stack */
    void * stack_addr;
    size_t stack_size;
    
    lwt_info_t * status;
}
* lwt_t;

/* Global variable */
int initiated=0;
lwt_t queue_head=0;
lwt_t queue_head=

/* Funciton declaration */
lwt_t lwt_create(lwt_fn_t fn, void *data);
void * lwt_join(lwt_t);
void lwt_die(void *);
int lwt_yield(lwt_t);
lwt_t lwt_current(void);
int lwt_id(lwt_t);
int lwt_info(lwt_info_t t);
void damn();


#endif





