#ifndef __pspgl_dlist_h__
#define __pspgl_dlist_h__

#include "pspgl_misc.h"


/* Initialize the dlist subsystem */
extern void __pspgl_dlist_init(void);

/* flush pending buffers to hardware, and return asynchronously. */
extern void __pspgl_dlist_submit (void);

/* Wait until all commands in currently queued dlist are executed and
   drawn, or the predicate function returns true.  The predicate is
   called each time a buffer of commands has been finalized.  A NULL
   predicate is considered to always return False. */
extern void __pspgl_dlist_await_completion (int (*pred)(void *p), void *data);

/* cancel the currently running wait queue */
extern void __pspgl_dlist_cancel (void);

/* Pin a pspgl_buffer, indicating it is being used by hardware.  The
   buffer is unpinned when the GE has reached the current point in the
   commands stream.  The flags contain one or both of BF_PINNED_RD or
   BF_PINNED_WR to indicate whether the buffer is pinned for reading
   or writing. */
extern void __pspgl_dlist_pin_buffer(struct pspgl_buffer *data, unsigned flags);

/**
 *  check if dlist has room for (size) bytes embedded in cmd buf and
 *  advance cmd_buf pointer, useful e.g. for vertex data embedded in
 *  command stream.  Will go to next dlist if necessary to get the
 *  space.  Returned memory pointer is aligned to 16-byte boundaries.
 */
extern void * __pspgl_dlist_insert_space (unsigned long size);


/**
 *  insert cmd in dlist. If dlist is full, automatically go to the next one.
 */
extern void __pspgl_dlist_enqueue_cmd (unsigned long cmd);

#endif

