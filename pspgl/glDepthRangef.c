#include <GLES/gl.h>
#include "pspgl_internal.h"

void glDepthRangef (GLclampf zNear, GLclampf zFar)
{
	float mid;

	zNear = CLAMPF(zNear) * 65535.f;
	zFar = CLAMPF(zFar) * 65535.f;

	mid = (zNear+zFar) / 2;

	/* z Scale and Offset

	   The PSP seems to use the opposite convention from OpenGL
	   about the depth direction; uses positive Z into the screen,
	   with larger depth-buffer values being closer.  This Z scale
	   factor flips Z around to match what the rest of GL expects.
	 */
	sendCommandf(CMD_VIEWPORT_SZ, mid - zFar);
	sendCommandf(CMD_VIEWPORT_TZ, mid + pspgl_curctx->viewport.depth_offset);

	if (zNear > zFar) {
		GLfloat temp = zNear;
		zNear = zFar;
		zFar = temp;
	}

	/* Far and Near Clip Planes.  Clipping happens before the
	   viewport transform, and are therefore in the OpenGL
	   coordinate system. */
	sendCommandi(CMD_CLIP_NEAR, (int) zNear);
	sendCommandi(CMD_CLIP_FAR, (int) zFar);
}
