/*
 *  main.h
 * os_project
 *
 *  Created by Taoran Xue on 1/21/17.
 *  Copyright © 2017 Taoran Xue. All rights reserved.
 */

#ifndef main_h
#define main_h


#endif /* main_h */

/* Data structure */

/* Thread state.  */
typedef enum _lwt_info_t
{
    /* The thread is running. */
    LWT_INFO_NTHD_RUNNABLE = 0,
    /* The thread is blocked. */
    LWT_INFO_NTHD_BLOCKED,
    /* This is a zombie thread. */
    LWT_INFO_NTHD_ZOMBIES
}
lwt_info_t;

/* Thread id */
typedef int p_id;


/* This structure describes a LWT thread. */
typedef struct _lwt_t
{
    /* Thread id */
    p_id lwt_id;
    
    /* Thread stack */
    void * stack_addr;
    size_t stack_size;
    int stack;
    
    void * status;
}
* lwt_t;

typedef void * (*lwt_fn_t)(void *);

/* Funciton declaration */
lwt_t lwt_create(lwt_fn_t fn, void *data);
void * lwt_join(lwt_t);
void lwt_die(void *);
int lwt_yield(lwt_t);
lwt_t lwt_current(void);
int lwt_id(lwt_t);
int lwt_info(lwt_info_t t);
