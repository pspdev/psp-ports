#include "pspgl_internal.h"
#include "pspgl_texobj.h"

EGLBoolean eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
	unsigned error;
	struct pspgl_texobj *tobj;
	struct pspgl_teximg *timg;
	struct pspgl_surface *s = (struct pspgl_surface *)surface;
	const struct pspgl_texfmt *texfmt;

	error = EGL_BAD_MATCH;
	if (dpy == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE || !(s->flags & SURF_TEXTURE))
		goto out_error;

	/* find an appropriate texture format */
	for(texfmt = __pspgl_texformats; texfmt->format != 0; texfmt++)
		if (texfmt->hwformat == s->pixfmt &&						// match hardware format...
		    ((s->flags & SURF_USEALPHA) == 0) == ((texfmt->flags & TF_ALPHA) == 0))	// ...and use of alpha
			break;

	if (texfmt->format == 0)
		goto out_error;	/* could happen if we want alpha with non-alpha texfmt */

	error = EGL_BAD_PARAMETER;
	if (buffer != EGL_BACK_BUFFER)
		goto out_error;

	if (!pspgl_curctx->texture.bound)
		glBindTexture(GL_TEXTURE_2D, 0); /* bind default texture */

	tobj = pspgl_curctx->texture.bound;

	/* free all teximgs */
	for(int i = 0; i < MIPMAP_LEVELS; i++)
		if (tobj->images[i] != NULL) {
			__pspgl_teximg_free(tobj->images[i]);
			tobj->images[i] = NULL;
		}

	tobj->flags &= ~TOF_SWIZZLED;
	tobj->flags |= TOF_FLIPPED;

	timg = __pspgl_teximg_from_buffer(s->color_back, 0,
					  s->width, s->height, s->pixelperline, texfmt);

	__pspgl_set_texture_image(tobj, 0, timg);
	__pspgl_update_texenv(tobj);
	if (tobj->flags & TOF_GENERATE_MIPMAPS)
		__pspgl_update_mipmaps();

	return EGL_TRUE;

  out_error:
	EGLERROR(error);
	return EGL_FALSE;	/* ? */
}

EGLBoolean eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
	/* No operation.  In this implementation, we still allow a
	   surface to be used while it is bound to a texture.  We also
	   allow it to be bound to multiple textures.  This doesn't
	   keep track of which texture(s) the surface has been bound
	   to, so we don't try to track them down and do anything
	   about it. */

	return EGL_TRUE;
}
