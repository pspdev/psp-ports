#include <stdlib.h>
#include "pspgl_internal.h"

/* Framebuffer pixel formats.  These are in order of most desirable to
   least, appropximately.  This means: 
    - smaller formats are better
    - prefer alpha over stencil
*/
const struct pspgl_pixconfig __pspgl_pixconfigs[] = {
/*        R  G  B   A  S   hwformat          */	
	{ 5, 6, 5,  0, 0,  GE_RGB_565   },
	{ 5, 5, 5,  1, 0,  GE_RGBA_5551 },
	{ 5, 5, 5,  0, 1,  GE_RGBA_5551 },
	{ 4, 4, 4,  4, 0,  GE_RGBA_4444 },
	{ 4, 4, 4,  0, 4,  GE_RGBA_4444 },
	{ 8, 8, 8,  8, 0,  GE_RGBA_8888 },
	{ 8, 8, 8,  0, 8,  GE_RGBA_8888 },

	{ 0, 0, 0,  0, 0,  -1 }
};


EGLBoolean eglChooseConfig (EGLDisplay dpy, const EGLint *attrib_list,
			    EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
	int alpha_size = 0, blue_size = 0, green_size = 0, red_size = 0;
	int depth_size = 0, stencil_size = 0;
	int nconfig;
	int i;

	while (attrib_list && attrib_list[0] != EGL_NONE) {
		switch (*attrib_list) {
		case EGL_ALPHA_SIZE:	alpha_size = attrib_list[1]; break;
		case EGL_BLUE_SIZE:	blue_size = attrib_list[1]; break;
		case EGL_GREEN_SIZE:	green_size = attrib_list[1]; break;
		case EGL_RED_SIZE:	red_size = attrib_list[1]; break;
		case EGL_DEPTH_SIZE:	depth_size = attrib_list[1]; break;
		case EGL_STENCIL_SIZE:	stencil_size = attrib_list[1]; break;

		case EGL_BUFFER_SIZE:
			break;

		case EGL_CONFIG_CAVEAT:
		case EGL_CONFIG_ID:
		case EGL_LEVEL:
		case EGL_MAX_PBUFFER_HEIGHT:
		case EGL_MAX_PBUFFER_PIXELS:
		case EGL_MAX_PBUFFER_WIDTH:
		case EGL_NATIVE_RENDERABLE:
		case EGL_NATIVE_VISUAL_ID:
		case EGL_NATIVE_VISUAL_TYPE:
		case EGL_SAMPLES:
		case EGL_SAMPLE_BUFFERS:
		case EGL_SURFACE_TYPE:
		case EGL_TRANSPARENT_TYPE:
		case EGL_TRANSPARENT_BLUE_VALUE:
		case EGL_TRANSPARENT_GREEN_VALUE:
		case EGL_TRANSPARENT_RED_VALUE:
		case EGL_BIND_TO_TEXTURE_RGB:
		case EGL_BIND_TO_TEXTURE_RGBA:
		case EGL_MIN_SWAP_INTERVAL:
		case EGL_MAX_SWAP_INTERVAL:
			break;

		default:
			EGLERROR(EGL_BAD_ATTRIBUTE);
			return EGL_FALSE;
		}

		attrib_list += 2;
	}

	psp_log("looking for configs for red=%d green=%d blue=%d alpha=%d stencil=%d depth=%d\n",
		red_size, green_size, blue_size, alpha_size, stencil_size, depth_size);

	nconfig = 0;
	for(i = 0; __pspgl_pixconfigs[i].hwformat != -1; i++) {
		const struct pspgl_pixconfig *p = &__pspgl_pixconfigs[i];

		psp_log("  config %d has red=%d green=%d blue=%d alpha=%d stencil=%d hwformat=%d\n",
			i, p->red_bits, p->green_bits, p->blue_bits, p->alpha_bits, 
			p->stencil_bits, p->hwformat);

		if (depth_size > 16)
			continue;
		if (red_size > p->red_bits)
			continue;
		if (green_size > p->green_bits)
			continue;
		if (blue_size > p->blue_bits)
			continue;
		if (alpha_size > p->alpha_bits)
			continue;
		if (stencil_size > p->stencil_bits)
			continue;

		if (configs != NULL) {
			if (nconfig >= config_size)
				break;
			configs[nconfig] = EGLCONFIGIDX(i, depth_size);
		}

		psp_log("found config %d 0x%x\n", nconfig, EGLCONFIGIDX(i, depth_size));

		nconfig++;
	}
	*num_config = nconfig;

	return EGL_TRUE;
}

