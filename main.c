/* standard include */
#include <stdio.h>
#include <stdlib.h>

/* user lib include */
#include "lwt.h"
#include "lwt_dispatch.h"

void fun();
int main();
void fun()
{
    printf("fun() \n");
}

int main()
{
    lwt_t * t1 = lwt_create(fun, NULL);
    
}
