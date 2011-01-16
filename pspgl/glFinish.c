#include "pspgl_internal.h"
#include "pspgl_dlist.h"

void glFinish (void)
{
	__pspgl_dlist_submit();
	__pspgl_dlist_await_completion(NULL, NULL);
}
