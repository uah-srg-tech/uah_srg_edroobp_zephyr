#ifndef PUBLIC__STRLEN_IFACE_C99_H
#define PUBLIC__STRLEN_IFACE_C99_H


#include "basic_types.h"
#include "config.h"

/*PROTECTED REGION ID(public_strlen_iface_h) ENABLED START*/
/**
 *  @addtogroup ICUSW_SERVICE_MODULES ICUSw Service Modules
 *  @{
 */
/**
 *  @addtogroup LIBC LIBC
 *  @{
 */
/**
 *  @defgroup STRLEN STRLEN
 *  @{
 */
/**
 * The strlen function returns the length of the null-terminated
 * string s in bytes
 * @param s the string
 * @return the size of the string.
 */
size_t strlen(const char *s);

/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 * @param s
 * @return
 */
/*PROTECTED REGION END*/



#endif // PUBLIC__STRLEN_IFACE_C99_H
