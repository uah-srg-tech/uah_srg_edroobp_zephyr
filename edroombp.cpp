#include <zephyr.h>

#include <init.h>

#include "edroombp_zephyr_wrapper.h"
#include "edroombp.h"
#include "config.h"
#include "autoconf.h"

typedef uint32_t OS_TIME_t;



#define EDROOMBP_TICKS_TO_TIMESPEC(_timespec, _ticks)                  \
	do                                                                 \
	{                                                                  \
		_timespec->seconds = _ticks / (CLICKS_PER_SEC * 1);            \
		_ticks -= _timespec->seconds * (CLICKS_PER_SEC * 1);           \
		_timespec->microseconds = _ticks * 1000000 / (CLICKS_PER_SEC); \
	} while (0)

#define EDROOMBP_TIMESPEC_NORMALIZE(t)          \
	{                                           \
		if ((t)->microseconds >= USECS_PER_SEC) \
		{                                       \
			(t)->microseconds -= USECS_PER_SEC; \
			(t)->seconds++;                     \
		}                                       \
	}

#define EDROOMBP_TIMESPEC_ADD(t1, t2)             \
	do                                            \
	{                                             \
		(t1)->microseconds += (t2)->microseconds; \
		(t1)->seconds += (t2)->seconds;           \
		EDROOMBP_TIMESPEC_NORMALIZE(t1);          \
	} while (0)

#define EDROOMBP_TIMESPEC_ADD_NS(t, n)  \
	do                                  \
	{                                   \
		(t)->microseconds += (n);       \
		EDROOMBP_TIMESPEC_NORMALIZE(t); \
	} while (0)

#define EDROOMBP_TIMESPEC_NZ(t) ((t)->seconds != 0 || (t)->microseconds != 0)

#define EDROOMBP_TIMESPEC_LT(t1, t2) ((t1)->seconds < (t2)->seconds ||   \
									  ((t1)->seconds == (t2)->seconds && \
									   (t1)->microseconds < (t2)->microseconds))

#define EDROOMBP_TIMESPEC_GT(t1, t2) (EDROOMBP_TIMESPEC_LT(t2, t1))

#define EDROOMBP_TIMESPEC_GE(t1, t2) (0 == EDROOMBP_TIMESPEC_LT(t1, t2))

#define EDROOMBP_TIMESPEC_LE(t1, t2) (0 == EDROOMBP_TIMESPEC_GT(t1, t2))

#define EDROOMBP_TIMESPEC_EQ(t1, t2) ((t1)->seconds == (t2)->seconds && \
									  (t1)->microseconds == (t2)->microseconds)

#define YEAR 2000
#define MONTH 1
#define DAY 1

static EDROOMTimeSpec start_time;

//******************************************************
//****************  Pr_Kernel **************************
//******************************************************

/*				PR_KERNEL FUNCTIONALITY		*/
int edroombp_system_init(int level)
{
}

Pr_Kernel::Pr_Kernel()
{
}

void Pr_Kernel::Start()
{
}


//******************************************************
//****************  Pr_Task ****************************
//******************************************************

//****************  CONSTRUCTORS ***********************

/*k_tid_t k_thread_create(struct k_thread *new_thread,
							k_thread_stack_t *stack, size_t stack_size,
							 k_thread_entry_t entry, void *p1,
							 void *p2, void *p3, int prio,
							 uint32_t options, k_timeout_t delay)

	new_thread – Pointer to uninitialized struct k_thread
	stack – Pointer to the stack space.
	stack_size – Stack size in bytes.
	entry – Thread entry function.
	p1 – 1st entry point parameter.
	p2 – 2nd entry point parameter.
	p3 – 3rd entry point parameter.
	prio – Thread priority.
	options – Thread options.
	delay – Scheduling delay, or K_NO_WAIT (for no delay).*/

#define TOTAL_SYSTEM_STACK_SIZE 1024*64*16
static K_THREAD_STACK_DEFINE(Pr_Task_stack_area, TOTAL_SYSTEM_STACK_SIZE);


