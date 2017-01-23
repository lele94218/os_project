#include <stdio.h>

#include "lwt.h"

void damn(){
	printf("fuck\n");
}

static void initiate()
{


}

lwt_t lwt_create(lwt_fn_t fn, void *data)
{
	if(!INITIATED) initiate();
	


}
