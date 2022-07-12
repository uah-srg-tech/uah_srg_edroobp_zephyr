/**
 * \file	ut_edroombp_pr_semaphore.cpp
 * \brief	This swpackage implements the test suites applied over the edroombp
 * 			Semaphore Manager (Pr_SemaphoreRec and Pr_SemaphoreBin) services
 * 			for the RTEMSAPI 4.8 improvement
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>

 * \internal
 * Created: 	12-nov-2013
 * Compiler: 	gcc,g++
 * Company:  Space Research Group, Universidad de Alcalá.
 * Copyright: Copyright (c) 2013, Santiago Carmona Meco
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 * This software is provided under the terms of the GNU General Public v2
 * Licence. A full copy of the GNU GPL is provided in the file COPYING
 * found in the development root of the project.
 *
 */

#include "ut_edroombp_pr_semaphore_iface_v1.h"

#include <stdio.h>


#define ERROR_NOTIFY()	CUT_EDROOMBP::WriteFail("error", __LINE__, __FILE__)



/*
 * The next set of tests are identified under the rtems improvement validation
 * test suites, but each of these tests has been adapted to be applied to  one
 * or more of the edroombp Semaphore Manager services
 */


/*------------------------------ WAIT_AND_SIGNAL -----------------------------*/

bool_t taskAOk=false;
bool_t taskBOk=false;

uint32_t sharedBinVal;
uint32_t sharedRecVal;
uint32_t sharedRecCeilingVal;

Pr_SemaphoreBin * pSemBinAux;
Pr_SemaphoreBin * pSemBin;
Pr_SemaphoreRec * pMutex;
Pr_SemaphoreRec * pMutex2;

static Pr_TaskRV_t TASK_A_SEMAPHORE(Pr_TaskP_t)
{
	/*it's going to increase the value protected by the SemaphoreBin*/
	pSemBin->Wait();

	pSemBinAux->Wait();/* This wait is to lock the task until the task B
	releases the semaphore and guaranteeing the correct execution order in the test */

	for (uint32_t i=0;i<50;i++)
	{

		if (0==sharedBinVal)
		{
			sharedBinVal=0;
		}
		else
		{
			return;
		}

	}
	sharedBinVal=1;
	pSemBin->Signal();

	/*it's going to increase the value protected by the SemaphoreRec*/
	pMutex->Wait();

	pSemBinAux->Wait();

	for (uint32_t i=0;i<50;i++)
	{

		if (0==sharedRecVal)
		{
			sharedRecVal=0;
		}
		else
		{
			return;
		}
	}
	sharedRecVal=1;
	pMutex->Signal();


	/*it's going to increase the value protected by the SemaphoreRec*/
	pMutex2->Wait();

	pSemBinAux->Wait();

	for (uint32_t i=0;i<50;i++)
	{

		if (0==sharedRecCeilingVal)
		{
			sharedRecCeilingVal=0;
		}
		else
		{
			return;
		}
	}
	sharedRecCeilingVal=1;
	pMutex2->Signal();

	taskAOk=true;

}

static Pr_TaskRV_t TASK_B_SEMAPHORE(Pr_TaskP_t)
{

	/*it's going to increase the value protected by the SemaphoreBin*/

	pSemBinAux->Signal();

	pSemBin->Wait();
	for (uint32_t i=0;i<50;i++)
	{

		if (1==sharedBinVal)
		{
			sharedBinVal=1;
		}
		else
		{
			return;
		}

	}
	pSemBin->Signal();


	pSemBinAux->Signal();

	/*it's going to increase the value protected by the SemaphoreRec*/
	pMutex->Wait();
	for (uint32_t i=0;i<50;i++)
	{

		if (1==sharedRecVal)
		{
			sharedRecVal=1;
		}
		else
		{
			return;
		}

	}
	pMutex->Signal();


	pSemBinAux->Signal();

	/*it's going to increase the value protected by the SemaphoreRec*/
	pMutex2->Wait();
	for (uint32_t i=0;i<50;i++)
	{

		if (1==sharedRecCeilingVal)
		{
			sharedRecCeilingVal=1;
		}
		else
		{
			return;
		}

	}
	pMutex2->Signal();

	taskBOk=true;

}


CUT_EDROOMBP_PR_SEMAPHORE_WAIT_AND_SIGNAL::CUT_EDROOMBP_PR_SEMAPHORE_WAIT_AND_SIGNAL(const char * id) :
										CUTBase(id, true, true, 9873829),
										kernel(),
										semAux(0),
										semRec(),
										semBin(1),
										semRecCeiling(1),
										taskA(TASK_A_SEMAPHORE, EDROOMprioNormal, 2048),
										taskB(TASK_B_SEMAPHORE, EDROOMprioNormal, 2048),
										pTaskAOk(&taskAOk),
										pTaskBOk(&taskBOk)
{
	sharedBinVal = 0;
	sharedRecVal = 0;
	sharedRecCeilingVal = 0;
	pSemBinAux = &semAux;
	pSemBin = &semBin;
	pMutex = &semRec;
	pMutex2 = &semRecCeiling;

}


bool_t CUT_EDROOMBP_PR_SEMAPHORE_WAIT_AND_SIGNAL::DoTest(void)
{

	Pr_DelayIn(Pr_Time(10,0));

	return (*pTaskAOk && *pTaskBOk);
}


/*---------------------------------- WAIT_COND -------------------------------*/



bool_t semRecLocked;
bool_t semRecCeilingLocked;
bool_t semBinLocked;

Pr_SemaphoreBin * pSemBinCond;
Pr_SemaphoreRec * pMutexCond;
Pr_SemaphoreRec * pMutexCeilingCond;

static Pr_TaskRV_t COND_A_SEMAPHORE(Pr_TaskP_t)
{
	Pr_Time stopTime(0,500000);

	if (pSemBinCond->WaitCond())
	{
		if (pMutexCeilingCond->WaitCond())
		{

			Pr_DelayIn(stopTime);
			if (!(pMutexCond->WaitCond()))
			{
				semRecLocked = true;
			}
			else
			{
				printf(" ERROR: pMutexCond\n");
			}
			Pr_DelayIn(stopTime);
		}
		else
		{
			printf(" ERROR: pMutexCeilingCond->WaitCond()\n");
		}
	}
	else
	{
		printf(" ERROR: pSemBinCond->WaitCond()\n");
	}

}

static Pr_TaskRV_t COND_B_SEMAPHORE(Pr_TaskP_t)
{
	Pr_Time stopTime(0,500000);
	if (pMutexCond->WaitCond())
	{
		Pr_DelayIn(stopTime);

		if (!(pSemBinCond->WaitCond()))
		{

			semBinLocked = true;
		}
		else
		{
			printf(" ERROR: pSemBinCond\n");
		}

		if (!(pMutexCeilingCond->WaitCond()))
		{

			semRecCeilingLocked = true;
		}
		else
		{
			printf(" ERROR: pMutexCeilingCond\n");
		}

	}

}

CUT_EDROOMBP_PR_SEMAPHORE_WAIT_COND::CUT_EDROOMBP_PR_SEMAPHORE_WAIT_COND(const char * id) :
								CUTBase(id, true, true, 9873829),
								kernel(),
								semRec(),
								semRecCeiling(1),
								semBin(1),
								taskA(COND_A_SEMAPHORE, EDROOMprioNormal, 2048),
								taskB(COND_B_SEMAPHORE, EDROOMprioNormal, 2048),
								pSemBinLockedOk(&semBinLocked),
								pSemRecLockedOk(&semRecLocked),
								pSemRecCeilingLockedOk(&semRecCeilingLocked)
{
	semRecLocked = false;
	semBinLocked = false;
	semRecCeilingLocked = false;


	pSemBinCond = &semBin;
	pMutexCond = &semRec;
	pMutexCeilingCond = &semRecCeiling;
}


