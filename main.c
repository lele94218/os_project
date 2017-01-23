/* standard include */
#include <stdio.h>
#include <stdlib.h>

/* user lib include */
//#include "lwt.h"
#include "lwt_dispatch.h"

void fun();
int main();
void fun()
{
    printf("fun() \n");
}

int main()
{
    printf("main() \n");
    
    struct lwt_context * main_context = (struct lwt_context *) malloc (sizeof (struct lwt_context));
    main_context->ip = (unsigned long) main;
    main_context->sp = 0;
    
    struct lwt_context * next_context = (struct lwt_context *) malloc (sizeof (struct lwt_context));
    next_context->ip = (unsigned long) fun;
    next_context->sp = 0;
    
    __lwt_dispatch(main_context, next_context);
    
}