static uint64_t const * Pr_Task_stack_area_pointer = (uint64_t *)Pr_Task_stack_area;
static uint64_t * Pr_Task_next_task_stack_area_pointer = (uint64_t *) Pr_Task_stack_area_pointer;

#define SYSTEM_STACK_ADDRESS_LIMIT (Pr_Task_stack_area_pointer + TOTAL_SYSTEM_STACK_SIZE/8)


Pr_Task::Pr_Task(Pr_TaskRV_t (*_taskCode)(Pr_TaskP_t ignored), /*  Task IP */
			TEDROOMPriority _priority, /*  task priority   */
			unsigned _stackSize): /*  stack size  */
	semSend(0), semReceive(0)
{
	

	//Check if taskCode is not null
    if (_taskCode){
    
		//Check stack
		uint64_t * next_task_stack_area_pointer=Pr_Task_next_task_stack_area_pointer;
	
		next_task_stack_area_pointer += ((_stackSize/8) + 1) ;

		if(next_task_stack_area_pointer<SYSTEM_STACK_ADDRESS_LIMIT){

			/*  Stablish the task instruction pointer   */
			this->TaskIP = _taskCode;

			taskStackPointer = (k_thread_stack_t*) Pr_Task_next_task_stack_area_pointer;
		
			Pr_Task_next_task_stack_area_pointer=next_task_stack_area_pointer;
		
			priorityMsg = _priority;
			priorityTmp = _priority;
			old_prio = _priority;

		    
			this->k_tid=edroombp_task_create(&taskID, taskStackPointer, (k_thread_entry_t) Pr_Task::TaskFunction, (int)priorityTmp, 0, K_FOREVER,this);
			
			edroombp_task_start(&taskID);
		}
	}
}


TEDROOMPriority Pr_Task::GetTmpTaskPrio()
{
	return (TEDROOMPriority)edroombp_task_get_current_priority(&taskID);
}

Pr_TaskRV_t Pr_Task::TaskFunction(void *pPr_Task , void *p2, void *p3)
{

	Pr_Task *mythis = (Pr_Task*) pPr_Task;
	
	mythis->taskID.custom_data=pPr_Task; 
	mythis->TaskIP(0);
	
}

void Pr_Task::SetPriority(TEDROOMPriority _priority)
{
	priorityMsg = _priority;
	if (priorityTmp != priorityMsg)
	{
		priorityTmp = priorityMsg;
		ChangePriority(_priority);
	}
}

void Pr_Task::SetMaxPrioTmp(TEDROOMPriority _priority)
{
	if (_priority < priorityTmp)
	{
		priorityTmp = _priority;
		ChangePriority(_priority);
	}
}

void Pr_Task::SetMaxPrioMsg(TEDROOMPriority _priority)
{
	if (_priority < priorityMsg)
	{
		priorityMsg = _priority;
		if (priorityMsg < priorityTmp)
		{
			priorityTmp = priorityMsg;
			ChangePriority(_priority);
		}
	}
}

void Pr_Task::RestorePrioMsg(void)
{
	if (priorityTmp != priorityMsg)
	{
		priorityTmp = priorityMsg;
		ChangePriority(priorityMsg);
	}
}

TEDROOMPriority Pr_Task::GetTaskPrio()
{
	return priorityMsg;
}

void Pr_Task::ChangePriority(TEDROOMPriority _priority)
{
	edroombp_task_change_priority(&taskID, _priority);
}



//******************************************************
//****************  Pr_Time ****************************
//******************************************************

//****************  CONSTRUCTORS ***********************


Pr_Time::Pr_Time()
{
	time.seconds = 0;
	time.microseconds = 0;
}

Pr_Time::Pr_Time(const Pr_Time &_time)
{
	time.microseconds = _time.time.microseconds;
	time.seconds = _time.time.seconds;
}

Pr_Time::Pr_Time(EDROOMTimeSpec _time)
{
	time.seconds = _time.seconds;

	time.microseconds = _time.microseconds;
}

Pr_Time::Pr_Time(uint32_t _secs, uint32_t _usecs)
{
	time.seconds =  _secs;
	time.microseconds = _usecs;
}