bool_t CUT_EDROOMBP_PR_SEMAPHORE_WAIT_COND::DoTest(void)
{

	Pr_DelayIn(Pr_Time(10,0));
	
	return ((*pSemRecLockedOk) && (*pSemBinLockedOk) && (*pSemRecCeilingLockedOk));
}


/*--------------------------------- WAIT_TIMED -------------------------------*/

bool_t semBinTimedOk=false;
bool_t semBinLockedOk=false;

Pr_SemaphoreBin * pSemBinTimed;


static Pr_TaskRV_t TIMED_ANY_SEMAPHORE(Pr_TaskP_t)
{
	Pr_Time waitTime(0,500000);
	Pr_Time stopTime(1,0);

	if (pSemBinTimed->WaitTimed(waitTime))
	{
		semBinLockedOk=true;
	}

	Pr_DelayIn(stopTime);

}

static Pr_TaskRV_t TIMED_BIN_SEMAPHORE(Pr_TaskP_t)
{

	Pr_Time waitTime(0,500000);
	Pr_Time stopTime(0,200000);

	Pr_DelayIn(stopTime);

	if (!(pSemBinTimed->WaitTimed(waitTime)))
	{
		semBinTimedOk = true;
	}

}

CUT_EDROOMBP_PR_SEMAPHORE_WAIT_TIMED::CUT_EDROOMBP_PR_SEMAPHORE_WAIT_TIMED(const char * id) :
				CUTBase(id, true, true, 9873829),
				kernel(),
				semBin(1),
				taskA(TIMED_ANY_SEMAPHORE, EDROOMprioNormal, 2048),
				taskB(TIMED_BIN_SEMAPHORE, EDROOMprioNormal, 2048),
				pSemBinLockedOk(&semBinTimedOk),
				pSemBinTimedOk(&semBinTimedOk)
{


	pSemBinTimed = &semBin;

}


bool_t CUT_EDROOMBP_PR_SEMAPHORE_WAIT_TIMED::DoTest(void)
{

	Pr_DelayIn(Pr_Time(10,0));

	return ((*pSemBinLockedOk) && (*pSemBinTimedOk));
}



/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_04_030 ------------------------------*/
/*----------------------------------------------------------------------------*/

Pr_SemaphoreRec * pSem;
Pr_SemaphoreRec * pSemInherit;

uint32_t valclock = 0;
uint32_t t1val = 0;
uint32_t t2val = 0;

bool_t wait2Ok=false;

static Pr_TaskRV_t TASK_A_SEMAPHORE_2(Pr_TaskP_t)
{

	pSem->Wait();

	t1val = valclock;
	valclock++;

	pSem->Signal();

}

static Pr_TaskRV_t TASK_B_SEMAPHORE_2(Pr_TaskP_t)
{

	pSem->Wait();

	t2val = valclock;
	valclock++;

	pSem->Signal();

}

static Pr_TaskRV_t TASK_C_SEMAPHORE_2(Pr_TaskP_t)
{
	for (uint8_t i=0;i<2;i++)
	{
		pSem->Wait();

		Pr_Task	taskB(TASK_B_SEMAPHORE_2, EDROOMprioNormal, 2048);
		Pr_DelayIn(Pr_Time(0,100000));

		Pr_Task taskA(TASK_A_SEMAPHORE_2, EDROOMprioHigh, 2048);
		Pr_DelayIn(Pr_Time(0,100000));

		pSem->Signal();

		if (t1val>t2val)
		{
			return;
		}
		pSem=pSemInherit;
	}


	wait2Ok=true;
}


CUT_EDROOMBP_PR_SEMAPHORE_WAIT_AND_SIGNAL_2::CUT_EDROOMBP_PR_SEMAPHORE_WAIT_AND_SIGNAL_2(const char * id) :
										CUTBase(id, true, true, 9873829),
										kernel(),
										taskC(TASK_C_SEMAPHORE_2, EDROOMprioLow, 2048),
										semCeiling(1),
										semInherit(),
										pTestOk(&wait2Ok)
{
	pSem=&semCeiling;
	pSemInherit=&semInherit;

}


bool_t CUT_EDROOMBP_PR_SEMAPHORE_WAIT_AND_SIGNAL_2::DoTest(void)
{

	Pr_DelayIn(Pr_Time(10,0));
	

	return (*pTestOk);
}


/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_04_020 ------------------------------*/
/*----------------------------------------------------------------------------*/

bool_t semBinReleased=false;
bool_t semRecDelReleased=false;
bool_t semRecReleased=false;

Pr_SemaphoreBin * pSemBinDeleted;
Pr_SemaphoreRec * pMutexDeleted;

static Pr_TaskRV_t USE_SEMAPHORE_BIN(Pr_TaskP_t)
{
	pSemBinDeleted->Wait();

	semBinReleased = true;
}

static Pr_TaskRV_t DELETE_SEMAPHORE_BIN(Pr_TaskP_t)
{

	pSemBinDeleted->~Pr_SemaphoreBin();
	pSemBinDeleted->Signal();

}

static Pr_TaskRV_t USE_AND_DEL_SEMAPHORE_REC(Pr_TaskP_t)
{

	Pr_Time timeTask(1,0);

	pMutexDeleted->Wait();

	Pr_DelayIn(timeTask);

	pMutexDeleted->~Pr_SemaphoreRec();
	pMutexDeleted->Signal();


	Pr_DelayIn(timeTask);

	semRecDelReleased = true;

}

static Pr_TaskRV_t USE_SEMAPHORE_REC(Pr_TaskP_t)
{

	pMutexDeleted->Wait();


	if (false == semRecDelReleased)
	{
		semRecReleased = true;
	}

}


CUT_EDROOMBP_PR_SEMAPHORE_DELETE_SEMAPHORES::CUT_EDROOMBP_PR_SEMAPHORE_DELETE_SEMAPHORES(const char * id) :
				CUTBase(id, true, true, 9873829),
				kernel(),
				semRec(),
				semBin(0),
				taskUseBin(USE_SEMAPHORE_BIN, EDROOMprioNormal, 2048),
				taskDelBin(DELETE_SEMAPHORE_BIN, EDROOMprioNormal, 2048),
				taskUseDelRec(USE_AND_DEL_SEMAPHORE_REC, EDROOMprioHigh, 2048),
				taskUseRec(USE_SEMAPHORE_REC, EDROOMprioNormal, 2048),
				pSemBinOk(&semBinReleased),
				pSemRecDelOk(&semRecDelReleased),
				pSemRecOk(&semRecReleased)
{
	semBinReleased = false;
	semRecDelReleased = false;
	semRecReleased = false;

	pSemBinDeleted = &semBin;
	pMutexDeleted = &semRec;
}



bool_t CUT_EDROOMBP_PR_SEMAPHORE_DELETE_SEMAPHORES::DoTest(void)
{

	Pr_DelayIn(Pr_Time(10,0));

	return ((*pSemBinOk) && (*pSemRecDelOk) && (*pSemRecOk));
}


/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_04_070 ------------------------------*/
/*----------------------------------------------------------------------------*/

const TEDROOMPriority max_priority = EDROOMprioVeryHigh;
const TEDROOMPriority mid_priority = EDROOMprioNormal;
const TEDROOMPriority min_priority = EDROOMprioVeryLow;

Pr_SemaphoreBin * pSemBinSuspendA;
Pr_SemaphoreBin * pSemBinSuspendB;
Pr_SemaphoreRec * pMutexCeiling;

Pr_Task * pTaskC;

bool_t semRecCeilingTaskB=false;
bool_t semRecCeilingTestOk=false;

uint32_t check = 0;

static Pr_TaskRV_t PRIO_CEILING_TASK_A(Pr_TaskP_t)
{

	pSemBinSuspendA->Wait(); //to simulate the suspend of the thread

	/* obtaining semaphore */
	pMutexCeiling->Wait();
	/* releasing semaphore  */
	pMutexCeiling->Signal();

	/* this is used to verify if the execution of the threads is
	 * in the correct order */

	check = 1;

}

