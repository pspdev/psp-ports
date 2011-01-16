#include <zzip/fetch.h>

#if defined ZZIP_WORDS_BIGENDIAN &&  defined bswap_16 && defined bswap_32
# define __ZZIP_GET16(__p)                        bswap_16(*(uint16_t*)(__p))
# define __ZZIP_GET32(__p)                        bswap_32(*(uint32_t*)(__p))
# define __ZZIP_SET16(__p,__x) (*(uint16_t*)(__p) = bswap_16((uint16_t)(__x)))
# define __ZZIP_SET32(__p,__x) (*(uint32_t*)(__p) = bswap_32((uint32_t)(__x)))
#endif

/* ------------------------- fetch helpers --------------------------------- */

/**
 * Make 32 bit value in host byteorder from little-endian mapped octet-data
 * (works also on machines which SIGBUS on misaligned data access (eg. 68000))
 */
uint32_t __zzip_get32(unsigned char * s)
{
#if defined __ZZIP_GET32
    return __ZZIP_GET32(s);
#else
    return ((uint32_t)s[3] << 24) | ((uint32_t)s[2] << 16)
	|  ((uint32_t)s[1] << 8)  |  (uint32_t)s[0];
#endif
}

/** => __zzip_get32
 * This function does the same for a 16 bit value.
 */
uint16_t __zzip_get16(unsigned char * s)
{
#if defined __ZZIP_GET16
    return __ZZIP_GET16(s);
#else
    return ((uint16_t)s[1] << 8) | (uint16_t)s[0];
#endif
}

/** => __zzip_get32
 * This function pushes a 32bit value at the specified address
 */
void __zzip_set32(unsigned char * s, uint32_t v)
{
#if defined __ZZIP_SET32
    return __ZZIP_SET32(s, v);
#else
    s[0] = (unsigned char) (v);
    v >>= 8;
    s[1] = (unsigned char) (v);
    v >>= 8;
    s[2] = (unsigned char) (v);
    v >>= 8;
    s[3] = (unsigned char) (v);
#endif
}

/** => __zzip_get32
 * This function does the same for a 16 bit value.
 */
void __zzip_set16(unsigned char * s, uint16_t v)
{
#if defined __ZZIP_SET16
    return __ZZIP_SET16(s, v);
#else
    s[0] = (unsigned char) (v);
    v >>= 8;
    s[1] = (unsigned char) (v);
#endif
}