void Pr_Time::GetTime(void)
{
	uint32_t currentTicks = k_uptime_ticks();
	time.seconds = currentTicks/CONFIG_SYS_CLOCK_TICKS_PER_SEC;
    time.microseconds =	currentTicks-((uint32_t)time.seconds*CONFIG_SYS_CLOCK_TICKS_PER_SEC);
}

EDROOMClockTicksType Pr_Time::GetTicks()
{
	return (EDROOMClockTicksType)k_uptime_ticks();
}


Pr_Time& Pr_Time::operator+=(const Pr_Time &_time)
{

	EDROOMBP_TIMESPEC_ADD(&time, &(_time.time));

	return *this;
}

Pr_Time& Pr_Time::operator-=(const Pr_Time &_time)
{
	//Check if the left operand seconds is higher than the right operand's
	if (_time.time.seconds <= time.seconds)
	{
		//substract the seconds.
		time.seconds -= _time.time.seconds;
		//Check the microseconds in the left operand against the right operand's
		if (_time.time.microseconds > time.microseconds)
		{
			if (time.seconds > 0)
			{
				time.seconds--;
				time.microseconds += USECS_PER_SEC - _time.time.microseconds;
			} else
			{
				time.seconds = time.microseconds = 0;
			}
		} else
		{
			time.microseconds -= _time.time.microseconds;
		}
	} else
	{
		time.seconds = time.microseconds = 0;
	}

	return *this;
}

Pr_Time& Pr_Time::operator=(const Pr_Time &_time)
{
	time.microseconds = _time.time.microseconds;
	time.seconds = _time.time.seconds;

	return *this;
}

int Pr_Time::operator==(const Pr_Time &_time)
{
	return (EDROOMBP_TIMESPEC_EQ(&time, &(_time.time)));
}

int Pr_Time::operator!=(const Pr_Time &_time)
{
	return (0 == EDROOMBP_TIMESPEC_EQ(&time, &(_time.time)));
}

int Pr_Time::operator>(const Pr_Time &_time)
{
	return (EDROOMBP_TIMESPEC_GT(&time, &(_time.time)));
}

int Pr_Time::operator<(const Pr_Time &_time)
{
	return (EDROOMBP_TIMESPEC_LT(&time, &(_time.time)));
}

int Pr_Time::operator>=(const Pr_Time &_time)
{
	return (EDROOMBP_TIMESPEC_GE(&time, &(_time.time)));
}

int Pr_Time::operator<=(const Pr_Time &_time)
{
	return (EDROOMBP_TIMESPEC_LE(&time, &(_time.time)));
}


EDROOMClockTicksType Pr_Time::Ticks() const
{
	EDROOMClockTicksType TimeInTicks;
	TimeInTicks = time.seconds;
	TimeInTicks = ((TimeInTicks * CONFIG_SYS_CLOCK_TICKS_PER_SEC)
	+ (time.microseconds / USEC_PER_TICK));

	return TimeInTicks;
}

void Pr_Time::RoudMicrosToTicks()
{

	uint32_t ticksFromMicroseconds;
	uint32_t microsecondsFromTicks;

	ticksFromMicroseconds = time.microseconds / USEC_PER_TICK;
	microsecondsFromTicks = ticksFromMicroseconds * USEC_PER_TICK;

	if (time.microseconds != microsecondsFromTicks)
	{
		microsecondsFromTicks += USEC_PER_TICK / 2;
		if (microsecondsFromTicks <= time.microseconds)
		{
			time.microseconds += USEC_PER_TICK;
			EDROOMBP_TIMESPEC_NORMALIZE(&time);
		}
	}
}

void Pr_DelayIn(const Pr_Time &_interval)
{
	EDROOMClockTicksType TimeInTicks;
	//calculate the ticks to sleep.
	TimeInTicks = _interval.Ticks();
	edroombp_wait_ticks(TimeInTicks);
}