static Pr_TaskRV_t PRIO_CEILING_TASK_B(Pr_TaskP_t)
{

	pSemBinSuspendB->Wait();	//to simulate the suspend of the thread

	if (1 == check)/*Check the correct order of the execution tasks*/
	{
		semRecCeilingTaskB = true;
	}
}

static Pr_TaskRV_t PRIO_CEILING_TASK_C(Pr_TaskP_t)
{
	TEDROOMPriority current_priority;

	/* obtaining task initial priority */

	current_priority = pTaskC->GetTmpTaskPrio();

	if (min_priority != current_priority)
	{
		printf(
				"min_priority!=current_priority\n\tmin_priority=%d\n\tcurrent_priority=%d",
				min_priority, current_priority);
		return;
	}

	pMutexCeiling->Wait();
	/* obtaining semaphore task priority should change now */

	/* checking current task priority */
	current_priority = pTaskC->GetTmpTaskPrio();

	if (current_priority != max_priority)
	{
		printf(
				"current_priority!=max_priority\n\tcurrent_priority=%d\n\tmax_priority=%d",
				current_priority, max_priority);
		return;
	}

	/* resuming middle priority thread */
	pSemBinSuspendB->Signal();	//to simulate resume service

	/* resuming thread_1 */
	pSemBinSuspendA->Signal();	//to simulate resume service

	/* releasing semaphore */
	pMutexCeiling->Signal();

	/* getting current task priority to check if lower again */
	current_priority = pTaskC->GetTmpTaskPrio();
	if (current_priority != min_priority)
	{
		printf(
				"current_priority!=min_priority\n\tcurrent_priority=%d\n\tmin_priority=%d",
				current_priority, min_priority);
		return;
	}
	semRecCeilingTestOk = true;
}

CUT_EDROOMBP_PR_SEMAPHORE_INCREASED_CEILING::CUT_EDROOMBP_PR_SEMAPHORE_INCREASED_CEILING(const char * id) :
						CUTBase(id, true, true, 9873829),
						kernel(),
						semBinSuspendA(0),
						semBinSuspendB(0),
						semRecCeiling(max_priority),
						taskA(PRIO_CEILING_TASK_A, max_priority, 2048),
						taskB(PRIO_CEILING_TASK_B, mid_priority, 2048),
						taskC(PRIO_CEILING_TASK_C, min_priority, 2048),
						pExpectedBVal(&semRecCeilingTaskB),
						pTestOk(&semRecCeilingTestOk)
{
	semRecCeilingTaskB = false;
	semRecCeilingTestOk = false;

	pSemBinSuspendA = &semBinSuspendA;
	pSemBinSuspendB = &semBinSuspendB;
	pMutexCeiling = &semRecCeiling;

	pTaskC = &taskC;

}


bool_t CUT_EDROOMBP_PR_SEMAPHORE_INCREASED_CEILING::DoTest(void)
{

	Pr_DelayIn(Pr_Time(10,0));

	return ((*pExpectedBVal) && (*pTestOk));
}


/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_04_080 ------------------------------*/
/*----------------------------------------------------------------------------*/

Pr_SemaphoreBin * pSemBinInheritanceA;
Pr_SemaphoreBin * pSemBinInheritanceB;
Pr_SemaphoreBin * pSemBinInheritanceC;
Pr_SemaphoreRec * pMutexInheritance;

Pr_Task * pTaskD;

bool_t semRecInheritanceTaskA=false;
bool_t semRecInheritanceTaskB=false;
bool_t semRecInheritanceTaskC=false;
bool_t semRecInheritanceTestOk=false;

uint32_t last = 0;

static Pr_TaskRV_t PRIORITY_INHERITANCE_1_TASK_A(Pr_TaskP_t)
{
	/* self suspending */
	pSemBinInheritanceA->Wait();

	/* obtaining semaphore */
	pMutexInheritance->Wait();

	/* releasing semaphore */
	pMutexInheritance->Signal();

	/* first thread finnishing */
	if (last)
	{
		printf("last is not 0");
		return;
	}
	last = 1;
	semRecInheritanceTaskA = true;
}

static Pr_TaskRV_t PRIORITY_INHERITANCE_1_TASK_B(Pr_TaskP_t)
{

	/* self suspending */
	pSemBinInheritanceB->Wait();

	/* obtaining semaphore */
	pMutexInheritance->Wait();

	/* releasing semaphore */
	pMutexInheritance->Signal();

	/* second thread finnishing */
	if (1 != last)
	{
		printf("last is not 1");
		return;
	}
	last = 2;
	semRecInheritanceTaskB = true;

}

static Pr_TaskRV_t PRIORITY_INHERITANCE_1_TASK_C(Pr_TaskP_t)
{

	/* self suspending */
	pSemBinInheritanceC->Wait();

	/* obtaining semaphore */
	pMutexInheritance->Wait();

	/* releasing semaphore */
	pMutexInheritance->Signal();

	/* third thread finnishing */
	if (2 != last)
	{
		printf("last is not 2");
		return;
	}
	last = 3;
	semRecInheritanceTaskC = true;

}

/*
 * this is the lower priority task this task should have is priority
 * changed to higher value because owns a semaphore that the other
 * tasks are trying to obtain, the priority values of this task should
 * change in the following order
 * 1st priority value equal to task C
 * 2nd priority value equal to task B
 * 3rd priority value equal to task A to finish is run
 * after release the semaphore the priority value should return to is
 * initial value
 */
static Pr_TaskRV_t PRIORITY_INHERITANCE_1_TASK_D(Pr_TaskP_t)
{
	TEDROOMPriority current_priority;

	/* getting the initial priority of the task */
	current_priority = pTaskD->GetTmpTaskPrio();

	if (EDROOMprioLow != current_priority)
	{
		printf(
				"EDROOMprioLow!=current_priority\n\EDROOMprioLow=%d\n\tcurrent_priority=%d",
				EDROOMprioLow, current_priority);
		return;
	}

	/* obtaining semaphore */
	pMutexInheritance->Wait();

	/* resuming task C */
	pSemBinInheritanceC->Signal();

	/* checking current task priority, the priority value should now
	 * equal to priority value of task C */
	current_priority = pTaskD->GetTmpTaskPrio();

	if (EDROOMprioNormal != current_priority)
	{
		printf(
				"EDROOMprioNormal!=current_priority\n\EDROOMprioNormal=%d\n\tcurrent_priority=%d",
				EDROOMprioNormal, current_priority);
		return;
	}

	/* resuming task B */
	pSemBinInheritanceB->Signal();

	/* checking current task priority, the priority value should now
	 * equal to priority value of task B */
	current_priority = pTaskD->GetTmpTaskPrio();

	if (EDROOMprioHigh != current_priority)
	{
		printf(
				"EDROOMprioHigh!=current_priority\n\EDROOMprioHigh=%d\n\tcurrent_priority=%d",
				EDROOMprioHigh, current_priority);
		return;
	}

	/* resuming task A */
	pSemBinInheritanceA->Signal();

	/* checking current task priority, the priority value should now
	 * equal to priority value of task A */
	current_priority = pTaskD->GetTmpTaskPrio();

	if (EDROOMprioVeryHigh != current_priority)
	{
		printf(
				"EDROOMprioVeryHigh!=current_priority\n\EDROOMprioVeryHigh=%d\n\tcurrent_priority=%d",
				EDROOMprioVeryHigh, current_priority);
		return;
	}

	/* releasing semaphore, after this step task should loose processor
	 * to task A because the semaphore is a priority policy queueing
	 * semaphore */
	pMutexInheritance->Signal();

	/* this test verifies that this task have loose the processor after
	 * release the semaphore */
	if (3 != last)
	{
		printf("last is not 3");
		return;
	}

	/* getting current task priority, the value of priority should be
	 * the initial task priority value */
	current_priority = pTaskD->GetTmpTaskPrio();

	if (EDROOMprioLow != current_priority)
	{
		printf(
				"EDROOMprioLow!=current_priority\n\EDROOMprioLow=%d\n\tcurrent_priority=%d",
				EDROOMprioLow, current_priority);
		return;
	}
	semRecInheritanceTestOk = true;

}

