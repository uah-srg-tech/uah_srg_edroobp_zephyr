/**
 * \file	ut_edroombp_pr_irq.cpp
 * \brief	This swpackage implements the EDROOMBP tests for the services provide
 * 			 by the Pr_IRQManager class and the Pr_IRQEvent	class
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>
 * \internal
 * Created: 	09/09/2014
 * Compiler: 	gcc,g++
 * Company:  Space Research Group, Universidad de Alcalá.
 * Copyright: Copyright (c) 2014, Santiago Carmona Meco
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 * This software is provided under the terms of the GNU General Public v2
 * Licence. A full copy of the GNU GPL is provided in the file COPYING
 * found in the development root of the project.
 *
 */

#include "ut_edroombp_pr_irq_iface_v1.h"

#include <stdio.h>

#define ERROR_NOTIFY()	CUT_EDROOMBP::WriteFail("error", __LINE__, __FILE__)


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*---------------------------- Pr_IRQManager Tests ---------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

volatile uint32_t * pMaskInterrupt=(uint32_t*)0x80000090;
volatile uint32_t * pForceInterrupt=(uint32_t*)0x80000098;

/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_07_010 ------------------------------*/
/*----------------------------------------------------------------------------*/

/*------------------------------ INSTALL HANDLER -----------------------------*/


bool_t testOk=false;


void IRQ_DUMMY_HANDLER(void)
{
	testOk=true;
}


CUT_EDROOMBP_PR_IRQ_MANAGER_INSTALL_HANDLER::CUT_EDROOMBP_PR_IRQ_MANAGER_INSTALL_HANDLER(const char * id) :
						CUTBase(id, true, true, 9873829),
						kernel(),
						pTestOk(&testOk)
{
	uint8_t IRQVectorNumber=0x12;

	/*Set enable the interrupt chosen*/
	*pMaskInterrupt|=0x4;

	Pr_IRQManager::InstallIRQHandler(IRQ_DUMMY_HANDLER,0,IRQVectorNumber);

	/*force the interrupt chosen*/
	*pForceInterrupt=0x4;


}

bool_t CUT_EDROOMBP_PR_IRQ_MANAGER_INSTALL_HANDLER::DoTest(void)
{
	Pr_DelayIn(Pr_Time(2,0));

	return (*pTestOk);
}


/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_07_020 ------------------------------*/
/*----------------------------------------------------------------------------*/

int semReleaseOrderControl = 1;

bool_t testSemaphoreReleaseOk=true;

Pr_SemaphoreRec * pSemRecISR;
Pr_SemaphoreBin * pSemBinISR;


void SEMAPHORE_REC_RELEASE_ISR(void)
{
	pSemRecISR->Signal();
}

static Pr_TaskRV_t TASK_A_SEM_REC_ISR(Pr_TaskP_t)
{

	if (!pSemRecISR->WaitCond())
	{
		Pr_DelayIn(Pr_Time(1,0));
	}
	else
	{
		DEBUG("");
		testSemaphoreReleaseOk=false;
	}


}



void SEMAPHORE_BIN_RELEASE_ISR(void)
{

	pSemBinISR->Signal();
}

static Pr_TaskRV_t TASK_A_SEM_BIN_ISR(Pr_TaskP_t)
{

	if (pSemBinISR->WaitCond())
	{
		Pr_DelayIn(Pr_Time(1,0));
	}
	else
	{
		DEBUG("");
		testSemaphoreReleaseOk=false;
	}

	pSemBinISR->Signal();



}

