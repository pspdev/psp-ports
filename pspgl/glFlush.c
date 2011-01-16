#include "pspgl_internal.h"
#include "pspgl_dlist.h"

void glFlush (void)
{
	__pspgl_dlist_submit();
}