CUT_EDROOMBP_PR_SEMAPHORE_PRIORITY_INHERITANCE_1::CUT_EDROOMBP_PR_SEMAPHORE_PRIORITY_INHERITANCE_1(const char * id) :
				CUTBase(id, true, true, 9873829),
				kernel(),
				semBinSuspendA(0),
				semBinSuspendB(0),
				semBinSuspendC(0),
				semRecInheritance(),
				taskA(PRIORITY_INHERITANCE_1_TASK_A, EDROOMprioVeryHigh, 2048),
				taskB(PRIORITY_INHERITANCE_1_TASK_B, EDROOMprioHigh, 2048),
				taskC(PRIORITY_INHERITANCE_1_TASK_C, EDROOMprioNormal, 2048),
				taskD(PRIORITY_INHERITANCE_1_TASK_D, EDROOMprioLow, 2048),
				pExpectedAVal(&semRecInheritanceTaskA),
				pExpectedBVal(&semRecInheritanceTaskB),
				pExpectedCVal(&semRecInheritanceTaskC),
				pTestOk(&semRecInheritanceTestOk)
{

	semRecInheritanceTaskA = false;
	semRecInheritanceTaskB = false;
	semRecInheritanceTaskC = false;
	semRecInheritanceTestOk = false;
	last = 0;
	pSemBinInheritanceA = &semBinSuspendA;
	pSemBinInheritanceB = &semBinSuspendB;
	pSemBinInheritanceC = &semBinSuspendC;
	pMutexInheritance = &semRecInheritance;
	pTaskD = &taskD;

}



bool_t CUT_EDROOMBP_PR_SEMAPHORE_PRIORITY_INHERITANCE_1::DoTest(void)
{

	Pr_DelayIn(Pr_Time(10,0));

	return ((*pExpectedAVal) && (*pExpectedBVal) && (*pExpectedCVal)
			&& (*pTestOk));
}


/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_04_090 ------------------------------*/
/*----------------------------------------------------------------------------*/

Pr_SemaphoreBin * pSemBinInheritance2A;
Pr_SemaphoreBin * pSemBinInheritance2B;
Pr_SemaphoreBin * pSemBinInheritance2C;
Pr_SemaphoreRec * pMutexInheritance2A;	//sid1
Pr_SemaphoreRec * pMutexInheritance2B;	//sid2

Pr_Task * pTask2D;

bool_t semRecInheritance2TaskA=false;
bool_t semRecInheritance2TaskB=false;
bool_t semRecInheritance2TaskC=false;
bool_t semRecInheritance2TestOk=false;

uint32_t last2 = 0;

static Pr_TaskRV_t PRIORITY_INHERITANCE_2_TASK_A(Pr_TaskP_t)
{
	printf("Tarea A empieza\n");
	/* self suspending so this way task B could start */
	printf("Tarea A contador pSemBinInheritance2A 1 %i\n",pSemBinInheritance2A->countSem());
	pSemBinInheritance2A->Wait();
	printf("Tarea A contador pSemBinInheritance2A 2 %i\n",pSemBinInheritance2A->countSem());


	/* obtaining semaphore */
	printf("Tarea A contador pMutexInheritance2A 1 %i\n",pMutexInheritance2A->countSem());

	pMutexInheritance2A->Wait();
	printf("Tarea A contador pMutexInheritance2A 2 %i\n",pMutexInheritance2A->countSem());


	/* releasing semaphore */
	printf("Tarea A contador pMutexInheritance2A 3 %i\n",pMutexInheritance2A->countSem());

	pMutexInheritance2A->Signal();
	printf("Tarea A contador pMutexInheritance2A 4 %i\n",pMutexInheritance2A->countSem());

	/* first thread finishing */
	if (0 != last2)
	{
		printf("last2 is not 0");
		return;
	}

	last2 = 1;
	semRecInheritance2TaskA = true;
	printf("Tarea A termina\n");

	printf("AHORA DEBERIA COGERLO EL SEM B\n");
}

static Pr_TaskRV_t PRIORITY_INHERITANCE_2_TASK_B(Pr_TaskP_t)
{
	printf("Tarea B empieza\n");
	/* self suspending so this way task_2 could start */
	printf("Tarea B contador pSemBinInheritance2B 1 %i\n",pSemBinInheritance2B->countSem());

	pSemBinInheritance2B->Wait();
	printf("Tarea B contador pSemBinInheritance2B 2 %i\n",pSemBinInheritance2B->countSem());


	/* obtaining semaphore */
	printf("Tarea B contador pMutexInheritance2B 1 %i\n",pMutexInheritance2B->countSem());

	pMutexInheritance2B->Wait();
	printf("Tarea B contador pMutexInheritance2B 2 %i\n",pMutexInheritance2B->countSem());


	/* releasing semaphore */
	printf("Tarea B contador pMutexInheritance2B 3 %i\n",pMutexInheritance2B->countSem());

	pMutexInheritance2B->Signal();
	printf("Tarea B contador pMutexInheritance2B 4 %i\n",pMutexInheritance2B->countSem());


	/* second thread finnishing */
	if (1 != last2)
	{
		printf("last2 is not 1");
		return;
	}

	last2 = 2;
	semRecInheritance2TaskB = true;
	printf("Tarea B termina\n");

}

static Pr_TaskRV_t PRIORITY_INHERITANCE_2_TASK_C(Pr_TaskP_t)
{
	printf("Tarea C empieza\n");
	/* self suspending so this way task_2 could start */
	printf("Tarea C contador pSemBinInheritance2C 1 %i\n",pSemBinInheritance2C->countSem());

	pSemBinInheritance2C->Wait();
	printf("Tarea C contador pSemBinInheritance2C 2 %i\n",pSemBinInheritance2C->countSem());
	

	/* obtaining semaphore */
	printf("Tarea C contador pMutexInheritance2A 5 %i\n",pMutexInheritance2A->countSem());

	pMutexInheritance2A->Wait();
	printf("Tarea C contador pMutexInheritance2A 6 %i\n",pMutexInheritance2A->countSem());


	/* releasing semaphore */
	printf("Tarea C contador pMutexInheritance2A 7 %i\n",pMutexInheritance2A->countSem());

	pMutexInheritance2A->Signal();
	printf("Tarea C contador pMutexInheritance2A 8 %i\n",pMutexInheritance2A->countSem());


	/* third thread finnishing */
	if (2 != last2)
	{
		printf("last2 is not 2\n");
		return;
	}

	last2 = 3;
	semRecInheritance2TaskC = true;
	printf("Tarea C termina\n");
}