static Pr_TaskRV_t TASK_B_SEM_ISR(Pr_TaskP_t)
{
	/* Enable the IRQ chosen */
	*pMaskInterrupt|=0x8;

	/* Install handler to the IRQ that will be force */
	Pr_IRQManager::InstallIRQHandler(SEMAPHORE_REC_RELEASE_ISR,0,0x13);

	semReleaseOrderControl=1;

	{
		Pr_SemaphoreRec semaphoreISR;
		/*Obtain the semaphore*/
		semaphoreISR.Wait();
		pSemRecISR=&semaphoreISR;

		/* Force the IRQ force */
		*pForceInterrupt=(uint32_t)0x8;

		Pr_Task taskA(TASK_A_SEM_REC_ISR,EDROOMprioVeryHigh, 2048);
		Pr_DelayIn(Pr_Time(1,0));
		semaphoreISR.Signal();
	}
	semReleaseOrderControl++;
	if (testSemaphoreReleaseOk){

		Pr_SemaphoreRec semaphoreISR(0);
		/*Obtain the semaphore*/
		semaphoreISR.Wait();
		pSemRecISR=&semaphoreISR;

		/* Force the IRQ force */
		*pForceInterrupt=(uint32_t)0x8;


		Pr_Task taskA(TASK_A_SEM_REC_ISR,EDROOMprioVeryHigh, 2048);
		Pr_DelayIn(Pr_Time(1,0));
		semaphoreISR.Signal();
	}
	else
	{
		DEBUG("");
		return;
	}

	/* Install handler to the IRQ that will be force */
	Pr_IRQManager::InstallIRQHandler(SEMAPHORE_BIN_RELEASE_ISR,0,0x13);

	semReleaseOrderControl++;
	if (testSemaphoreReleaseOk)
	{
		Pr_SemaphoreBin semaphoreISR(0);
		pSemBinISR=&semaphoreISR;
		*pForceInterrupt=(uint32_t)0x8;
		Pr_Task taskA(TASK_A_SEM_BIN_ISR,EDROOMprioVeryHigh, 2048);
		Pr_DelayIn(Pr_Time(1,0));
	}


}


CUT_EDROOMBP_PR_IRQ_MANAGER_RELEASE_SEMAPHORE::CUT_EDROOMBP_PR_IRQ_MANAGER_RELEASE_SEMAPHORE(const char * id) :
						CUTBase(id, true, true, 9873829),
						kernel(),
						pTestOk(&testSemaphoreReleaseOk),
						taskB(TASK_B_SEM_ISR,EDROOMprioLow,2048)
{

}

bool_t CUT_EDROOMBP_PR_IRQ_MANAGER_RELEASE_SEMAPHORE::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));

	return (*pTestOk);

}


/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_07_040 ------------------------------*/
/*----------------------------------------------------------------------------*/

Pr_Time * pThreadTime;

bool_t testTimerOk=true;

volatile uint32_t _isr_fired_flag;

void GET_TIME_ISR(void)
{
	Pr_Time isrTime;
	(*pThreadTime)+=Pr_Time(1,0);
	isrTime.GetTime();
	isrTime+=Pr_Time(0,10000);/* We are having problems because sometimes the
					ticks expected are one unit less that the expected ticks. In the RTEMS CPU
					Kit Manual, we read: "A characteristic of interval timing is that the actual
					interval period may be a fraction of a tick less than the interval
					requested. This occurs because the time at which the delay timer is set
					up occurs at some time between two clock ticks. Therefore, the first
					countdown tick occurs in less than the complete time interval for a tick.
					This can be a problem if the clock granularity is large.*/

	if (!(isrTime>=(*pThreadTime)))
	{
		DEBUG("GET_TIME_ISR isrTime %d\npThreadTime %d\n",isrTime.Ticks(),pThreadTime->Ticks());
		testTimerOk=false;
	}

}


void GET_TICKS_ISR(void)
{

	Pr_Time isrTime;
	(*pThreadTime)+=Pr_Time(1,0);

	if (!(isrTime.GetTicks()>=pThreadTime->Ticks()))
	{
		DEBUG("GET_TICKS_ISR isrTime %d\npThreadTime %d\n",isrTime.Ticks(),pThreadTime->Ticks());
		testTimerOk=false;
	}

}