void Pr_DelayAt(const Pr_Time &_time)
{

	EDROOMTimeSpec time;
	float sec1, sec2;
	uint32_t intervalo;

	uint32_t currentTicks = k_uptime_ticks();
	time.seconds = currentTicks/CONFIG_SYS_CLOCK_TICKS_PER_SEC;
    time.microseconds =	currentTicks-((uint32_t)time.seconds*CONFIG_SYS_CLOCK_TICKS_PER_SEC);
	

	if (start_time.seconds <= time.seconds)
	{
		time.seconds -= start_time.seconds;
		if (start_time.microseconds > time.microseconds)
		{
			if (time.seconds > 0)
			{

				time.seconds--;
				time.microseconds += USEC_PER_TICK - start_time.microseconds;
			} else
			{

				time.seconds = time.microseconds = 0;

			}
		} else
		{

	time.microseconds -= start_time.microseconds;
		}
	} else
	{
		time.seconds = time.microseconds = 0;
	}

	//------------ calculate the value of time to wake the task.

	sec1 = time.seconds + (float) (time.microseconds / USEC_PER_TICK);
	sec2 = _time.time.seconds
		+ (float) (_time.time.microseconds/ USEC_PER_TICK);

	if (sec2 > sec1)
	{
		sec2 -= sec1;

		time.seconds = (uint32_t) sec2;
		time.microseconds = (uint32_t) ((sec2 - time.seconds)* USEC_PER_TICK);
	} else
	{
		DEBUG("too late!");
		return; // zero delay
	}
	//interval to wait
	intervalo = (time.seconds * USEC_PER_TICK) + time.microseconds;
	//call to sleep.
	edroombp_wait_ticks(
			intervalo / CONFIG_SYS_CLOCK_TICKS_PER_SEC);

}

//********************************************************
//********************  Pr_Semaphore  ********************
//********************************************************

Pr_Semaphore::Pr_Semaphore(uint32_t _value)
{
	
}

//********************************************************
//********************  Pr_SemaphoreBin  *****************
//********************************************************

Pr_SemaphoreBin::Pr_SemaphoreBin(uint32_t _value) : Pr_Semaphore(_value)
{
	edroombp_semaphore_create(&sem, (int) _value, MAX_SEM_LIMIT);
}

void Pr_SemaphoreBin::Signal()
{
	edroombp_semaphore_release(&sem);
}

void Pr_SemaphoreBin::Wait()
{
	edroombp_semaphore_catch(&sem, K_FOREVER);
}


int32_t Pr_SemaphoreBin::WaitCond()
{
	return edroombp_semaphore_try_catch(&sem);
}

bool Pr_SemaphoreBin::WaitTimed(const Pr_Time &_waittime)
{
	uint32_t TimeInTicks;
	//ticks to wait.
	TimeInTicks = (uint32_t) _waittime.Ticks();

	//call to sleep the task.
	return (edroombp_semaphore_catch(&sem, K_MSEC(TimeInTicks)) == 0);
}

Pr_SemaphoreBin::~Pr_SemaphoreBin()
{

}

int Pr_SemaphoreBin::countSem(){
	return edroombp_semphore_count(&sem);
}
//********************************************************
//********************  Pr_SemaphoreRec  *****************
//********************************************************

Pr_SemaphoreRec::Pr_SemaphoreRec()
{
	edroombp_mutex_create(&mutex);
}

Pr_SemaphoreRec::Pr_SemaphoreRec(int32_t prioceiling)
{
	edroombp_mutex_create(&mutex);
}

void Pr_SemaphoreRec::Signal()
{
	edroombp_mutex_unlock(&mutex);
}

void Pr_SemaphoreRec::Wait()
{
	edroombp_mutex_lock(&mutex, K_FOREVER);
}

int32_t Pr_SemaphoreRec::WaitCond()
{
	return edroombp_mutex_try_catch(&mutex);
}

void Pr_Send(Pr_Task &_task, void *_p_data)
{
	//copy the pointer to the task's attribute.
	_task.p_data = _p_data;
	
	//signal the received data.
	_task.semReceive.Signal();
	//catch the send semaphore.
	_task.semSend.Wait();

}