static Pr_TaskRV_t PRIORITY_INHERITANCE_2_TASK_D(Pr_TaskP_t)
{
	printf("Tarea D empieza\n");
	TEDROOMPriority current_priority;
	
	/* getting task initial priority */
	printf("PRIORIDAD pTask2D 1 %i \n", pTask2D->GetTmpTaskPrio());
	current_priority = pTask2D->GetTmpTaskPrio();
	printf("PRIORIDAD pTask2D 2 %i \n", pTask2D->GetTmpTaskPrio());


	if (EDROOMprioLow != current_priority)
	{
		printf(
				"EDROOMprioLow!=current_priority\n EDROOMprioLow=%d\n\tcurrent_priority=%d",
				EDROOMprioLow, current_priority);
		return;
	}

	/* obtaining semaphore task priority should change now */
	printf("contador pMutexInheritance2B 5 %i\n",pMutexInheritance2B->countSem());

	pMutexInheritance2B->Wait();
	printf("contador pMutexInheritance2B 6 %i\n",pMutexInheritance2B->countSem());

	printf("contador pMutexInheritance2A 9 %i\n",pMutexInheritance2A->countSem());

	pMutexInheritance2A->Wait();
	printf("contador pMutexInheritance2A 10 %i\n",pMutexInheritance2A->countSem());


	/* resuming task C */
	printf("contador pSemBinInheritance2C 3 %i\n",pSemBinInheritance2C->countSem());

	pSemBinInheritance2C->Signal();
	printf("contador pSemBinInheritance2C 4 %i\n",pSemBinInheritance2C->countSem());


	/* checking current task priority */
	printf("PRIORIDAD pTask2D 3 %i \n", pTask2D->GetTmpTaskPrio());

	current_priority = pTask2D->GetTmpTaskPrio();
	printf("PRIORIDAD pTask2D 4 %i \n", pTask2D->GetTmpTaskPrio());


	if (EDROOMprioNormal != current_priority)
	{
		printf(
				"EDROOMprioNormal!=current_priority\n EDROOMprioNormal=%d\n\tcurrent_priority=%d",
				EDROOMprioNormal, current_priority);
		return;
	}

	/* resuming task B */
	printf("contador pSemBinInheritance2B 3 %i\n",pSemBinInheritance2B->countSem());

	pSemBinInheritance2B->Signal();
	printf("contador pSemBinInheritance2B 4 %i\n",pSemBinInheritance2B->countSem());


	/* checking current task priority */
	printf("PRIORIDAD pTask2D 5 %i \n", pTask2D->GetTmpTaskPrio());

	current_priority = pTask2D->GetTmpTaskPrio();
	printf("PRIORIDAD pTask2D 6 %i \n", pTask2D->GetTmpTaskPrio());


	if (EDROOMprioHigh != current_priority)
	{
		printf(
				"EDROOMprioHigh!=current_priority\n EDROOMprioHigh=%d\n\tcurrent_priority=%d",
				EDROOMprioHigh, current_priority);
		return;
	}

	/* resuming task A */
	printf("contador pSemBinInheritance2A 3 %i\n",pSemBinInheritance2A->countSem());
	pSemBinInheritance2A->Signal();
	printf("contador pSemBinInheritance2A 4 %i\n",pSemBinInheritance2A->countSem());


	/* checking current task priority */
	printf("PRIORIDAD pTask2D 7 %i \n", pTask2D->GetTmpTaskPrio());

	current_priority = pTask2D->GetTmpTaskPrio();
	printf("PRIORIDAD pTask2D 8 %i \n", pTask2D->GetTmpTaskPrio());


	if (EDROOMprioVeryHigh != current_priority)
	{
		printf(
				"EDROOMprioVeryHigh!=current_priority\n EDROOMprioVeryHigh=%d\n\tcurrent_priority=%d",
				EDROOMprioVeryHigh, current_priority);
		return;
	}

	/* releasing semaphore */
	printf("contador pMutexInheritance2A 11 %i\n",pMutexInheritance2A->countSem());

	pMutexInheritance2A->Signal(); //Cuando suelta este, debería coger el B, pero coge el C
	printf("contador pMutexInheritance2A 12 %i\n",pMutexInheritance2A->countSem());


	/* checking current task priority */
	
	printf("PRIORIDAD pTask2D 9 %i \n", pTask2D->GetTmpTaskPrio());

	current_priority = pTask2D->GetTmpTaskPrio();
	printf("PRIORIDAD pTask2D 10 %i \n", pTask2D->GetTmpTaskPrio());
	
	if (EDROOMprioHigh != current_priority)
	{
		printf(
				"EDROOMprioHigh!=current_priority\n\tEDROOMprioHigh=%d\n\tcurrent_priority=%d\n",
				EDROOMprioHigh, current_priority);
		return;
	}

	printf("contador pMutexInheritance2B 7 %i\n",pMutexInheritance2B->countSem());

	pMutexInheritance2B->Signal();
	printf("contador pMutexInheritance2B 8 %i\n",pMutexInheritance2B->countSem());


	/* checking current task priority */
	printf("PRIORIDAD pTask2D 11 %i \n", pTask2D->GetTmpTaskPrio());

	current_priority = pTask2D->GetTmpTaskPrio();
	printf("PRIORIDAD pTask2D 12 %i \n", pTask2D->GetTmpTaskPrio());


	if (EDROOMprioLow != current_priority)
	{
		printf(
				"EDROOMprioLow!=current_priority\n EDROOMprioLow=%d\n\tcurrent_priority=%d",
				EDROOMprioLow, current_priority);
		return;
	}

	/* last thread finishing it looses priority when
	 * release semaphore */
	if (3 != last2)
	{
		printf("last2 is not 3");
		return;
	}

	/* getting current task priority to check if lower again */
	printf("PRIORIDAD pTask2D 13 %i \n", pTask2D->GetTmpTaskPrio());
	
	current_priority = pTask2D->GetTmpTaskPrio();
	printf("PRIORIDAD pTask2D 14 %i \n", pTask2D->GetTmpTaskPrio());


	if (EDROOMprioLow != current_priority)
	{
		printf(
				"EDROOMprioLow!=current_priority\n EDROOMprioLow=%d\n\tcurrent_priority=%d",
				EDROOMprioLow, current_priority);
		return;
	}

	semRecInheritance2TestOk = true;
	printf("Tarea D termina\n");

}


CUT_EDROOMBP_PR_SEMAPHORE_PRIORITY_INHERITANCE_2::CUT_EDROOMBP_PR_SEMAPHORE_PRIORITY_INHERITANCE_2(const char * id) :
				CUTBase(id, true, true, 9873829),
				kernel(),
				semBinSuspendA(0),
				semBinSuspendB(0),
				semBinSuspendC(0),
				semRecInheritanceA(),
				semRecInheritanceB(),
				taskA(PRIORITY_INHERITANCE_2_TASK_A, EDROOMprioVeryHigh, 2048),
				taskB(PRIORITY_INHERITANCE_2_TASK_B, EDROOMprioHigh, 2048),
				taskC(PRIORITY_INHERITANCE_2_TASK_C, EDROOMprioNormal, 2048),
				taskD(PRIORITY_INHERITANCE_2_TASK_D, EDROOMprioLow, 2048),
				pExpectedAVal(&semRecInheritance2TaskA),
				pExpectedBVal(&semRecInheritance2TaskB),
				pExpectedCVal(&semRecInheritance2TaskC),
				pTestOk(&semRecInheritance2TestOk)
{

	semRecInheritance2TaskA = false;
	semRecInheritance2TaskB = false;
	semRecInheritance2TaskC = false;
	semRecInheritance2TestOk = false;
	last2 = 0;
	pSemBinInheritance2A = &semBinSuspendA;
	pSemBinInheritance2B = &semBinSuspendB;
	pSemBinInheritance2C = &semBinSuspendC;
	pMutexInheritance2A = &semRecInheritanceA;
	pMutexInheritance2B = &semRecInheritanceB;
	pTask2D = &taskD;

}


bool_t CUT_EDROOMBP_PR_SEMAPHORE_PRIORITY_INHERITANCE_2::DoTest(void)
{

	Pr_DelayIn(Pr_Time(10,0));

	return ((*pExpectedAVal) && (*pExpectedBVal) && (*pExpectedCVal)
			&& (*pTestOk));
}


/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_04_110 ------------------------------*/
/*----------------------------------------------------------------------------*/

/**
 * \todo review the behaviour of this test
 */

bool_t semaphoreNestedObtainOk=false;

