#ifndef LWT_DISPATCH_H
#define LWT_DISPATCH_H

typedef struct _lwt_context
{
    unsigned int ip, sp;
}
lwt_context;
void __lwt_dispatch(lwt_context *curr, lwt_context *next);

#endif	/* LWT_DISPATCH_H */

