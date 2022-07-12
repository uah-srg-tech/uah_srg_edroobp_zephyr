#ifndef __PUBLIC__CONFIG_H__
#define __PUBLIC__CONFIG_H__


//***************************************************************************
//TIMER
//USECS_PER_SEC
#define USECS_PER_SEC 1000000

//USECS_PER_TICK
//ToDo revisa TICK
#define CONFIG_PLATFORM_ZEPHYR_USECS_PER_TICK 10000
#define CONFIG_PLATFORM_RISCV_RV32_TIMER_COUNT 100000

#define CLICKS_PER_SEC	(USECS_PER_SEC/CONFIG_PLATFORM_ZEPHYR_USECS_PER_TICK)
#define USEC_PER_TICK	(CONFIG_PLATFORM_ZEPHYR_USECS_PER_TICK)

#define MAX_SEM_LIMIT 0xffffffff

#ifdef CONFIG_EDROOMBP_TEST_VERBOSE

#include <public/ut_edroombp_shared_functions_iface_v1.h>

#define DEBUG(fmt, args...)	iprintf ( \
		__FILE__ ":%d: %s: " fmt "\n",\
		__LINE__, __func__, ##args)




#else

//------------------------------------------------------------------------------
#define DEBUG(fmt, args...)
#define ASSERT(expr)
//------------------------------------------------------------------------------

#endif /* CONFIG_SWP_EDROOMBP_SLIB_V3_0_TESTING */


//UT_EDROOMBP_PR_SEMAPHORE_SLIB__CONFIG

#define CONFIG_SWP_UT_EDROOMBP_PR_SEMAPHORE_SLIB_HEAD_OUTPUT_CHANNEL_ID 0
#define CONFIG_SWI_XPRINTF_IFACE_V1_OUTPUT_CHANNEL_UT_EDROOMBP_PR_SEMAPHORE_SLIB_HEAD_OUTPUT_CHANNEL_ID 0

#define CONFIG_SWP_UT_EDROOMBP_PR_SEMAPHORE_SLIB_OUTPUT_CHANNEL_ID CONFIG_SWP_UT_EDROOMBP_PR_SEMAPHORE_SLIB_HEAD_OUTPUT_CHANNEL_ID
#define CONFIG_SWI_XPRINTF_IFACE_V1_OUTPUT_CHANNEL_UT_EDROOMBP_PR_SEMAPHORE_SLIB_OUTPUT_CHANNEL_ID CONFIG_SWI_XPRINTF_IFACE_V1_OUTPUT_CHANNEL_UT_EDROOMBP_PR_SEMAPHORE_SLIB_HEAD_OUTPUT_CHANNEL_ID


#endif // __PUBLIC__CONFIG_H__
