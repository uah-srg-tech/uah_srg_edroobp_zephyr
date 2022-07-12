
#include <zephyr.h>
#include "basic_types.h"

int32_t get_random_seed()
{

	return (int32_t)k_uptime_get_32();

}