static Pr_TaskRV_t TASK_TIMER_ISR(Pr_TaskP_t)
{
	/* Enable the IRQ chosen */
	*pMaskInterrupt|=0x4;

	/* GET_TIME_ISR function */
	{
		Pr_Time time;
		time.GetTime();
		pThreadTime=&time;

		/* Install handler to the IRQ that will be force */
		Pr_IRQManager::InstallIRQHandler(GET_TIME_ISR,0,0x12);

		Pr_DelayIn(Pr_Time(1,0));


		/* Force the IRQ force */
		*pForceInterrupt=(uint32_t)0x4;

	}

	/* GET_TICKS_ISR function */
	{
		Pr_Time time;
		time.GetTicks();
		pThreadTime=&time;

		/* Install handler to the IRQ that will be force */
		Pr_IRQManager::InstallIRQHandler(GET_TICKS_ISR,0,0x12);

		Pr_DelayIn(Pr_Time(1,0));

		/* Force the IRQ force */
		*pForceInterrupt=(uint32_t)0x4;

	}


}


CUT_EDROOMBP_PR_IRQ_MANAGER_TIMER_OPERATIONS::CUT_EDROOMBP_PR_IRQ_MANAGER_TIMER_OPERATIONS(const char * id) :
						CUTBase(id, true, true, 9873829),
						kernel(),
						task(TASK_TIMER_ISR,EDROOMprioHigh,2048),
						pTestOk(&testTimerOk)
{

}

bool_t CUT_EDROOMBP_PR_IRQ_MANAGER_TIMER_OPERATIONS::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));

	return (*pTestOk);

}


/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_07_070 ------------------------------*/
/*----------------------------------------------------------------------------*/

Pr_Task *pTaskEnableDisable;

bool_t testEnableIRQOk=true;
bool_t testEnableDisableOk=false;


void ENABLE_ISR(void)
{
	testEnableIRQOk=true;
}

void DISABLE_ISR(void)
{
	testEnableIRQOk=false;
}


static Pr_TaskRV_t TASK_ENABLE_DISABLE_ISR(Pr_TaskP_t)
{
	*pMaskInterrupt|=0x4;

	/* Install new handler to the IRQ that will be force */
	Pr_IRQManager::InstallIRQHandler(DISABLE_ISR,0,0x12);
	/*Disable IRQ*/
	Pr_IRQManager::DisableIRQ(0x12);

	/* Force the IRQ force */
	*pForceInterrupt=(uint32_t)0x4;

	if (!testEnableIRQOk)
	{
		printf("1\n");
		return;
	}
	testEnableIRQOk=false;

	/* Install handler to the IRQ that will be force */
	Pr_IRQManager::InstallIRQHandler(ENABLE_ISR,0,0x12);
	/*Enable IRQ*/
	Pr_IRQManager::EnableIRQ(0x12);

	/* Force the IRQ force */
	*pForceInterrupt=(uint32_t)0x4;

	if (!testEnableIRQOk)
	{
		printf("2\n");
		return;
	}


	testEnableIRQOk=false;

	/*Disable all IRQs*/
	Pr_IRQManager::DisableAllIRQs();

	/* Force the IRQ force */
	*pForceInterrupt=(uint32_t)0x4;

	if (testEnableIRQOk)
	{
		printf("4\n");
		return;
	}

	/*TODO: In theory this method enable all masks*/
	Pr_IRQManager::ApplyCurrentIRQMask();

	/* Force the IRQ force */
	*pForceInterrupt=(uint32_t)0x4;

	if (!testEnableIRQOk)
	{
		printf("5\n");
		return;
	}
	testEnableDisableOk=true;
}



CUT_EDROOMBP_PR_IRQ_MANAGER_ENABLE_DISABLE::CUT_EDROOMBP_PR_IRQ_MANAGER_ENABLE_DISABLE(const char * id) :
						CUTBase(id, true, true, 9873829),
						kernel(),
						task(TASK_ENABLE_DISABLE_ISR,EDROOMprioHigh,2048),
						pTestIRQOk(&testEnableIRQOk),
						pTestOk(&testEnableDisableOk)

{
	pTaskEnableDisable=&task;

}

bool_t CUT_EDROOMBP_PR_IRQ_MANAGER_ENABLE_DISABLE::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));

	return (*pTestOk && pTestIRQOk);

}







/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------- Pr_IRQEvent Tests ----------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_08_010 ------------------------------*/
/*----------------------------------------------------------------------------*/

/*-----------------------------------WAIT-------------------------------------*/

