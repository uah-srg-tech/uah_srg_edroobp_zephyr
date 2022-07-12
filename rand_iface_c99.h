#ifndef PUBLIC__RAND_IFACE_C99_H
#define PUBLIC__RAND_IFACE_C99_H


#include "config.h"
#include "basic_types.h"

/*PROTECTED REGION ID(public_rand_iface_h) ENABLED START*/
/**
 *  @addtogroup ICUSW_SERVICE_MODULES ICUSw Service Modules
 *  @{
 */
/**
 *  @addtogroup LIBC LIBC
 *  @{
 */
/**
 *  @defgroup RAND RAND
 *  @{
 */
/**
 * The rand function returns the next pseudo-random number in the
 * series. The value ranges from 0 to RAND_MAX.
 * @return the "random" number.
 */
int rand(void);

int get_seed();
/**
 * This function establishes seed as the seed for a new series of
 * pseudo-random numbers. If you call rand before a seed has been
 * established with srand, it uses the value 1 as a default seed.
 * @param seed seed to start the randomness
 */
void srand(unsigned int seed);

/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */
/*PROTECTED REGION END*/


#endif // PUBLIC__RAND_IFACE_C99_H
