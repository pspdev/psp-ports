#include "pspgl_internal.h"
#include "pspgl_texobj.h"

void glColorTable(GLenum target, GLenum internalformat, 
		  GLsizei width, GLenum format, GLenum type, const GLvoid *data)
{
	struct pspgl_teximg *cmap;
	struct pspgl_texobj *tobj;
	const struct pspgl_texfmt *fmt;
	GLenum error;

	error = GL_INVALID_ENUM;
	if (unlikely(target != GL_TEXTURE_2D))
		goto out_error;

	if (!(format == GL_RGB || format == GL_RGBA))
		goto out_error;

	fmt = __pspgl_hardware_format(__pspgl_texformats, format, type);
	if (unlikely(fmt == NULL))
		goto out_error;

	error = GL_INVALID_OPERATION;
	if (internalformat != format)
		goto out_error;

	error = GL_INVALID_VALUE;
	if (!ispow2(width))
		goto out_error;

	cmap = __pspgl_teximg_new(data, &pspgl_curctx->unpack, width, 1, 0, GL_FALSE, fmt);
	if (cmap == NULL)
		return;		/* error already set */

	if (!pspgl_curctx->texture.bound)
		glBindTexture(target, 0);

	tobj = pspgl_curctx->texture.bound;

	if (tobj->cmap) {
		/* release old cmap */
		__pspgl_teximg_free(tobj->cmap);
	}
	tobj->cmap = cmap;
	pspgl_curctx->hw.dirty |= HWD_CLUT;

	__pspgl_update_texenv(tobj);

	return;

  out_error:
	GLERROR(error);
	return;
}

void glColorTableEXT(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *data)
	__attribute__((alias("glColorTable")));