Pr_IRQEvent *pIrqEventReceiveWait;

bool_t testIRQEventReceiveWait=false;


void ISR_EVENT_SIGNAL(void)
{
	pIrqEventReceiveWait->Signal();
}

static Pr_TaskRV_t TASK_EVENT_WAIT(Pr_TaskP_t)
{
	/* Enable the IRQ chosen */
	*pMaskInterrupt|=0x4;

	/* Install handler to the IRQ that will be force */
	Pr_IRQManager::InstallIRQHandler(ISR_EVENT_SIGNAL,0,0x12);

	/* Force the IRQ force */
	*pForceInterrupt=(uint32_t)0x4;

	pIrqEventReceiveWait->Wait();

	testIRQEventReceiveWait=true;

	pIrqEventReceiveWait->Wait();

	testIRQEventReceiveWait=false;


}



CUT_EDROOMBP_PR_IRQ_EVENT_RECEIVE_WAIT::CUT_EDROOMBP_PR_IRQ_EVENT_RECEIVE_WAIT(const char * id) :
								CUTBase(id, true, true, 9873829),
								kernel(),
								irqEvent(16),
								taskWait(TASK_EVENT_WAIT,EDROOMprioLow,2048),
								pTestOk(&testIRQEventReceiveWait)
{

	testIRQEventReceiveWait=false;
	pIrqEventReceiveWait=&irqEvent;

}

bool_t CUT_EDROOMBP_PR_IRQ_EVENT_RECEIVE_WAIT::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));
	return (*pTestOk);

}

/*-------------------------------WAIT COND------------------------------------*/

Pr_IRQEvent *pIrqEventReceiveWaitCond;

bool_t testIRQEventReceiveWaitCond=false;


void ISR_EVENT_SIGNAL_COND(void)
{
	pIrqEventReceiveWaitCond->Signal();
}

static Pr_TaskRV_t TASK_EVENT_WAIT_COND(Pr_TaskP_t)
{
	/* Enable the IRQ chosen */
	*pMaskInterrupt|=0x4;

	/* Install handler to the IRQ that will be force */
	Pr_IRQManager::InstallIRQHandler(ISR_EVENT_SIGNAL_COND,0,0x12);

	/*The event is not sent yet*/
	if (pIrqEventReceiveWaitCond->WaitCond())
	{
		DEBUG("");
		return;
	}
	/* Force the IRQ force */
	*pForceInterrupt=(uint32_t)0x4;

	/*The event was sent by the IRQ Handler*/
	if (pIrqEventReceiveWaitCond->WaitCond())
	{
		testIRQEventReceiveWaitCond=true;
	}

	/*The event was received by the previous Wait*/
	if (pIrqEventReceiveWaitCond->WaitCond())
	{
		testIRQEventReceiveWaitCond=false;
	}


}



CUT_EDROOMBP_PR_IRQ_EVENT_RECEIVE_WAIT_COND::CUT_EDROOMBP_PR_IRQ_EVENT_RECEIVE_WAIT_COND(const char * id) :
								CUTBase(id, true, true, 9873829),
								kernel(),
								irqEvent(16),
								taskWait(TASK_EVENT_WAIT_COND,EDROOMprioLow,2048),
								pTestOk(&testIRQEventReceiveWaitCond)
{

	testIRQEventReceiveWaitCond=false;
	pIrqEventReceiveWaitCond=&irqEvent;

}

bool_t CUT_EDROOMBP_PR_IRQ_EVENT_RECEIVE_WAIT_COND::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));
	return (*pTestOk);

}

/*------------------------------WAIT TIMED------------------------------------*/

Pr_IRQEvent *pIrqEventReceiveWaitTimed;

bool_t testIRQEventReceiveWaitTimed=false;


void ISR_EVENT_SIGNAL_TIMED(void)
{
	pIrqEventReceiveWaitTimed->Signal();
}