static Pr_TaskRV_t SEMAPHORE_NESTED_OBTAIN_TASK(Pr_TaskP_t)
{
	Pr_SemaphoreBin semCounting(0xffffffff);
	semCounting.Wait();
	semCounting.Wait();
	semCounting.Wait();
	semCounting.~Pr_SemaphoreBin();


	Pr_SemaphoreRec semRec(4);
	semRec.Wait();
	semRec.Wait();
	semRec.WaitCond();
	semRec.Signal();
	semRec.Signal();
	semRec.Signal();
	semRec.~Pr_SemaphoreRec();
	semaphoreNestedObtainOk = true;

}

CUT_EDROOMBP_PR_SEMAPHORE_SEMAPHORE_NESTED_OBTAIN::CUT_EDROOMBP_PR_SEMAPHORE_SEMAPHORE_NESTED_OBTAIN(const char * id) :
				CUTBase(id, true, true, 9873829),
				kernel(),
				mainTask(SEMAPHORE_NESTED_OBTAIN_TASK, EDROOMprioNormal, 2048),
				pTestOk(&semaphoreNestedObtainOk)
{
	semaphoreNestedObtainOk = false;
}


bool_t CUT_EDROOMBP_PR_SEMAPHORE_SEMAPHORE_NESTED_OBTAIN::DoTest(void)
{

	Pr_DelayIn(Pr_Time(10,0));

	return (*pTestOk);
}


/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_04_120 ------------------------------*/
/*----------------------------------------------------------------------------*/

Pr_SemaphoreBin * pSemCountingFIFO;
Pr_SemaphoreRec * pSemBinaryPriority;

volatile int lclock = 0;
volatile int tthread1 = 0;
volatile int tthread2 = 0;

bool_t queueingDisciplineOk=false;

static Pr_TaskRV_t COUNTING_FIFO_TASK_A(Pr_TaskP_t)
{
	pSemCountingFIFO->Wait();

	/* given clock value to this thread check variable */
	tthread1 = lclock;
	/* increasing the logic clock */
	lclock++;
	pSemCountingFIFO->Signal();

}

static Pr_TaskRV_t COUNTING_FIFO_TASK_B(Pr_TaskP_t)
{

	pSemCountingFIFO->Wait();

	/* given clock value to this thread check variable */
	tthread2 = lclock;
	/* increasing the logic clock */
	lclock++;
	pSemCountingFIFO->Signal();

}

static Pr_TaskRV_t BINARY_PRIORITY_TASK_A(Pr_TaskP_t)
{

	pSemBinaryPriority->Wait();
	/* given clock value to this thread check variable */
	tthread1 = lclock;
	/* increasing the logic clock */
	lclock++;
	pSemBinaryPriority->Signal();

}

static Pr_TaskRV_t BINARY_PRIORITY_TASK_B(Pr_TaskP_t)
{

	pSemBinaryPriority->Wait();
	/* given clock value to this thread check variable */
	tthread2 = lclock;
	/* increasing the logic clock */
	lclock++;
	pSemBinaryPriority->Signal();

}

static Pr_TaskRV_t QUEUEING_DISCIPLINE_TASK_CONTROL(Pr_TaskP_t)
{
	{

		pSemCountingFIFO->Wait();
		/* now we go start threads */
		Pr_Task taskB(COUNTING_FIFO_TASK_B, EDROOMprioNormal, 2048); 
		Pr_Task taskA(COUNTING_FIFO_TASK_A, EDROOMprioNormal, 2048); //EDROOMprioHigh

		pSemCountingFIFO->Signal();

		/* check if the execution order is correct */
		if (tthread1 < tthread2)
		{
			printf("tthread1 < tthread2\n\ttthread1=%d\n\ttthread2=%d\n", tthread1,
					tthread2);
			return;
		}
		pSemCountingFIFO->~Pr_SemaphoreBin();
		pSemCountingFIFO->Signal();

	}
	lclock = 0;
	{

		pSemBinaryPriority->Wait();

		/* now we go start threads */
		Pr_Task taskB(BINARY_PRIORITY_TASK_B, EDROOMprioNormal, 2048); 
		Pr_Task taskA(BINARY_PRIORITY_TASK_A, EDROOMprioHigh, 2048);
		pSemBinaryPriority->Signal();

		/* check if the execution order is correct */
		if (tthread1 > tthread2)
		{
			printf("tthread1 > tthread2\n\ttthread1=%d\n\ttthread2=%d", tthread1,
					tthread2);
			return;
		}
		pSemBinaryPriority->~Pr_SemaphoreRec();

		pSemBinaryPriority->Signal();

	}

	queueingDisciplineOk = true;

}

CUT_EDROOMBP_PR_SEMAPHORE_QUEUEING_DISCIPLINE::CUT_EDROOMBP_PR_SEMAPHORE_QUEUEING_DISCIPLINE(const char * id) :
				CUTBase(id, true, true, 9873829),
				kernel(),
				semCountingFIFO(1),
				semBinaryPriority(1),
				taskControl(QUEUEING_DISCIPLINE_TASK_CONTROL, EDROOMprioLow, 2048),
				pTestOk(&queueingDisciplineOk)
{
	queueingDisciplineOk = false;
	pSemCountingFIFO = &semCountingFIFO;
	pSemBinaryPriority = &semBinaryPriority;

}


bool_t CUT_EDROOMBP_PR_SEMAPHORE_QUEUEING_DISCIPLINE::DoTest(void)
{

	Pr_DelayIn(Pr_Time(10,0));

	return (*pTestOk);
}


/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_04_130 ------------------------------*/
/*----------------------------------------------------------------------------*/

const TEDROOMPriority tAprio = EDROOMprioVeryHigh;
const TEDROOMPriority tBprio = EDROOMprioVeryLow;
const TEDROOMPriority ceilprio = EDROOMprioNormal;

Pr_SemaphoreBin * pSemSuspendA;
Pr_SemaphoreBin * pSemSuspendB;
Pr_SemaphoreRec * pSemCeilingA;
Pr_SemaphoreRec * pSemCeilingB;

bool_t priorityCeilingA;
bool_t priorityCeilingB;

Pr_Task * pTaskACeiling;
Pr_Task * pTaskBCeiling;

static Pr_TaskRV_t PRIORITY_CEILING_TASK_A(Pr_TaskP_t)
{
	TEDROOMPriority current_priority;

	/* self suspend for being resumed by thread2 */
	pSemSuspendA->Wait();

	/* trying to obtain the semaphore */
	pSemCeilingA->Wait();

	/* check for thread pripority */
	current_priority = pTaskACeiling->GetTmpTaskPrio();
	if (tAprio != current_priority)
	{
		printf("tAprio!=current_priority\n\tAprio=%d\n\tcurrent_priority=%d",
				tAprio, current_priority);
		return;
	}

	pSemCeilingA->Signal();

	pSemSuspendA->Wait();

	/* this obtain fails and the semaphore should not be obtained
	 * by this thread after fails the semaphore should still free */

	pSemCeilingB->Wait();


	priorityCeilingA=true;

}

static Pr_TaskRV_t PRIORITY_CEILING_TASK_B(Pr_TaskP_t)
{
	TEDROOMPriority current_priority;

	/* getting semaphore */

	pSemCeilingA->Wait();


	/* getting current priority */
	current_priority = pTaskBCeiling->GetTmpTaskPrio();
	/* verify if the current priority is what we espect */
	if (ceilprio != current_priority)
	{
		printf(
				"ceilprio!=current_priority\n\tceilprio=%d\n\tcurrent_priority=%d",
				ceilprio, current_priority);
		return;
	}

	/* resumes the first thread */

	pSemSuspendA->Signal();

	/* check if have changes in priority value */
	current_priority = pTaskBCeiling->GetTmpTaskPrio();
	/* verify if the current priority is what we espect */
	if (ceilprio != current_priority)
	{
		printf("ceilprio!=current_priority\n\tceilprio=%d\n\tcurrent_priority=%d",
				ceilprio, current_priority);
		return;
	}


	pSemCeilingA->Signal();

	/* check priority value again this time we have here low priority again */
	current_priority = pTaskBCeiling->GetTmpTaskPrio();
	/* verify if the current priority is what we espect */
	if (tBprio != current_priority)
	{
		printf("tBprio!=current_priority\n\ttBprio=%d\n\tcurrent_priority=%d",
				tBprio, current_priority);
		return;
	}


	// pSemCeilingA->~Pr_SemaphoreRec();

	/* resumes the first thread */
	pSemSuspendA->Signal();

	pSemCeilingB->Wait();

	/* check priority value again this time we have here low priority again */
	current_priority = pTaskBCeiling->GetTmpTaskPrio();
	/* verify if the current priority is what we espect */
	if (ceilprio != current_priority)
	{
		printf("ceilprio!=current_priority\n\tceilprio=%d\n\tcurrent_priority=%d",
				ceilprio, current_priority);
		return;
	}

	priorityCeilingB=true;
}

