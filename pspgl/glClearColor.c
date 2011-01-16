#include "pspgl_internal.h"

void glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	pspgl_curctx->clear.color = ((((int) (255.0 * CLAMPF(alpha))) << 24) |
				     (((int) (255.0 * CLAMPF(blue))) << 16) |
				     (((int) (255.0 * CLAMPF(green))) << 8) |
				     ((int) (255.0 * CLAMPF(red))));
}

