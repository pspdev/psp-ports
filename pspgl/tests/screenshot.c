#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <png.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

static void writepng(FILE *fp, const unsigned char *image, int width, int height)
{
	const unsigned char *rows[height];
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	int row;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
		return;
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		goto out_free_write_struct;

	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, width, height,
		     8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
		     PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	for(row = 0; row < height; row++)
		rows[row] = &image[row * width * 4];

	png_set_rows(png_ptr, info_ptr, (png_byte**)rows);
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
	png_write_end(png_ptr, info_ptr);

  out_free_write_struct:
	png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
}

void screenshot(const char *basename)
{
	if (basename == NULL)
		basename = "screenshot";
	char name[6 + strlen(basename) + 4 + 4 + 1];
	int count = 0;
	FILE *fp = NULL;
	unsigned char *image;

	do {
		if (fp)
			fclose(fp);
#if SYS
		sprintf(name, "%s%03d.png", basename, count++);
#else
		sprintf(name, "ms0:/%s%03d.png", basename, count++);
#endif
		fp = fopen(name, "rb");
	} while(fp != NULL && count < 1000);

	if (count == 1000) {
		if (fp)
			fclose(fp);
		return;
	}

	fp = fopen(name, "wb");
	if (fp == NULL)
		return;

	glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
	glPixelStorei(GL_PACK_INVERT_MESA, GL_TRUE);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);

	//image = malloc(480*272*4);
	image = memalign(64, 480*272*4);
	glReadPixels(0,0, 480,272, GL_RGBA,GL_UNSIGNED_BYTE, image);

	glPopClientAttrib();

	if (glGetError() != 0)
		goto out;

	writepng(fp, image, 480, 272);

  out:
	fclose(fp);
	free(image);
}
