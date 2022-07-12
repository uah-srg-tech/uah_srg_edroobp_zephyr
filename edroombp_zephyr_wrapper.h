/**
 * \file	edroombp_4_8_impr_wrapper.h
 * \brief
 *
 * \author	Alberto Carrasco Gallardo, <acarrasco@srg.aut.uah.es>
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>
 * \author	Álvaro González García, <alvaro.gonzalezgarci@uah.es>
 *
 * \internal
 * Created: 	07-may-2014
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcalá.
 * Copyright: Copryright (c) 2014, Alberto Carrasco Gallardo
 * Copyright: Copyright (c) 2014, Santiago Carmona Meco
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */

#ifndef EDROOMBP_4_8_IMPR_WRAPPER_H_
#define EDROOMBP_4_8_IMPR_WRAPPER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "edroomtypes.h"
#include <stdlib.h>

/**
 *  @addtogroup ICUSW_SERVICE_MODULES ICUSw Service Modules
 *  @{
 */
/**
 *  @addtogroup EDROOMSL EDROOM SERVICE LIBRARY
 *  @{
 */
/**
 *  @defgroup EDROOMBP_WRAPPER EDROOMBP C WRAPPER RTEMS
 *  @{
 */
/*				PR_KERNEL FUNCTIONALITY		*/
/**
 * \brief	This function initializes the system. Basically it stablishes the
 * 			Time of Day for RTEMS and capture the initial time value in the
 * 			memory pointed by the parameter start_time.
 * @param[out] start_time	Memory to store the initial time value in seconds and
 * 			microseconds.
 */
int edroombp_system_init(int level);


/*				PR_TASK FUNCTIONALITY		*/
/**
 * 	\brief	Create a task with a given initial priority and stack size.
 * @param[in] initial_priority	the initial priority for the task.
 * @param[in] stack_size	The size of the minimum stack needed by the task.
 * @param[out] id	identifier to return the task id.
 * @return	status of the call.
 */
k_tid_t edroombp_task_create(struct k_thread *new_task,k_thread_stack_t *stack,k_thread_entry_t entry, int prio, uint32_t options,k_timeout_t delay, void * pPr_Task);

/**
 * \brief	Put a task in the ready state.
 * @param[in] id	the identifier of the task.
 * @param[in] entry_point	the entry point of the task's code.
 * @param[in] argument	the argument(if any) for the task code.
 */
void edroombp_task_start(struct k_thread *task);

/**
 * \brief	delete a task
 * @param[in] id	the identifier of the task to delete.
 */
void edroombp_task_delete(struct k_thread *task);

/**
 * \brief	Get the current priority of a task.
 * @param[in] id	the task identifier.
 * @param[out] old_priority	the old priority shall be returned.
 */
int edroombp_task_get_current_priority(struct k_thread *task);

/**
 * \brief	change the priority of a task.
 * @param[in] priority	the new priority.
 * @param[in] task_id	the task identifier.
 */
void edroombp_task_change_priority(struct k_thread *task, int newPrio);


/*			PR_TIME	FUNCTIONALITY				*/
/**
 * \brief	Get the current time.
 * @param[out] time_val	memory to store the value in seconds and microseconds.
 */
int64_t edroombp_get_time(void);

/**
 * \brief	Wait for a number of clock ticks.
 * @param[in] ticks the number of ticks to wait.
 */
void edroombp_wait_ticks(uint32_t ticks);


/*				SEMAPHORE FUNCTIONALITY			*/
/**
 * \brief	Create a semaphore.
 * @param[in] count	the count, in case is a counting semaphore.
 * @param[in] priority_ceiling	the priority ceiling of the semaphore.
 * @param[out] semId	memory for returning the semaphore identifier.
 */
int edroombp_semaphore_create(struct k_sem *sem, unsigned int initial_count, unsigned int limit);

/**
 * \brief	release a semaphore.
 * @param[in] semId	the semaphore identifier.
 */
void edroombp_semaphore_release(struct k_sem *sem);

/**
 * \brief	Catch a semaphore.
 * @param[in] semId	the semaphore identifier.
 */
int edroombp_semaphore_catch(struct k_sem *sem, k_timeout_t timeout);

