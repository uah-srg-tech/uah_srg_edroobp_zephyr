#include <zephyr.h>
#include "kernel.h"
#include "irq.h"
#include <sys_clock.h>


#include "edroombp_zephyr_wrapper.h"


k_tid_t edroombp_task_create(struct k_thread *new_task,k_thread_stack_t *stack,
k_thread_entry_t entry, int prio, uint32_t options,k_timeout_t delay, void * pPr_Task)
{
    return k_thread_create(new_task, stack, K_THREAD_STACK_SIZEOF(stack),
                    entry, pPr_Task, NULL, NULL, prio, options, delay);
}


void edroombp_task_start(struct k_thread *task)
{
    k_thread_start(task);
}

void edroombp_task_delete(struct k_thread *task)
{
    k_thread_abort(task);
}

int edroombp_task_get_current_priority(struct k_thread *task)
{
    return k_thread_priority_get(task);
}

void edroombp_task_change_priority(struct k_thread *task, int newPrio)
{
    k_thread_priority_set(task, newPrio);
}

int64_t edroombp_get_time(void)
{
	return k_uptime_ticks();
}

void edroombp_wait_ticks(uint32_t ticks)
{
	k_timeout_t ticksTimeOut =  K_TICKS(ticks);
	k_sleep(ticksTimeOut);
}

int edroombp_semaphore_create(struct k_sem *sem, unsigned int initial_count, unsigned int limit)
{
	int semaforo = k_sem_init(sem, initial_count, limit);
	return semaforo;
}

void edroombp_semaphore_release(struct k_sem *sem)
{
    k_sem_give(sem);
}

int edroombp_semaphore_catch(struct k_sem *sem, k_timeout_t timeout)
{
    return k_sem_take(sem, timeout);
}

int edroombp_semphore_count(struct k_sem *sem)
{
    return k_sem_count_get(sem);
}


int edroombp_semaphore_try_catch(struct k_sem *sem)
{
	int32_t solucion = (int32_t)k_sem_take(sem, K_NO_WAIT);
	return solucion==0;
}

int edroombp_mutex_create(struct k_mutex *mutex)
{
    return k_mutex_init(mutex);
}

int edroombp_mutex_lock(struct k_mutex *mutex, k_timeout_t timeout)
{
	int unlockMutex = k_mutex_lock(mutex, timeout);
    return unlockMutex;
}

int32_t edroombp_mutex_try_catch(struct k_mutex *mutex)
{
   int32_t not_locked;
  
   not_locked = (int32_t)k_mutex_lock(mutex, K_NO_WAIT);
      
   return not_locked==0;
}


int edroombp_mutex_unlock(struct k_mutex *mutex)
{
	int unlockMutex = k_mutex_unlock(mutex);
    return unlockMutex;
}

int edroombp_mutex_count(struct k_mutex *mutex)
{
    return mutex->lock_count;
}

int edroombp_mutex_priority(struct k_mutex *mutex)
{
	return mutex->owner_orig_prio;
}


/*				PR_IRQMANAGER FUNCTIONALITY			*/
#define ZEPHYR_INTERRUPT	1
#define RAW_INTERRUPT	0

void edroombp_disable_irqs(int irq)
{
	irq_enable(irq); 
}

void edroombp_enable_irqs(int irq)
{
	irq_disable(irq);
}

void edroombp_install_handler(void * handler, uint8_t vector_num)
{
	ARCH_IRQ_CONNECT(1, 1, vector_num, handler, 1);
	// IRQ_CONNECT(vector_num,1,vector_num,handler,1);
}

void edroombp_deinstall_handler(uint8_t vector_num)
{
	//nothing to be done.
}

void edroombp_mask_irq(uint8_t irq_level)
{
	//disable a given irq.
	//LEON_Mask_interrupt(irq_num - 0x10);
}

void edroombp_unmask_irq(uint8_t irq_level)
{
	//enable a given irq
	//LEON_Unmask_interrupt(irq_num - 0x10);
}

/*				IRQ_EVENT FUNCTIONALITY				*/


void edroombp_event_signal(uint32_t caller_task_id, uint32_t irq_event, struct k_event *evento)
{
	k_event_set(evento, irq_event);
}

void edroombp_event_catch(uint32_t * caller_task_id, uint32_t irq_event, struct k_event *evento)
{
	k_event_post(evento, irq_event);
}

bool_t edroombp_event_timed_catch(uint32_t * caller_task_id, uint32_t irq_event,
		uint32_t ticks, struct k_event *evento)
{
	k_timeout_t tiempo;
	tiempo.ticks = ticks;
	return k_event_wait(evento, irq_event, true, tiempo) > 0; 	
}

bool_t edroombp_event_try_catch(uint32_t * caller_task_id, 
uint32_t irq_event, struct k_event *evento)
{
	return evento->events == irq_event; 	
}

