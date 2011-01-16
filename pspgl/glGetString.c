#include <stdlib.h>
#include "pspgl_internal.h"


static const
char *gl_strings [] = {
	/* GL_VENDOR */
	"pspGL",
	/* GL_RENDERER */
	"OpenGL ES-CM 1.1",
	/* GL_VERSION */
	"1.3 (pspGL build " __DATE__ ", " __TIME__ ")",
	/* GL_EXTENSIONS */
	"GL_EXT_texture_env_add "
	"GL_ARB_texture_env_add "
	"GL_EXT_blend_minmax "
	"GL_EXT_blend_subtract "
 	"GL_EXT_paletted_texture "
	"GL_ARB_texture_compression "
	"GL_EXT_texture_compression_s3tc "
	"GL_EXT_vertex_array " 
	"GL_EXT_compiled_vertex_array "
	"GL_PSP_statistics "
	"GL_ARB_vertex_buffer_object "
	"GL_PSP_bezier_patch "
	"GL_PSP_vertex_blend "
	"GL_PSP_view_matrix "
	"GL_PSP_sprites "
	"GL_MESA_pack_invert "
	"GL_ARB_pixel_buffer_object "
	"GL_EXT_pixel_buffer_object "
};


const GLubyte * glGetString (GLenum name)
{
	unsigned long idx = name - GL_VENDOR;
	const char *s;

	s = NULL;
	if (idx >= sizeof(gl_strings)/sizeof(gl_strings[0]))
		goto out_error;

	s = gl_strings[idx];

	return (const GLubyte *) s;

  out_error:
	GLERROR(GL_INVALID_ENUM);
	return NULL;
}