static Pr_TaskRV_t TASK_EVENT_WAIT_TIMED(Pr_TaskP_t)
{
	/* Enable the IRQ chosen */
	*pMaskInterrupt|=0x4;

	/* Install handler to the IRQ that will be force */
	Pr_IRQManager::InstallIRQHandler(ISR_EVENT_SIGNAL_TIMED,0,0x12);

	/*The event is not sent yet*/

	if (pIrqEventReceiveWaitTimed->WaitTimed(Pr_Time(1,0)))
	{
		DEBUG("");
		return;
	}
	/* Force the IRQ force */
	*pForceInterrupt=(uint32_t)0x4;

	/*The event was sent by the IRQ Handler*/
	if (pIrqEventReceiveWaitTimed->WaitTimed(Pr_Time(1,0)))
	{
		testIRQEventReceiveWaitTimed=true;
	}

	/*The event was received by the previous Wait*/
	if (pIrqEventReceiveWaitTimed->WaitTimed(Pr_Time(1,0)))
	{
		testIRQEventReceiveWaitTimed=false;
	}


}



CUT_EDROOMBP_PR_IRQ_EVENT_RECEIVE_WAIT_TIMED::CUT_EDROOMBP_PR_IRQ_EVENT_RECEIVE_WAIT_TIMED(const char * id) :
								CUTBase(id, true, true, 9873829),
								kernel(),
								irqEvent(16),
								taskWait(TASK_EVENT_WAIT_TIMED,EDROOMprioLow,2048),
								pTestOk(&testIRQEventReceiveWaitTimed)
{

	testIRQEventReceiveWaitTimed=false;
	pIrqEventReceiveWaitTimed=&irqEvent;

}

bool_t CUT_EDROOMBP_PR_IRQ_EVENT_RECEIVE_WAIT_TIMED::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));
	return (*pTestOk);

}



/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_08_020 ------------------------------*/
/*----------------------------------------------------------------------------*/

/*-----------------------------------WAIT-------------------------------------*/

Pr_IRQEvent *pIrqEventSendWait;

bool_t testIRQEventSendWait=false;

uint8_t irqEventSendWaitCount=0;

void ISR_EVENT_WAIT(void)
{
	pIrqEventSendWait->Wait();
	if (0==irqEventSendWaitCount)
	{
		testIRQEventSendWait=true;
	}
	else
	{
		testIRQEventSendWait=false;
	}
	irqEventSendWaitCount++;
}

static Pr_TaskRV_t TASK_EVENT_SEND_WAIT(Pr_TaskP_t)
{
	/* Enable the IRQ chosen */
	*pMaskInterrupt|=0x4;

	/* Install handler to the IRQ that will be force */
	Pr_IRQManager::InstallIRQHandler(ISR_EVENT_WAIT,0,0x12);

	pIrqEventSendWait->Signal();

	/* Force the IRQ force */
	*pForceInterrupt=(uint32_t)0x4;

	/* Force the IRQ force */
	*pForceInterrupt=(uint32_t)0x4;


}



CUT_EDROOMBP_PR_IRQ_EVENT_SEND_WAIT::CUT_EDROOMBP_PR_IRQ_EVENT_SEND_WAIT(const char * id) :
										CUTBase(id, true, true, 9873829),
										kernel(),
										irqEvent(17),
										taskSignal(TASK_EVENT_SEND_WAIT,EDROOMprioHigh,2048),
										pTestOk(&testIRQEventSendWait)
{

	testIRQEventSendWait=false;
	pIrqEventSendWait=&irqEvent;

}

bool_t CUT_EDROOMBP_PR_IRQ_EVENT_SEND_WAIT::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));

	return (*pTestOk);

}

/*-------------------------------WAIT COND------------------------------------*/

Pr_IRQEvent *pIrqEventSendWaitCond;

bool_t testIRQEventSendWaitCond=false;

uint8_t irqEventSendWaitCondCount=0;


void ISR_EVENT_WAIT_COND(void)
{
	switch(irqEventSendWaitCondCount)
	{
		case 0:
			if (!pIrqEventSendWaitCond->WaitCond())
			{
				testIRQEventSendWaitCond=true;
			}
			break;
		case 1:
			if (pIrqEventSendWaitCond->WaitCond())
			{
				testIRQEventSendWaitCond=true;
			}
			break;
		case 2:
			if (pIrqEventSendWaitCond->WaitCond())
			{
				DEBUG("");
				testIRQEventSendWaitCond=false;
			}
			break;
	}
	irqEventSendWaitCondCount++;
}

