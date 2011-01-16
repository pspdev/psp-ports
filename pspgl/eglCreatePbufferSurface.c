#include <stdlib.h>
#include <string.h>

#include "pspgl_internal.h"
#include "pspgl_buffers.h"

#define MASK(bits)	((1 << (bits)) - 1)

EGLSurface eglCreatePbufferSurface (EGLDisplay dpy, EGLConfig config,
				    const EGLint *attrib_list)
{
	unsigned width, height;
	unsigned egl_texfmt, egl_textarget;
	unsigned error;
	struct pspgl_surface *s;
	const struct pspgl_pixconfig *pixconf;
	int has_depthbuffer = EGLCFG_HASDEPTH(config);
	int bytesperpixel;
	unsigned bufferlen;

	pixconf = &__pspgl_pixconfigs[EGLCFG_PIXIDX(config)];

	s = NULL;
	width = height = 0;
	egl_texfmt = EGL_NO_TEXTURE;
	egl_textarget = EGL_NO_TEXTURE;

	for(; attrib_list && attrib_list[0] != EGL_NONE; attrib_list += 2) {
		switch(attrib_list[0]) {
		case EGL_WIDTH:
			width = attrib_list[1];
			break;
		case EGL_HEIGHT:
			height = attrib_list[1];
			break;
		case EGL_TEXTURE_TARGET:
			egl_textarget = attrib_list[1];
			break;
		case EGL_TEXTURE_FORMAT:
			egl_texfmt = attrib_list[1];
			break;
		}
	}

	error = EGL_BAD_MATCH;

	/* Can't have stencil and alpha */
	if (pixconf->stencil_bits && (EGL_TEXTURE_FORMAT == EGL_TEXTURE_RGBA))
		goto out_error;

	if ((egl_textarget == EGL_NO_TEXTURE) != (egl_textarget == EGL_NO_TEXTURE))
		goto out_error;

	if (pixconf->hwformat == GE_RGBA_8888)	
		bytesperpixel = 4;
	else
		bytesperpixel = 2;

	bufferlen = pow2(width) * height * bytesperpixel;

	if (egl_textarget != EGL_NO_TEXTURE) {
		if (!ispow2(width) || !ispow2(height))
			goto out_error;

	}

	error = EGL_BAD_ALLOC;

	s = malloc(sizeof(*s));

	if (s == NULL)
		goto out_error;

	memset(s, 0, sizeof(*s));
	s->refcount = 1;
	s->width = width;
	s->height = height;
	s->pixelperline = pow2(width);
	s->flags = 0;
	s->pixfmt = pixconf->hwformat;

	s->read = &s->color_back;
	s->draw = &s->color_back;

	if (egl_textarget != EGL_NO_TEXTURE)
		s->flags |= SURF_TEXTURE;
	if (egl_texfmt == EGL_TEXTURE_RGBA)
		s->flags |= SURF_USEALPHA;

	s->alpha_mask	= MASK(pixconf->alpha_bits);
	s->stencil_mask	= MASK(pixconf->stencil_bits);

	if (!(s->color_back = __pspgl_buffer_new(bufferlen, GL_STATIC_COPY_ARB)))
		goto out_error;
	s->color_front = s->color_back;
	s->color_front->refcount++;

	if (has_depthbuffer) {
		bufferlen = s->height * s->pixelperline * 2;
		if ((s->depth_buffer = __pspgl_buffer_new(bufferlen, GL_STATIC_COPY_ARB)) == NULL)
			goto out_error;
	}

	return (EGLSurface)s;

  out_error:
	if (s)
		eglDestroySurface(dpy, s);
	EGLERROR(error);
	return EGL_NO_SURFACE;
}