CUT_EDROOMBP_PR_SEMAPHORE_PRIORITY_CEILING::CUT_EDROOMBP_PR_SEMAPHORE_PRIORITY_CEILING(const char * id) :
						CUTBase(id, true, true, 9873829),
						kernel(),
						semCeilingA(ceilprio),
						semCeilingB(ceilprio),
						semSuspendA(0),
						semSuspendB(0),
						taskA(PRIORITY_CEILING_TASK_A, tAprio, 2048),
						taskB(PRIORITY_CEILING_TASK_B, tBprio, 2048),
						pTestAOk(&priorityCeilingA),
						pTestBOk(&priorityCeilingB)
{
	/*
	 * TODO:corregir test
	 */
	priorityCeilingA = false;
	priorityCeilingB = false;
	pTaskBCeiling = &taskB;
	pTaskACeiling = &taskA;
	pSemSuspendA = &semSuspendA;
	pSemSuspendB = &semSuspendB;
	pSemCeilingA = &semCeilingA;
	pSemCeilingB = &semCeilingB;
}

bool_t CUT_EDROOMBP_PR_SEMAPHORE_PRIORITY_CEILING::DoTest(void)
{

	Pr_DelayIn(Pr_Time(10,0));

	return ((*pTestAOk) && (*pTestBOk));
}


/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_04_140 ------------------------------*/
/*----------------------------------------------------------------------------*/

const TEDROOMPriority taskPrioCeilingNestedA = EDROOMprioURGENT;	//1
const TEDROOMPriority taskPrioCeilingNestedB = EDROOMprioHigh;	//3
const TEDROOMPriority taskPrioCeilingNestedC = EDROOMprioLow;	//5
const TEDROOMPriority taskPrioCeilingNestedD = EDROOMprioIDLE;	//7

Pr_SemaphoreRec * pSemCeilingNestedA;
Pr_SemaphoreRec * pSemCeilingNestedB;
Pr_SemaphoreRec * pSemCeilingNestedC;

Pr_SemaphoreBin * pSemCeilingNestedSuspendA;
Pr_SemaphoreBin * pSemCeilingNestedSuspendB;
Pr_SemaphoreBin * pSemCeilingNestedSuspendC;

Pr_Task * pTaskCeilingNestedA;
Pr_Task * pTaskCeilingNestedB;
Pr_Task * pTaskCeilingNestedC;
Pr_Task * pTaskCeilingNestedD;

bool_t ceilingNestedTaskA=false;
bool_t ceilingNestedTaskB=false;
bool_t ceilingNestedTaskC=false;
bool_t ceilingNestedTaskD=false;

static Pr_TaskRV_t CEILING_NESTED_OBTAIN_TASK_A(Pr_TaskP_t)
{
	printf("EMPEZAMOS\n");
	/* let suspend ourselves in order to task D "resume" (with signal the
	 * semaphore) us later */
	pSemCeilingNestedSuspendA->Wait();

	pSemCeilingNestedA->Wait();

	if (!(CheckPriorities(taskPrioCeilingNestedB,
			pTaskCeilingNestedD->GetTmpTaskPrio(),
			"CEILING_NESTED_OBTAIN_TASK_A")))
	{
		return;
	}

	pSemCeilingNestedA->Signal();

	ceilingNestedTaskA = true;

}
static Pr_TaskRV_t CEILING_NESTED_OBTAIN_TASK_B(Pr_TaskP_t)
{

	/* let suspend ourselves in order to task D resume us later */
	pSemCeilingNestedSuspendB->Wait();

	pSemCeilingNestedB->Wait();

	/* here we go check the current priority of task_4 */
	if (!(CheckPriorities(taskPrioCeilingNestedC,
			pTaskCeilingNestedD->GetTmpTaskPrio(),
			"CEILING_NESTED_OBTAIN_TASK_B")))
	{
		return;
	}

	pSemCeilingNestedB->Signal();

	ceilingNestedTaskB = true;
}

static Pr_TaskRV_t CEILING_NESTED_OBTAIN_TASK_C(Pr_TaskP_t)
{

	/* let suspend ourselves in order to task D resume us later */
	pSemCeilingNestedSuspendC->Wait();

	pSemCeilingNestedC->Wait();
	/* here we go check the current priority of task_4 */
	if (!(CheckPriorities(taskPrioCeilingNestedD,
			pTaskCeilingNestedD->GetTmpTaskPrio(),
			"CEILING_NESTED_OBTAIN_TASK_C")))
	{
		return;
	}

	pSemCeilingNestedC->Signal();
	ceilingNestedTaskC = true;

}

static Pr_TaskRV_t CEILING_NESTED_OBTAIN_TASK_D(Pr_TaskP_t)
{
	/* check current priority */

	if (!(CheckPriorities(taskPrioCeilingNestedD,
			pTaskCeilingNestedD->GetTmpTaskPrio(),
			"CEILING_NESTED_OBTAIN_TASK_D (1)")))
	{
		return;
	}

	pSemCeilingNestedC->Wait();

	/* resuming thread C */
	pSemCeilingNestedSuspendC->Signal();

	/* check current priority */

	if (!(CheckPriorities(taskPrioCeilingNestedC,
			pTaskCeilingNestedD->GetTmpTaskPrio(),
			"CEILING_NESTED_OBTAIN_TASK_D (2)")))
	{
		return;
	}

	pSemCeilingNestedB->Wait();
	/* resuming thread B */
	pSemCeilingNestedSuspendB->Signal();

	/* check current priority */
	if (!(CheckPriorities(taskPrioCeilingNestedB,
			pTaskCeilingNestedD->GetTmpTaskPrio(),
			"CEILING_NESTED_OBTAIN_TASK_D  (3)")))
	{
		return;
	}

	pSemCeilingNestedA->Wait();
	/* resuming thread A */
	pSemCeilingNestedSuspendA->Signal();

	/* check current priority */
	if (!(CheckPriorities(taskPrioCeilingNestedA,
			pTaskCeilingNestedD->GetTmpTaskPrio(),
			"CEILING_NESTED_OBTAIN_TASK_D  (4)")))
	{
		return;
	}

	pSemCeilingNestedA->Signal();
	//

	/* check current priority */
	if (!(CheckPriorities(taskPrioCeilingNestedB,
			pTaskCeilingNestedD->GetTmpTaskPrio(),
			"CEILING_NESTED_OBTAIN_TASK_D  (5)")))
	{
		return;
	}

	pSemCeilingNestedB->Signal();

	/* check current priority */
	if (!(CheckPriorities(taskPrioCeilingNestedC,
			pTaskCeilingNestedD->GetTmpTaskPrio(),
			"CEILING_NESTED_OBTAIN_TASK_D  (6)")))
	{
		return;
	}

	pSemCeilingNestedC->Signal();

	/* check current priority */

	if (!(CheckPriorities(taskPrioCeilingNestedD,
			pTaskCeilingNestedD->GetTmpTaskPrio(),
			"CEILING_NESTED_OBTAIN_TASK_D  (7)")))
	{
		return;
	}

	ceilingNestedTaskD = true;

}