/**
 * \brief	try to catch a semaphore withOUT waiting in case it is already
 * 			catch.
 * @param[in] semId the semaphore identifier.
 * @return	1 if the semaphore was successfully catch, 0 otherwise.
 */
int edroombp_semaphore_try_catch(struct k_sem *sem);

/**
 * @brief return de sem count
 * 
 * @param sem 
 * @return int 
 */
int edroombp_semphore_count(struct k_sem *sem);



/**
 * \brief	Create a mutex.
 * @param[out] mutex_id	memory to return the mutex identifier.
 * @param[in] is_prio_ceiling	boolean to create the mutex with or without
 * 			priority ceiling.
 * @param[out] prio_type the type of priority with which the mutex is created.
 * @param[in] prio_ceiling the priority ceiling of the mutex.
 */
int edroombp_mutex_create(struct k_mutex *mutex);

/**
 * \brief	Catch a semaphore.
 * @param[in] semId	the semaphore identifier.
 */
int edroombp_mutex_lock(struct k_mutex *mutex, k_timeout_t timeout);

/**
 * \brief	try to catch a semaphore withOUT waiting in case it is already
 * 			catch.
 * @param[in] semId the semaphore identifier.
 * @return	1 if the semaphore was successfully catch, 0 otherwise.
 */
int32_t edroombp_mutex_try_catch(struct k_mutex *mutex);

/**
 * \brief	release a semaphore.
 * @param[in] semId	the semaphore identifier.
 */
int edroombp_mutex_unlock(struct k_mutex *mutex);

/**
 * @brief return the mutex count
 * 
 * @param mutex 
 * @return int 
 */
int edroombp_mutex_count(struct k_mutex *mutex);

/**
 * @brief return the mutex priority
 * 
 * @param mutex 
 * @return int 
 */
int edroombp_mutex_priority(struct k_mutex *mutex);



/*				PR_IRQMANAGER FUNCTIONALITY			*/
/**
 * \brief	disable all irqs.
 */
void edroombp_disable_irqs(int irq);

/**
 * \brief	enable all irqs.
 */
void edroombp_enable_irqs(int irq);

/**
 * \brief	install a handler for an irq.
 * @param[in] handler	the pointer to the handler.
 * @param[in] vector_num	the irq number.
 */
void edroombp_install_handler(void * handler, uint8_t vector_num);

/**
 * \brief	deinstall a handler for a given IRQ.
 * @param[in] vector_num	the irq number.
 */
void edroombp_deinstall_handler(uint8_t vector_num);

/**
 * \brief	mask a single external irq.
 * @param[in] irq_num	the irq level.
 */
void edroombp_mask_irq(uint8_t irq_level);

/**
 * \brief	unmask a single external irq.
 * @param[in] irq_num	the irq level.
 */
void edroombp_unmask_irq(uint8_t irq_level);


/*				IRQ_EVENT FUNCTIONALITY				*/
/**
 * \brief	release an event
 * @param[in] caller_task_id	the identifier of the caller task.
 * @param[in] irq_event	the irq event number.
 */
void edroombp_event_signal(uint32_t caller_task_id, uint32_t irq_event, struct k_event *evento);

/**
 * \brief	catch an event.
 * @param[out] caller_task_id	the identifier of the caller.(probably removable)
 * @param[in] irq_event	the irq event number.
 */
void edroombp_event_catch(uint32_t * caller_task_id, uint32_t irq_event, struct k_event *evento);

/**
 * \brief	try to catch an event during a given number of clock ticks.
 * @param[out] caller_task_id	the identifier of the caller task.
 * @param[in] irq_event	the irq event number.
 * @param[in] ticks	the maximum number of ticks to wait.
 * @return true or false.
 */
bool_t edroombp_event_timed_catch(uint32_t * caller_task_id,
											uint32_t irq_event, uint32_t ticks, struct k_event *evento);

/**
 * \brief	try to catch an event
 * @param caller_task_id	the identifier of the caller task.
 * @param irq_event	irq_event	the irq event number.
 * @return true or false.
 */
bool_t edroombp_event_try_catch(uint32_t * caller_task_id, uint32_t irq_event, struct k_event *evento);


#ifdef __cplusplus
}
#endif

#endif /* EDROOMBP_4_8_IMPR_WRAPPER_H_ */