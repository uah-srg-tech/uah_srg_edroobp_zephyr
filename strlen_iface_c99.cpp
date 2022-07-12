#include "basic_types.h"
#include <stdlib.h>	//necesaria para "size_t"
#include <cstddef>
//#include <climits>
#include "strlen_iface_c99.h"

/*
 * The contents of this file have been extracted from the implementation
 * of the strlen() function of the Newlib.
 */

#define LBLOCKSIZE   (sizeof (long))
#define UNALIGNED(X) ((long)X & (LBLOCKSIZE - 1))

#if LONG_MAX == 2147483647L
#define DETECTNULL(X) (((X) - 0x01010101) & ~(X) & 0x80808080)
#else
#if LONG_MAX == 9223372036854775807L
/* Nonzero if X (a long int) contains a NULL byte. */
#define DETECTNULL(X) (((X) - 0x0101010101010101) & ~(X) & 0x8080808080808080)
#else
#error long int is not a 32bit or 64bit type.
#endif
#endif

#ifndef DETECTNULL
#error long int is not a 32bit or 64bit byte
#endif

size_t strlen(const char *str)
{
#if defined(PREFER_SIZE_OVER_SPEED) || defined(__OPTIMIZE_SIZE__)
  const char *start = str;

  while (*str)
    str++;

  return str - start;
#else
  const char *start = str;
  unsigned long *aligned_addr;

  if (!UNALIGNED (str))
    {
      /* If the string is word-aligned, we can check for the presence of
         a null in each word-sized block.  */
      aligned_addr = (unsigned long*)str;
      while (!DETECTNULL (*aligned_addr))
        aligned_addr++;

      /* Once a null is detected, we check each byte in that block for a
         precise position of the null.  */
      str = (char*)aligned_addr;
    }

  while (*str)
    str++;
  return str - start;
#endif /* not PREFER_SIZE_OVER_SPEED */
}