CUT_EDROOMBP_PR_SEMAPHORE_CEILING_NESTED_OBTAIN::CUT_EDROOMBP_PR_SEMAPHORE_CEILING_NESTED_OBTAIN(const char * id) :
				CUTBase(id, true, true, 9873829),
				kernel(),
				taskA(CEILING_NESTED_OBTAIN_TASK_A, taskPrioCeilingNestedA, 2048),
				taskB(CEILING_NESTED_OBTAIN_TASK_B, taskPrioCeilingNestedB, 2048),
				taskC(CEILING_NESTED_OBTAIN_TASK_C, taskPrioCeilingNestedC, 2048),
				taskD(CEILING_NESTED_OBTAIN_TASK_D, taskPrioCeilingNestedD, 2048),
				semA(taskPrioCeilingNestedA),
				semB(taskPrioCeilingNestedB),
				semC(taskPrioCeilingNestedC),
				semBinSuspendA(0),
				semBinSuspendB(0),
				semBinSuspendC(0),
				pExpectedAVal(&ceilingNestedTaskA),
				pExpectedBVal(&ceilingNestedTaskB),
				pExpectedCVal(&ceilingNestedTaskC),
				pExpectedDVal(&ceilingNestedTaskD)
{
	ceilingNestedTaskA = false;
	ceilingNestedTaskB = false;
	ceilingNestedTaskC = false;
	ceilingNestedTaskD = false;

	pSemCeilingNestedSuspendA = &semBinSuspendA;
	pSemCeilingNestedSuspendB = &semBinSuspendB;
	pSemCeilingNestedSuspendC = &semBinSuspendC;

	pSemCeilingNestedA = &semA;
	pSemCeilingNestedB = &semB;
	pSemCeilingNestedC = &semC;

	pTaskCeilingNestedA = &taskA;
	pTaskCeilingNestedB = &taskB;
	pTaskCeilingNestedC = &taskC;
	pTaskCeilingNestedD = &taskD;

}

bool_t CUT_EDROOMBP_PR_SEMAPHORE_CEILING_NESTED_OBTAIN::DoTest(void)
{

	Pr_DelayIn(Pr_Time(10,0));

	return ((*pExpectedAVal) && (*pExpectedBVal) && (*pExpectedCVal)
			&& (*pExpectedDVal));
}


/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_04_160 ------------------------------*/
/*----------------------------------------------------------------------------*/

Pr_SemaphoreRec * pSemLowerPrio;

bool_t taskHigherPrioOk =false;


static Pr_TaskRV_t TASK_HIGHER_PRIO(Pr_TaskP_t)
{
	printf("Prioridad semáforo %i \n", pSemLowerPrio->prioritySem());
	if (!pSemLowerPrio->WaitCond())
	{
		taskHigherPrioOk = true;
	}

}


CUT_EDROOMBP_PR_SEMAPHORE_TASK_WITH_HIGHER_PRIORITY_THAN_CEILING::CUT_EDROOMBP_PR_SEMAPHORE_TASK_WITH_HIGHER_PRIORITY_THAN_CEILING(const char * id) :
						CUTBase(id, true, true, 9873829),
						kernel(),
						taskHigherPrio(TASK_HIGHER_PRIO, EDROOMprioVeryHigh, 2048),
						semCeiling(3),
						pExpectedVal(&taskHigherPrioOk)
{
	pSemLowerPrio = &semCeiling;
	taskHigherPrioOk= false;
}


bool_t CUT_EDROOMBP_PR_SEMAPHORE_TASK_WITH_HIGHER_PRIORITY_THAN_CEILING::DoTest(void)
{

	Pr_DelayIn(Pr_Time(10,0));

	return (*pExpectedVal);
}




/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_04_250 ------------------------------*/
/*----------------------------------------------------------------------------*/

Pr_Task * pTaskPriorityCeiling;

bool_t taskPriorityCeilingOk =false;

const TEDROOMPriority semTaskPriorityCeiling = EDROOMprioVeryHigh;	//5

static Pr_TaskRV_t TASK_PRIORITY_CEILING_SEMAPHORE(Pr_TaskP_t)
{

	Pr_SemaphoreRec semTaskPriorityCeiling(EDROOMprioVeryHigh);

	semTaskPriorityCeiling.Wait();

	if (!(CheckPriorities(EDROOMprioVeryHigh,
			pTaskPriorityCeiling->GetTmpTaskPrio(),
			"TASK_PRIORITY_CEILING_SEMAPHORE")))
	{
		return;
	}

	taskPriorityCeilingOk = true;
}


CUT_EDROOMBP_PR_SEMAPHORE_TASK_PRIORITY_CEILING_SEMAPHORE::CUT_EDROOMBP_PR_SEMAPHORE_TASK_PRIORITY_CEILING_SEMAPHORE(const char * id) :
						CUTBase(id, true, true, 9873829),
						kernel(),
						taskA(TASK_PRIORITY_CEILING_SEMAPHORE,semTaskPriorityCeiling,2048),
						pExpectedVal(&taskPriorityCeilingOk)
{
	pTaskPriorityCeiling = &taskA;
	taskPriorityCeilingOk= false;
}


bool_t CUT_EDROOMBP_PR_SEMAPHORE_TASK_PRIORITY_CEILING_SEMAPHORE::DoTest(void)
{

	Pr_DelayIn(Pr_Time(10,0));

	return (*pExpectedVal);
}





//////////////////////////////////////////////////////////////////////////

Pr_SemaphoreBin * pSemCounting;

bool_t taskCountingAOk =false;
bool_t taskCountingBOk =false;
bool_t taskCountingCOk =false;
bool_t taskCountingDOk =false;

static Pr_TaskRV_t TASK_COUNTING_A(Pr_TaskP_t)
{
	pSemCounting->Signal();
	pSemCounting->Signal();
	pSemCounting->Signal();
	taskCountingAOk = true;
}

static Pr_TaskRV_t TASK_COUNTING_B(Pr_TaskP_t)
{
	pSemCounting->Wait();

	taskCountingBOk = true;
}

static Pr_TaskRV_t TASK_COUNTING_C(Pr_TaskP_t)
{
	pSemCounting->Wait();
	taskCountingCOk = true;
}

static Pr_TaskRV_t TASK_COUNTING_D(Pr_TaskP_t)
{
	pSemCounting->Wait();
	taskCountingDOk = true;
}




CUT_EDROOMBP_PR_SEMAPHORE_COUNTING::CUT_EDROOMBP_PR_SEMAPHORE_COUNTING(const char * id) :
						CUTBase(id, true, true, 9873829),
						kernel(),
						taskA(TASK_COUNTING_A,semTaskPriorityCeiling,2048),
						taskB(TASK_COUNTING_B,semTaskPriorityCeiling,2048),
						taskC(TASK_COUNTING_C,semTaskPriorityCeiling,2048),
						taskD(TASK_COUNTING_D,semTaskPriorityCeiling,2048),
						sem(0),
						pExpectedA(&taskCountingAOk),
						pExpectedB(&taskCountingBOk),
						pExpectedC(&taskCountingCOk),
						pExpectedD(&taskCountingDOk)
{
	pSemCounting=&sem;
}


bool_t CUT_EDROOMBP_PR_SEMAPHORE_COUNTING::DoTest(void)
{

	Pr_DelayIn(Pr_Time(10,0));

	return (*pExpectedA && *pExpectedB && *pExpectedC && *pExpectedD);
}







/*----------------------------------------------------------------------------*/

bool_t CheckPriorities(TEDROOMPriority expectedPrio,
		TEDROOMPriority currentPrio, const char * nameTask)
{
	if (expectedPrio != currentPrio)
	{
		printf(
				"%s:expectedPrio!=currentPrio\n\texpectedPrio=%d\n\tcurrentPrio=%d \n",
				nameTask, expectedPrio, currentPrio);
		return false;
	}
	return true;
}



