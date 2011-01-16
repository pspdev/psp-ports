#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <endian.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "../guconsts.h"
#include "../pspgl_misc.h"

static int vram_id = -1;

static inline
uint32_t swap32 (uint32_t x)
{
	return  ((x >> 24) & 0x000000ff) |
		((x >> 8)  & 0x0000ff00) |
		((x << 8)  & 0x00ff0000) |
		((x << 24) & 0xff000000);
}

static inline
uint16_t swap16 (uint16_t x)
{
	return  ((x >> 8) & 0x00ff) |
		((x << 8) & 0xff00);
}


#if __BYTE_ORDER == __BIG_ENDIAN
#define be32_to_cpu(x) (x)
#define le32_to_cpu(x) swap32(x)
#define be16_to_cpu(x) (x)
#define le16_to_cpu(x) swap16(x)
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#define le32_to_cpu(x) (x)
#define be32_to_cpu(x) swap32(x)
#define le16_to_cpu(x) (x)
#define be16_to_cpu(x) swap16(x)
#else
#error unknown endianess!!
#endif

static void showsurf(const char *surf, struct pspgl_dump_surf *s)
{
	unsigned start = le32_to_cpu(s->start);
	unsigned size = le32_to_cpu(s->size);
	unsigned stride = le32_to_cpu(s->stride);

	fprintf(stderr, "  %s offset %d size %d stride %d\n",
		surf, start, size, stride);
}

static
void process_chunk (uint32_t tag, uint32_t *buf, unsigned long len)
{
	static int vram_dump_id = 0;

	fprintf(stderr, "\ngot chunk (tag 0x%08x, len %lu bytes)\n", tag, len);

	switch (tag) {
		struct pspgl_dump_surfaces *s;

	case PSPGL_GE_DUMP_SURFACES:
		s = (struct pspgl_dump_surfaces *)(buf + 2);

		fprintf(stderr, "  pixfmt=%d\n", le32_to_cpu(s->pixfmt));
		fprintf(stderr, "  alpha mask=%x, stencil mask=%x\n\n",
			le32_to_cpu(s->alpha_mask), le32_to_cpu(s->stencil_mask));
		showsurf("front", &s->front);
		showsurf("back", &s->back);
		showsurf("depth", &s->depth);
		break;

	case PSPGL_GE_DUMP_VRAM:
		if (vram_dump_id++ == vram_id) {
			uint32_t vram_start = buf[0];
			uint32_t vram_size = buf[1];

			write(1, buf+2, vram_size);
		}
		break;
	}
}


int main (int argc, char **argv)
{
	unsigned long flen, fpos;
	uint32_t *fptr;
	int fd;

	if (argc != 3) {
		fprintf(stderr, "\n\tusage: %s <dump.ge> vram-id\n\n", argv[0]);
		return 1;
	}

	if (isatty(1)) {
		fprintf(stderr, "Direct output to a file/pipe\n");
		return 1;
	}

	vram_id = atoi(argv[2]);

	fd = open(argv[1], O_RDONLY, 0);
	flen = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	fptr = (uint32_t*) mmap(NULL, flen, PROT_READ, MAP_PRIVATE, fd, 0);

	fpos = 0;
	flen /= 4;

	while (fpos<flen-2) {
		uint32_t tag = le32_to_cpu(fptr[fpos]);
		uint32_t len = le32_to_cpu(fptr[fpos+1]);

		if (fpos+len/4 > flen) {
			fprintf(stderr, "**** unexpected end of file! ****\n");
			break;
		}

		if (len == 0)
			break;

		process_chunk(tag, &fptr[fpos+2], len-2*4);
		fpos += len/4;
	}

	munmap((void*) fptr, flen);
	close(fd);

	return 0;
}