void Pr_Receive(void * _p_data, unsigned _datalength)
{
	
	Pr_Task *receiver;
	uint8_t *p_dataAux;
	uint8_t *p_dataAuxReceiver;

	//get the task variable.
    k_tid_t tid= k_current_get(); 
    receiver = (Pr_Task*) tid->custom_data; 

	//catch the recieve semaphore.
	receiver->semReceive.Wait();

	p_dataAux = (uint8_t *) _p_data;

	p_dataAuxReceiver = (uint8_t *) (receiver->p_data);
	//copy the data.
	for (unsigned int i = 0; i < _datalength; ++i)
	{
		*(p_dataAux + i) = *(p_dataAuxReceiver + i);
	}
	//release the send semaphore.
	receiver->semSend.Signal();
}

Pr_SemaphoreRec::~Pr_SemaphoreRec()
{
}

int Pr_SemaphoreRec::countSem(){
	return edroombp_mutex_count(&mutex);
}

int Pr_SemaphoreRec::prioritySem(){
	return edroombp_mutex_priority(&mutex);
}


//********************************************************
//********************  Pr_IRQManager ********************
//********************************************************

int key;
void Pr_IRQManager::DisableAllIRQs(void)
{
	//disable all the IRQs.
	edroombp_disable_irqs(1);
}

void Pr_IRQManager::ApplyCurrentIRQMask(void)
{
	//enable all the IRQs.
	edroombp_enable_irqs(1);
}


void Pr_IRQManager::InstallIRQHandler(Pr_IRQHandler handler,
		uint8_t IRQLevel, uint8_t IRQVectorNumber)
{
	//install the handler for the given IRQ vector.
	edroombp_install_handler((void *) handler, IRQVectorNumber);
}

void Pr_IRQManager::DeinstallIRQHandler(uint8_t IRQLevel,
		uint8_t IRQVectorNumber)
{
	//Deinstall the handler for the given IRQ vector.
	edroombp_deinstall_handler(IRQVectorNumber);
}

void Pr_IRQManager::DisableIRQ(uint32_t IRQVectorNumber)
{
	ASSERT(IRQVectorNumber>=0x10);
	ASSERT(IRQVectorNumber<0x20);


	edroombp_mask_irq(IRQVectorNumber-0x10);

}

void Pr_IRQManager::EnableIRQ(uint32_t IRQVectorNumber)
{
	ASSERT(IRQVectorNumber>=0x10);
	ASSERT(IRQVectorNumber<0x20);

	edroombp_unmask_irq(IRQVectorNumber-0x10);

}

//********************************************************
//********************  Pr_IRQEvent ********************
//********************************************************

Pr_IRQEvent::Pr_IRQEvent(uint8_t IRQ) : irq_event(0)
{
	//Check the event id is correct.
	ASSERT(IRQ < 32);
	//set the flag.
	irq_event = (1 << IRQ);
	caller_task_id = 0;
	evento->events = (uint32_t) irq_event;
	k_event_init(evento);
}


void Pr_IRQEvent::Signal()
{
	//Check irq_event and caller_task_id are not zero
	ASSERT(irq_event != 0); ASSERT(caller_task_id != 0);
	//Signal the IRQ event.
	edroombp_event_signal(caller_task_id, irq_event, evento);

}

void Pr_IRQEvent::Wait()
{
	//Check that the irq_event is not zero.
	ASSERT(irq_event != 0);
	//catch the IRQ event.
	edroombp_event_catch(&caller_task_id, irq_event, evento);

}

bool Pr_IRQEvent::WaitTimed(Pr_Time _time)
{
	uint32_t TimeInTicks;
	//Check that the irq_event is not zero.
	ASSERT(irq_event != 0);
	//convert the time to Ticks.
	TimeInTicks = (uint32_t) _time.Ticks();
	//try to catch the IRQ event during a given period of time.
	return edroombp_event_timed_catch(&caller_task_id, irq_event, TimeInTicks, evento);
}

bool Pr_IRQEvent::WaitCond()
{
	//Check irq_event and caller_task_id are not zero
	ASSERT(irq_event != 0); ASSERT(caller_task_id == 0);
	//try to catch the IRQ event.
	return edroombp_event_try_catch(&caller_task_id, irq_event, evento);

}