static Pr_TaskRV_t TASK_EVENT_SEND_WAIT_COND(Pr_TaskP_t)
{
	/* Enable the IRQ chosen */
	*pMaskInterrupt|=0x4;

	/* Install handler to the IRQ that will be force */
	Pr_IRQManager::InstallIRQHandler(ISR_EVENT_WAIT_COND,0,0x12);

	/* Force the IRQ force */
	*pForceInterrupt=(uint32_t)0x4;

	pIrqEventSendWaitCond->Signal();

	/* Force the IRQ force */
	*pForceInterrupt=(uint32_t)0x4;

	/* Force the IRQ force */
	*pForceInterrupt=(uint32_t)0x4;


}



CUT_EDROOMBP_PR_IRQ_EVENT_SEND_WAIT_COND::CUT_EDROOMBP_PR_IRQ_EVENT_SEND_WAIT_COND(const char * id) :
										CUTBase(id, true, true, 9873829),
										kernel(),
										irqEvent(17),
										taskSignal(TASK_EVENT_SEND_WAIT_COND,EDROOMprioHigh,2048),
										pTestOk(&testIRQEventSendWaitCond)
{

	testIRQEventSendWaitCond=false;
	pIrqEventSendWaitCond=&irqEvent;

}

bool_t CUT_EDROOMBP_PR_IRQ_EVENT_SEND_WAIT_COND::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));

	return (*pTestOk);

}


/*------------------------------WAIT TIMED------------------------------------*/

Pr_IRQEvent *pIrqEventSendWaitTimed;

bool_t testIRQEventSendWaitTimed=false;

uint8_t irqEventSendWaitTimedCount=0;

void ISR_EVENT_WAIT_TIMED(void)
{
	switch(irqEventSendWaitTimedCount)
	{
		case 0:
			if (!pIrqEventSendWaitTimed->WaitTimed(Pr_Time(1,0)))
			{
				testIRQEventSendWaitTimed=true;
			}
			break;
		case 1:
			if (pIrqEventSendWaitTimed->WaitTimed(Pr_Time(1,0)))
			{
				testIRQEventSendWaitTimed=true;
			}
			break;
		case 2:
			if (pIrqEventSendWaitTimed->WaitTimed(Pr_Time(1,0)))
			{
				DEBUG("");
				testIRQEventSendWaitTimed=false;
			}
			break;
	}
	irqEventSendWaitTimedCount++;
}

static Pr_TaskRV_t TASK_EVENT_SEND_WAIT_TIMED(Pr_TaskP_t)
{
	/* Enable the IRQ chosen */
	*pMaskInterrupt|=0x4;

	/* Install handler to the IRQ that will be force */
	Pr_IRQManager::InstallIRQHandler(ISR_EVENT_WAIT_COND,0,0x12);

	/* Force the IRQ force */
	*pForceInterrupt=(uint32_t)0x4;

	pIrqEventSendWaitTimed->Signal();

	/* Force the IRQ force */
	*pForceInterrupt=(uint32_t)0x4;

	/* Force the IRQ force */
	*pForceInterrupt=(uint32_t)0x4;
}



CUT_EDROOMBP_PR_IRQ_EVENT_SEND_WAIT_TIMED::CUT_EDROOMBP_PR_IRQ_EVENT_SEND_WAIT_TIMED(const char * id) :
										CUTBase(id, true, true, 9873829),
										kernel(),
										irqEvent(17),
										taskSignal(TASK_EVENT_SEND_WAIT_TIMED,EDROOMprioHigh,2048),
										pTestOk(&testIRQEventSendWaitTimed)
{

	testIRQEventSendWaitTimed=false;
	pIrqEventSendWaitTimed=&irqEvent;

}

bool_t CUT_EDROOMBP_PR_IRQ_EVENT_SEND_WAIT_TIMED::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));

	return (*pTestOk);

}




