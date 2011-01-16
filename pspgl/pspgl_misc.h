#ifndef __pspgl_misc_h__
#define __pspgl_misc_h__

#include <sys/types.h>
#include <stdint.h>

/* Return a pointer to uncached address space.  The pointer and size
   must both be a multiple CACHELINE_SIZE.  */
#define CACHELINE_SIZE	64
void *__pspgl_uncached(void *p, size_t size);

/* Round up to a particular power of 2.  "a" evaluated multiple
   times. */
#define ROUNDUP(x, a)  (((x)+((a)-1)) & ~((a)-1))

/* log-base-2 function. */
static inline uint32_t lg2(uint32_t x) __attribute__((const,always_inline));
static inline uint32_t lg2(uint32_t x)
{
	if (__builtin_constant_p(x))
		switch(x) {
		case 1:		return 0;
		case 2:		return 1;
		case 4:		return 2;
		case 8:		return 3;
		case 16:	return 4;
		case 32:	return 5;
		case 64:	return 6;
		case 128:	return 7;
		case 256:	return 8;
		case 512:	return 9;
		case 1024:	return 10;
		case 2048:	return 11;
		case 4096:	return 12;
		case 8192:	return 13;
		case 16384:	return 14;
		case 32768:	return 15;
		case 65536:	return 16;
		}

	return 31 - __builtin_clz(x);
}

/* returns true if x is a power of 2 */
static inline int ispow2(uint32_t x) __attribute__((const));
static inline int ispow2(uint32_t x)
{
	return (x & (x - 1)) == 0;
}

/* Return the next power of 2 >= x */
static inline uint32_t pow2(uint32_t x) __attribute__((const,always_inline));
static inline uint32_t pow2(uint32_t x)
{
	if (__builtin_constant_p(x)) {
		if (ispow2(x))
			return x;
		switch(x) {
#define POW2(x)	case ((x) / 2)+1 ... (x)-1: return (x)
			POW2(4);
			POW2(8);
			POW2(16);
			POW2(32);
			POW2(64);
			POW2(128);
			POW2(256);
			POW2(512);
			POW2(1024);
			POW2(2048);
			POW2(4096);
			POW2(8192);
			POW2(16384);
			POW2(32768);
			POW2(65536);
#undef POW2
		}
	}

	return 1 << (32 - __builtin_clz(x-1));
}

/* Branch probabilities */
#define likely(x)	__builtin_expect((x) != 0, 1)
#define unlikely(x)	__builtin_expect((x) != 0, 0)

extern void __pspgl_log (const char *fmt, ...);

enum pspgl_dump_tag {
	PSPGL_GE_DUMP_MATRIX    = 1,
	PSPGL_GE_DUMP_REGISTERS = 2,
	PSPGL_GE_DUMP_DLIST     = 3,
	PSPGL_GE_DUMP_VRAM      = 4,
	PSPGL_GE_DUMP_SURFACES	= 5,
};

struct pspgl_dump_surfaces {
	uint32_t pixfmt;
	uint32_t alpha_mask, stencil_mask;

	struct pspgl_dump_surf {
		uint32_t start, size, stride;
	} front, back, depth;
};

extern void __pspgl_ge_register_dump (void);
extern void __pspgl_ge_matrix_dump (void);
extern void __pspgl_dlist_dump (unsigned long *cmd_buf, unsigned long len);
extern void __pspgl_vram_dump (void);


/**
 *  Enable this to dump display lists, matrix and command registers to memory stick.
 *  Note: This will seriously slow down program exection, so be patient.
 *  The resulting file "ms0:/pspgl.ge" can easily get analyzed with the tools/decode_ge_dump utility.
 */
#if 0
#define pspgl_ge_register_dump() __pspgl_ge_register_dump()
#define pspgl_ge_matrix_dump() __pspgl_ge_matrix_dump()
#define pspgl_dlist_dump(buf,len) __pspgl_dlist_dump(buf,len)
#define pspgl_vram_dump() __pspgl_vram_dump()
#else
#define pspgl_ge_register_dump() do {} while (0)
#define pspgl_ge_matrix_dump() do {} while (0)
#define pspgl_dlist_dump(buf,len) do {} while (0)
#define pspgl_vram_dump() do {} while (0)
#endif

#if 0
#define psp_log(x...) do { __pspgl_log("%s (%d): ", __FUNCTION__, __LINE__); __pspgl_log(x); } while (0)
#else
#define psp_log(x...) do { } while (0)
#endif

extern void __pspgl_assert_fail(const char *expr, const void *retaddr, 
				const char *func, const char *file, int line);

#if 1
#define assert(x)							\
	do {								\
		if (unlikely(!(x)))					\
			__pspgl_assert_fail(#x, __builtin_return_address(0), \
				__FUNCTION__, __FILE__, __LINE__);	\
	} while(0)
#else
#define assert(x)
#endif


#if 0

#undef malloc
#undef calloc
#undef realloc
#undef free 

#define malloc(size)        __pspgl_calloc(1, size, __FUNCTION__, __LINE__)
#define calloc(count,size)  __pspgl_calloc(count, size, __FUNCTION__, __LINE__)
#define realloc(ptr,size)   __pspgl_realloc(ptr, size, __FUNCTION__, __LINE__)
#define free(ptr)           __pspgl_free(ptr, __FUNCTION__, __LINE__)

#endif

extern void* __pspgl_calloc (size_t count, size_t size, const char *function, unsigned int line);
extern void* __pspgl_realloc (void *ptr, size_t size, const char *function, unsigned int line);
extern void __pspgl_free (void *ptr, const char *function, unsigned int line);


#endif

