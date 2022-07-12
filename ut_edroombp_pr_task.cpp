/**
 * \file	ut_edroombp_pr_task.cpp
 *
 * \brief	This swpackage implements the test suites applied over the edroombp
 * 			Task Manager (Pr_Task) services for the RTEMSAPI 4.8 improvement
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>
 * \internal
 * Created: 	06-nov-2013
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

#include "ut_edroombp_pr_task_iface_v1.h"

#include <stdio.h>


///*This include is required to the CUT_EDROOMBP_PR_TASK_DELETE_TASK test*/
//#include <public/rtemsapi_4_8_impr.h>

#define ERROR_NOTIFY()	CUT_EDROOMBP::WriteFail("error", __LINE__, __FILE__)

/*
 * The next set of tests are identified under the rtems improvement validation
 * test suites, but each of these tests has been adapted to be applied for one
 * or more of the edroombp Task Manager service
 */

/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_03_010 ------------------------------*/
/*----------------------------------------------------------------------------*/

bool_t taskSuccessOk=false;
bool_t taskStackSizeErrorOk=false;

static Pr_TaskRV_t TASK_SUCCESS(Pr_TaskP_t)
{
	taskSuccessOk = true;

}

static Pr_TaskRV_t TASK_STACK_SIZE_ERROR(Pr_TaskP_t)
{
	taskStackSizeErrorOk = true;
}

CUT_EDROOMBP_PR_TASK_CREATION_AND_START::CUT_EDROOMBP_PR_TASK_CREATION_AND_START(const char * id) :
				CUTBase(id, true, true, 9873829),
				kernel(),
				taskSuccess(TASK_SUCCESS, EDROOMprioVeryHigh, 512),
				taskCodeError(NULL, EDROOMprioVeryHigh, 512),
				taskStackSizeError(TASK_STACK_SIZE_ERROR, EDROOMprioVeryHigh,
						4 * 1024 * 1024),
						pTaskSuccessOk(&taskSuccessOk),
						pTaskStackSizeErrorOk(&taskStackSizeErrorOk)
{
	/* Initializing the test values*/
	taskSuccessOk = false;
	taskStackSizeErrorOk = false;
}



bool_t CUT_EDROOMBP_PR_TASK_CREATION_AND_START::DoTest(void)
{
	/*Instantiating the tests (objects) that we want to execute*/
	Pr_DelayIn(Pr_Time(10,0));
	return ((*pTaskSuccessOk) && (!(*pTaskStackSizeErrorOk)));

}

/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_03_020 ------------------------------*/
/*----------------------------------------------------------------------------*/

extern Pr_Task * pPr_Task;

static Pr_TaskRV_t TASK_DELETED(Pr_TaskP_t)
{
	
}

CUT_EDROOMBP_PR_TASK_DELETE_TASK::CUT_EDROOMBP_PR_TASK_DELETE_TASK(const char * id) :
		CUTBase(id, true, true, 9873829),
		kernel(),
		testOk(true)
{
	Pr_Task taskDeleted(TASK_DELETED, EDROOMprioVeryHigh, 512);
	Pr_DelayIn(Pr_Time(1,0));

	/*
	 * We try to restart the task with the aim of check that after to be
	 * executed was deleted, i.e., the rtems_task_restart must return a error
	 * state
	 */
	 //TODO: este test habrá que rediseñarlo para no usar primitivas de rtems
//	if (RTEMS_SUCCESSFUL== rtems_task_restart(taskDeleted.GetTaskID(),
//					(rtems_task_argument) pPr_Task))
//	{
//		testOk = false;
//	}

}


bool_t CUT_EDROOMBP_PR_TASK_DELETE_TASK::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));
	return (testOk);

}



/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_03_060 ------------------------------*/
/*----------------------------------------------------------------------------*/

Pr_Task *pTaskPriority;
bool_t testPriorityOk=true;

static Pr_TaskRV_t TASK_PRIORITY(Pr_TaskP_t)
{

	/*check the initial priority*/
	if (EDROOMprioNormal != pTaskPriority->GetTaskPrio()
			&& EDROOMprioNormal != pTaskPriority->GetTmpTaskPrio())
	{
		printf("ERROR: check the initial priority\n");
		testPriorityOk = false;
		return;
	}

	/*SetMaxPrioTmp*/
	pTaskPriority->SetMaxPrioTmp(EDROOMprioLow);

	if (EDROOMprioNormal != pTaskPriority->GetTaskPrio()
			&& EDROOMprioNormal != pTaskPriority->GetTmpTaskPrio())
	{
		printf("ERROR: pTaskPriority->SetMaxPrioTmp(EDROOMprioLow)\n");
		testPriorityOk = false;
		return;
	}

	pTaskPriority->SetMaxPrioTmp(EDROOMprioVeryHigh);

	if (EDROOMprioNormal != pTaskPriority->GetTaskPrio()
			&& EDROOMprioVeryHigh != pTaskPriority->GetTmpTaskPrio())
	{
		printf("ERROR: pTaskPriority->SetMaxPrioTmp(EDROOMprioVeryHigh)\n");
		testPriorityOk = false;
		return;
	}

	/*SetMaxPrioMsg*/
	pTaskPriority->SetMaxPrioMsg(EDROOMprioLow);
	if (EDROOMprioNormal != pTaskPriority->GetTaskPrio()
				&& EDROOMprioVeryHigh != pTaskPriority->GetTmpTaskPrio())
		{
			printf("ERROR: pTaskPriority->SetMaxPrioMsg(EDROOMprioHigh)\n");
			testPriorityOk = false;
			return;
		}


	pTaskPriority->SetMaxPrioMsg(EDROOMprioHigh);

	if (EDROOMprioHigh != pTaskPriority->GetTaskPrio()
			&& EDROOMprioVeryHigh != pTaskPriority->GetTmpTaskPrio())
	{
		printf("ERROR: pTaskPriority->SetMaxPrioMsg(EDROOMprioHigh)\n");
		testPriorityOk = false;
		return;
	}

	pTaskPriority->SetMaxPrioMsg(EDROOMprioURGENT);

	if (EDROOMprioURGENT != pTaskPriority->GetTaskPrio()
			&& EDROOMprioURGENT != pTaskPriority->GetTmpTaskPrio())
	{
		printf("ERROR: pTaskPriority->SetMaxPrioMsg(EDROOMprioURGENT)\n");
		testPriorityOk = false;
		return;
	}

	/*SetPriority*/
	pTaskPriority->SetPriority(EDROOMprioNormal);

	if (EDROOMprioNormal != pTaskPriority->GetTaskPrio()
			&& EDROOMprioNormal != pTaskPriority->GetTmpTaskPrio())
	{
		printf("ERROR: pTaskPriority->SetPriority(EDROOMprioNormal)\n");
		testPriorityOk = false;
		return;
	}
	pTaskPriority->SetPriority(EDROOMprioNormal);

	pTaskPriority->SetMaxPrioTmp(EDROOMprioVeryHigh);

	pTaskPriority->RestorePrioMsg();
	pTaskPriority->RestorePrioMsg();

	if (EDROOMprioNormal != pTaskPriority->GetTaskPrio()
			&& EDROOMprioNormal != pTaskPriority->GetTmpTaskPrio())
	{
		printf("ERROR: pTaskPriority->SetPriority(EDROOMprioNormal)\n");
		testPriorityOk = false;
		return;
	}

}

CUT_EDROOMBP_PR_TASK_CHANGE_PRIORITIES_OPERATIONS::CUT_EDROOMBP_PR_TASK_CHANGE_PRIORITIES_OPERATIONS(const char * id) :
		CUTBase(id, true, true, 9873829),
		kernel(),
		taskPriority(TASK_PRIORITY, EDROOMprioNormal, 512),
		pTestOk(&testPriorityOk)
{
	testPriorityOk = true;
	pTaskPriority = &taskPriority;

}


bool_t CUT_EDROOMBP_PR_TASK_CHANGE_PRIORITIES_OPERATIONS::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));
	return (*pTestOk);

}



/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_03_102 ------------------------------*/
/*----------------------------------------------------------------------------*/


Pr_Task *pTaskASenderReceiver;
Pr_Task *pTaskCReceiverSender;
Pr_Task *pTaskBSenderReceiver;
Pr_Task *pTaskDReceiverSender;

bool_t testSendAndReceiveOk = false;

uint8_t orderControlSendReceive=0;

static Pr_TaskRV_t TASK_A_SEND_AND_RECEIVE(Pr_TaskP_t)
{
	uint8_t dataToSend = 0xAC;
	uint8_t dataToReceive = 0;

	Pr_Send(*pTaskCReceiverSender,(void*)&dataToSend);


	Pr_Receive(&dataToReceive, sizeof(uint8_t));

	if (0xDA != dataToReceive || orderControlSendReceive!=2)
	{
		printf("TASK_A_SEND_AND_RECEIVE data 0x%X order %d\n",dataToReceive,orderControlSendReceive);
		return;
	}
	else
	{
		orderControlSendReceive++;
	}

}

static Pr_TaskRV_t TASK_B_SEND_AND_RECEIVE(Pr_TaskP_t)
{
	uint8_t dataToSend = 0xBD;
	uint8_t dataToReceive = 0;

	Pr_Send(*pTaskDReceiverSender,(void*)&dataToSend);

	Pr_Receive(&dataToReceive, sizeof(uint8_t));
	if (0xCB != dataToReceive || orderControlSendReceive!=3)
	{
		printf("TASK_B_SEND_AND_RECEIVE data 0x%X order %d\n",dataToReceive,orderControlSendReceive);
		return;
	}
	else
	{
		testSendAndReceiveOk=true;
	}

}


static Pr_TaskRV_t TASK_C_RECEIVE_AND_SEND(Pr_TaskP_t)
{
	uint8_t dataToSend = 0xCB;
	uint8_t dataToReceive = 0;

	Pr_Receive(&dataToReceive, sizeof(uint8_t));

	if (0xAC != dataToReceive || orderControlSendReceive!=0)
	{
		printf("TASK_C_RECEIVE_AND_SEND data 0x%X order %d\n",dataToReceive,orderControlSendReceive);
		return;
	}
	else
	{
		orderControlSendReceive++;
	}
	Pr_DelayIn(Pr_Time(1,0));
	Pr_Send(*pTaskBSenderReceiver,(void*)&dataToSend);

}

static Pr_TaskRV_t TASK_D_RECEIVE_AND_SEND(Pr_TaskP_t)
{
	uint8_t dataToSend = 0xDA;
	uint8_t dataToReceive = 0;

	Pr_Receive(&dataToReceive, sizeof(uint8_t));

	if (0xBD != dataToReceive || orderControlSendReceive!=1)
	{
		printf("TASK_D_RECEIVE_AND_SEND data 0x%X order %d\n",dataToReceive,orderControlSendReceive);
		return;
	}
	else
	{
		orderControlSendReceive++;
	}

	Pr_Send(*pTaskASenderReceiver,(void*)&dataToSend);


}

CUT_EDROOMBP_PR_TASK_SEND_AND_RECEIVE::CUT_EDROOMBP_PR_TASK_SEND_AND_RECEIVE(const char * id) :
				CUTBase(id, true, true, 9873829),
				kernel(),
		taskASenderReceiver(TASK_A_SEND_AND_RECEIVE, EDROOMprioVeryHigh, 2048),
		taskBSenderReceiver(TASK_B_SEND_AND_RECEIVE, EDROOMprioHigh, 2048),
		taskCReceiverSender(TASK_C_RECEIVE_AND_SEND, EDROOMprioNormal, 2048),
		taskDReceiverSender(TASK_D_RECEIVE_AND_SEND, EDROOMprioLow, 2048),
		pTaskSendAndReceiveOk(&testSendAndReceiveOk)

{
	pTaskASenderReceiver = &taskASenderReceiver;
	pTaskCReceiverSender = &taskCReceiverSender;
	pTaskBSenderReceiver = &taskBSenderReceiver;
	pTaskDReceiverSender = &taskDReceiverSender;

	testSendAndReceiveOk = false;

}



bool_t CUT_EDROOMBP_PR_TASK_SEND_AND_RECEIVE::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));
	return (*pTaskSendAndReceiveOk);

}



Pr_Task *pTaskAReceiverSender;
Pr_Task *pTaskCSenderReceiver;
Pr_Task *pTaskBReceiverSender;
Pr_Task *pTaskDSenderReceiver;

bool_t testReceiveAndSendOk = false;

uint8_t orderControlReceiveSend=0;

static Pr_TaskRV_t TASK_A_RECEIVE_AND_SEND(Pr_TaskP_t)
{
	uint8_t dataToSend = 0xAD;
	uint8_t dataToReceive = 0;


	Pr_Receive(&dataToReceive, sizeof(uint8_t));

	if (0xCA != dataToReceive || orderControlReceiveSend!=0)
	{
		printf("TASK_A_RECEIVE_AND_SEND data 0x%X order %d\n",dataToReceive,orderControlReceiveSend);
		return;
	}
	else
	{
		orderControlReceiveSend++;
	}

	Pr_Send(*pTaskDSenderReceiver,(void*)&dataToSend);



}

static Pr_TaskRV_t TASK_B_RECEIVE_AND_SEND(Pr_TaskP_t)
{
	uint8_t dataToSend = 0xBC;
	uint8_t dataToReceive = 0;



	Pr_Receive(&dataToReceive, sizeof(uint8_t));

	if (0xDB != dataToReceive || orderControlReceiveSend!=1)
	{
		printf("TASK_B_RECEIVE_AND_SEND data 0x%X order %d\n",dataToReceive,orderControlReceiveSend);
		return;
	}
	else
	{
		orderControlReceiveSend++;
	}

	Pr_Send(*pTaskCSenderReceiver,(void*)&dataToSend);

}


static Pr_TaskRV_t TASK_C_SEND_AND_RECEIVE(Pr_TaskP_t)
{
	uint8_t dataToSend = 0xCA;
	uint8_t dataToReceive = 0;

	Pr_Send(*pTaskAReceiverSender,(void*)&dataToSend);

	Pr_Receive(&dataToReceive, sizeof(uint8_t));

	if (0xBC != dataToReceive || orderControlReceiveSend!=2)
	{
		printf("TASK_C_SEND_AND_RECEIVE data 0x%X order %d\n",dataToReceive,orderControlReceiveSend);
		return;
	}
	else
	{
		orderControlReceiveSend++;
	}




}

static Pr_TaskRV_t TASK_D_SEND_AND_RECEIVE(Pr_TaskP_t)
{
	uint8_t dataToSend = 0xDB;
	uint8_t dataToReceive = 0;

	Pr_Send(*pTaskBReceiverSender,(void*)&dataToSend);

	Pr_Receive(&dataToReceive, sizeof(uint8_t));

	if (0xAD != dataToReceive || orderControlReceiveSend!=3)
	{
		printf("TASK_D_SEND_AND_RECEIVE data 0x%X order %d\n",dataToReceive,orderControlReceiveSend);
		return;
	}
	else
	{
		testReceiveAndSendOk=true;
	}




}

CUT_EDROOMBP_PR_TASK_RECEIVE_AND_SEND::CUT_EDROOMBP_PR_TASK_RECEIVE_AND_SEND(const char * id) :
				CUTBase(id, true, true, 9873829),
				kernel(),
		taskAReceiverSender(TASK_A_RECEIVE_AND_SEND, EDROOMprioVeryHigh, 2048),
		taskBReceiverSender(TASK_B_RECEIVE_AND_SEND, EDROOMprioHigh, 2048),
		taskCSenderReceiver(TASK_C_SEND_AND_RECEIVE, EDROOMprioNormal, 2048),
		taskDSenderReceiver(TASK_D_SEND_AND_RECEIVE, EDROOMprioLow, 2048),
		pTaskSendAndReceiveOk(&testReceiveAndSendOk)

{
	pTaskAReceiverSender = &taskAReceiverSender;
	pTaskCSenderReceiver = &taskCSenderReceiver;
	pTaskBReceiverSender = &taskBReceiverSender;
	pTaskDSenderReceiver = &taskDSenderReceiver;

	testReceiveAndSendOk = false;

}



bool_t CUT_EDROOMBP_PR_TASK_RECEIVE_AND_SEND::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));
	return (*pTaskSendAndReceiveOk);

}



/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_03_101 ------------------------------*/
/*----------------------------------------------------------------------------*/


Pr_Task * pTaskQueuePrioFifoA;
Pr_Task * pTaskQueuePrioFifoB;
Pr_Task * pTaskQueuePrioFifoC;
Pr_Task * pTaskQueuePrioFifoD;

bool_t taskQueuePrioFifoBOk=false;
bool_t taskQueuePrioFifoCOk=false;
bool_t taskQueuePrioFifoDOk=false;

Pr_SemaphoreBin * pSemQueuePrioFifo;

uint32_t orderControlFifo = 0;

static Pr_TaskRV_t TASK_A_PRIORITY_FIFO(Pr_TaskP_t)
{
		/*obtain a semaphore*/
	pSemQueuePrioFifo->Wait();
	Pr_DelayIn(Pr_Time(1,0));

	if (0 != orderControlFifo)
	{
		DEBUG("0!=orderControl");
		return;
	}
	orderControlFifo++;

	pTaskQueuePrioFifoD->SetPriority(EDROOMprioVeryHigh); //2
	pTaskQueuePrioFifoC->SetPriority(EDROOMprioNormal); //4
	pTaskQueuePrioFifoB->SetPriority(EDROOMprioVeryLow); //6
	pSemQueuePrioFifo->Signal();
}

static Pr_TaskRV_t TASK_B_PRIORITY_FIFO(Pr_TaskP_t)
{
	/*obtain a semaphore*/
	pSemQueuePrioFifo->Wait();

	if (1 != orderControlFifo)
	{
		DEBUG("1!=orderControl %d",orderControlFifo );
		return;
	}
	orderControlFifo++;

	pSemQueuePrioFifo->Signal();
	taskQueuePrioFifoBOk = true;
}

static Pr_TaskRV_t TASK_C_PRIORITY_FIFO(Pr_TaskP_t)
{

	pSemQueuePrioFifo->Wait();

	if (2 != orderControlFifo)
	{
		DEBUG("2!=orderControl");
		return;
	}
	orderControlFifo++;

	pSemQueuePrioFifo->Signal();
	taskQueuePrioFifoCOk = true;

}

static Pr_TaskRV_t TASK_D_PRIORITY_FIFO(Pr_TaskP_t)
{
	pSemQueuePrioFifo->Wait();

	if (3 != orderControlFifo)
	{
		DEBUG("3!=orderControl");
		return;
	}
	orderControlFifo++;

	pSemQueuePrioFifo->Signal();
	taskQueuePrioFifoDOk = true;
}

CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED_FIFO::CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED_FIFO(const char * id) :
				CUTBase(id, true, true, 9873829),
				kernel(),
				taskA(TASK_A_PRIORITY_FIFO, EDROOMprioURGENT, 2048), //1
				taskB(TASK_B_PRIORITY_FIFO, EDROOMprioHigh, 2048), //3
				taskC(TASK_C_PRIORITY_FIFO, EDROOMprioLow, 2048), //5
				taskD(TASK_D_PRIORITY_FIFO, EDROOMprioIDLE, 2048), //7
				pTaskBOk(&taskQueuePrioFifoBOk),
				pTaskCOk(&taskQueuePrioFifoCOk),
				pTaskDOk(&taskQueuePrioFifoDOk),
				semQueuePrio(1)
{
	taskQueuePrioFifoBOk = false;
	taskQueuePrioFifoCOk = false;
	taskQueuePrioFifoDOk = false;
	orderControlFifo = 0;
	pSemQueuePrioFifo = &semQueuePrio;
	pTaskQueuePrioFifoA = &taskA;
	pTaskQueuePrioFifoB = &taskB;
	pTaskQueuePrioFifoC = &taskC;
	pTaskQueuePrioFifoD = &taskD;

}



bool_t CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED_FIFO::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));

	return ((*pTaskBOk) && (*pTaskCOk) && (*pTaskDOk));

}


/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_03_103 ------------------------------*/
/*----------------------------------------------------------------------------*/

Pr_Task * pTaskQueuePrioCeilingA;
Pr_Task * pTaskQueuePrioCeilingB;
Pr_Task * pTaskQueuePrioCeilingC;
Pr_Task * pTaskQueuePrioCeilingD;

bool_t taskQueuePrioCeilingBOk=false;
bool_t taskQueuePrioCeilingCOk=false;
bool_t taskQueuePrioCeilingDOk=false;

Pr_SemaphoreRec * pSemQueuePrioCeiling;

uint32_t orderControlCeiling = 0;

static Pr_TaskRV_t TASK_A_PRIORITY_CEILING(Pr_TaskP_t)
{
		/*obtain a semaphore*/
	pSemQueuePrioCeiling->Wait();

	printf("LA tarea A obtiene el semáforo\n");
	printf("Prioridad A %i\n",pTaskQueuePrioCeilingA->GetTaskPrio());


	Pr_DelayIn(Pr_Time(1,0));


	if (0 != orderControlCeiling)
	{
		DEBUG("0!=orderControl");
		return;
	}
	orderControlCeiling++;

	pTaskQueuePrioCeilingD->SetPriority(EDROOMprioVeryHigh); //2
	printf("nueva prioridad D: %i \n", pTaskQueuePrioCeilingD->GetTaskPrio());
	pTaskQueuePrioCeilingC->SetPriority(EDROOMprioNormal); //4
	printf("nueva prioridad C: %i \n", pTaskQueuePrioCeilingC->GetTaskPrio());
	pTaskQueuePrioCeilingB->SetPriority(EDROOMprioVeryLow); //6
	printf("nueva prioridad B: %i \n", pTaskQueuePrioCeilingB->GetTaskPrio());

	pSemQueuePrioCeiling->Signal();
	printf("LA tarea A suelta el semáforo\n");
}

static Pr_TaskRV_t TASK_B_PRIORITY_CEILING(Pr_TaskP_t)
{
	/*obtain a semaphore*/
	pSemQueuePrioCeiling->Wait();

	printf("LA tarea B obtiene el semáforo\n");
	printf("Prioridad B %i\n",pTaskQueuePrioCeilingB->GetTaskPrio());


	if (3 != orderControlCeiling)
	{
		DEBUG("3!=orderControl %d",orderControlCeiling );
		return;
	}
	orderControlCeiling++;

	printf("AQUI\n");
	pSemQueuePrioCeiling->Signal();
	taskQueuePrioCeilingBOk = true;
	printf("LA tarea B suelta el semáforo\n");
}

static Pr_TaskRV_t TASK_C_PRIORITY_CEILING(Pr_TaskP_t)
{

	pSemQueuePrioCeiling->Wait();

	printf("LA tarea C obtiene el semáforo\n");
	printf("Prioridad C %i\n",pTaskQueuePrioCeilingC->GetTaskPrio());


	if (2 != orderControlCeiling)
	{
		DEBUG("2!=orderControl");
		return;
	}
	orderControlCeiling++;

	pSemQueuePrioCeiling->Signal();
	taskQueuePrioCeilingCOk = true;
	printf("LA tarea C suelta el semáforo\n");

}

static Pr_TaskRV_t TASK_D_PRIORITY_CEILING(Pr_TaskP_t)
{

	pSemQueuePrioCeiling->Wait();

	printf("LA tarea D obtiene el semáforo\n");
	printf("Prioridad D %i\n",pTaskQueuePrioCeilingD->GetTaskPrio());
	

	if (1 != orderControlCeiling)
	{
		DEBUG("1!=orderControl");
		return;
	}
	orderControlCeiling++;

	pSemQueuePrioCeiling->Signal();
	taskQueuePrioCeilingDOk = true;
	printf("LA tarea D suelta el semáforo\n");
}

CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED_CEILING::CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED_CEILING(const char * id) :
				CUTBase(id, true, true, 9873829),
				kernel(),
				taskA(TASK_A_PRIORITY_CEILING, EDROOMprioURGENT, 2048), //1
				taskB(TASK_B_PRIORITY_CEILING, EDROOMprioHigh, 2048), //3
				taskC(TASK_C_PRIORITY_CEILING, EDROOMprioLow, 2048), //5
				taskD(TASK_D_PRIORITY_CEILING, EDROOMprioIDLE, 2048), //7
				pTaskBOk(&taskQueuePrioCeilingBOk),
				pTaskCOk(&taskQueuePrioCeilingCOk),
				pTaskDOk(&taskQueuePrioCeilingDOk),
				semQueuePrio(1)
{
	taskQueuePrioCeilingBOk = false;
	taskQueuePrioCeilingCOk = false;
	taskQueuePrioCeilingDOk = false;
	orderControlCeiling = 0;
	pSemQueuePrioCeiling = &semQueuePrio;
	pTaskQueuePrioCeilingA = &taskA;
	pTaskQueuePrioCeilingB = &taskB;
	pTaskQueuePrioCeilingC = &taskC;
	pTaskQueuePrioCeilingD = &taskD;

}





bool_t CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED_CEILING::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));
	return ((*pTaskBOk) && (*pTaskCOk) && (*pTaskDOk));

}


Pr_Task * pTaskQueuePrioInheritA;
Pr_Task * pTaskQueuePrioInheritB;
Pr_Task * pTaskQueuePrioInheritC;
Pr_Task * pTaskQueuePrioInheritD;

bool_t taskQueuePrioInheritBOk=false;
bool_t taskQueuePrioInheritCOk=false;
bool_t taskQueuePrioInheritDOk=false;

Pr_SemaphoreRec * pSemQueuePrioInherit;

uint32_t orderControlInherit = 0;

static Pr_TaskRV_t TASK_A_PRIORITY_INHERIT(Pr_TaskP_t)
{
	/*obtain a semaphore*/
	pSemQueuePrioInherit->Wait();

	if (0 != orderControlInherit)
	{
		printf("0!=orderControl\n");
		return;
	}
	orderControlInherit++;

	pTaskQueuePrioInheritD->SetPriority(EDROOMprioVeryHigh); //2
	pTaskQueuePrioInheritC->SetPriority(EDROOMprioNormal); //4
	pTaskQueuePrioInheritB->SetPriority(EDROOMprioVeryLow); //6

	// printf("contador A antes signal %i\n", pSemQueuePrioInherit->countSem());
	pSemQueuePrioInherit->Signal();
	// printf("contador A despues signal %i\n", pSemQueuePrioInherit->countSem());
}

static Pr_TaskRV_t TASK_B_PRIORITY_INHERIT(Pr_TaskP_t)
{
	/*obtain a semaphore*/
	pSemQueuePrioInherit->Wait();

	if (3 != orderControlInherit)
	{
		printf("3!=orderControl %d\n",orderControlInherit );
		return;
	}
	orderControlInherit++;

	pSemQueuePrioInherit->Signal();
	taskQueuePrioInheritBOk = true;
}

static Pr_TaskRV_t TASK_C_PRIORITY_INHERIT(Pr_TaskP_t)
{
	pSemQueuePrioInherit->Wait();

	if (2 != orderControlInherit)
	{
		printf("2!=orderControl\n");
		return;
	}
	orderControlInherit++;

	pSemQueuePrioInherit->Signal();
	taskQueuePrioInheritCOk = true;

}

static Pr_TaskRV_t TASK_D_PRIORITY_INHERIT(Pr_TaskP_t)
{
	pSemQueuePrioInherit->Wait();

	if (1 != orderControlInherit)
	{
		printf("1!=orderControl\n");
		return;
	}
	orderControlInherit++;
	pSemQueuePrioInherit->Signal();
	taskQueuePrioInheritDOk = true;
}

CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED_INHERIT::CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED_INHERIT(const char * id) :
				CUTBase(id, true, true, 9873829),
				kernel(),
				taskA(TASK_A_PRIORITY_INHERIT, EDROOMprioURGENT, 2048), //1
				taskB(TASK_B_PRIORITY_INHERIT, EDROOMprioHigh, 2048), //3
				taskC(TASK_C_PRIORITY_INHERIT, EDROOMprioLow, 2048), //5
				taskD(TASK_D_PRIORITY_INHERIT, EDROOMprioIDLE, 2048), //7
				pTaskBOk(&taskQueuePrioInheritBOk),
				pTaskCOk(&taskQueuePrioInheritCOk),
				pTaskDOk(&taskQueuePrioInheritDOk),
				semQueuePrio()
{
	taskQueuePrioInheritBOk = false;
	taskQueuePrioInheritCOk = false;
	taskQueuePrioInheritDOk = false;
	orderControlInherit = 0;
	pSemQueuePrioInherit = &semQueuePrio;
	pTaskQueuePrioInheritA = &taskA;
	pTaskQueuePrioInheritB = &taskB;
	pTaskQueuePrioInheritC = &taskC;
	pTaskQueuePrioInheritD = &taskD;

}



bool_t CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED_INHERIT::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));

	return ((*pTaskBOk) && (*pTaskCOk) && (*pTaskDOk));

}



/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_03_103 ------------------------------*/
/*----------------------------------------------------------------------------*/

bool_t testCeilingOk=false;

Pr_Task *pTaskSelf;

static Pr_TaskRV_t TASK_PRIORITY_WAIT_CEILING(Pr_TaskP_t)
{
	Pr_SemaphoreRec ceiling(3);
	
	ceiling.Wait();

	pTaskSelf->SetPriority(EDROOMprioLow);

	if (pTaskSelf->GetTmpTaskPrio()!=EDROOMprioHigh)
	{
		printf("1 pTaskSelf->GetTaskPrio() %d\n",pTaskSelf->GetTaskPrio());
		return;
	}


	ceiling.Signal();


	if (pTaskSelf->GetTmpTaskPrio()!=EDROOMprioLow)
	{
		printf("2 pTaskSelf->GetTaskPrio() %d\n",pTaskSelf->GetTaskPrio());
		return;
	}


	testCeilingOk=true;
}



CUT_EDROOMBP_PR_TASK_TASK_PRIORITY_WAIT_CEILING::CUT_EDROOMBP_PR_TASK_TASK_PRIORITY_WAIT_CEILING(const char * id) :
				CUTBase(id, true, true, 9873829),
				kernel(),
				task(TASK_PRIORITY_WAIT_CEILING, EDROOMprioHigh, 2048), //1 en realidad es un 3
				pTestOk(&testCeilingOk)
{
	pTaskSelf=&task;
}



bool_t CUT_EDROOMBP_PR_TASK_TASK_PRIORITY_WAIT_CEILING::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));

	return ((*pTestOk));

}




//
//
///*
// * TODO: Este test no pasa porque una vez encolado no le afecta el cambio
// * de prioridad y coge el semaforo independientemente de la prioridad actual que
// * tenga en el momento de bloquearlo en lugar de irse a la cola correspondiente:
// * ES DEBIDO A LOS ATRIBUTOS POR DEFECTO DE LA CREACIÓN DEL SEMAFORO. Tal y como
// * está definida la edroombp no sería posible este comportamiento
// */
//
//Pr_SemaphoreBin * pSemRequeue;
//
//uint32_t orderControlRequeue = 0;
//
//bool_t testTaskRequeueOk=true;
//
//static Pr_TaskRV_t TASK_A_REQUEUE(Pr_TaskP_t)
//{
//	DEBUG("TASK_A_REQUEUE");
//	if (testTaskRequeueOk && 0!=orderControlRequeue)
//	{
//		DEBUG("FALSE TASK_A_REQUEUE 1");
//		testTaskRequeueOk=false;
//		return;
//	}
//	orderControlRequeue++;
//
//	if (pSemRequeue->WaitTimed(Pr_Time(5,0)))
//	{
//		DEBUG("TASK_A_REQUEUE LOCKED SEM LOCK");
//	}
//	else
//	{
//		DEBUG("TASK_A_REQUEUE LOCKED SEM NO LOCK");
//	}
//
//	if (testTaskRequeueOk && 10!=orderControlRequeue)
//	{
//		DEBUG("FALSE TASK_A_REQUEUE 2");
//		testTaskRequeueOk=false;
//		return;
//	}
//
//	orderControlRequeue++;
//	DEBUG("END TASK_A_REQUEUE");
//}
//
//static Pr_TaskRV_t TASK_B_REQUEUE(Pr_TaskP_t)
//{
//	DEBUG("TASK_B_REQUEUE");
//	if (testTaskRequeueOk && 1!=orderControlRequeue)
//	{
//		DEBUG("FALSE TASK_B_REQUEUE 1");
//		testTaskRequeueOk=false;
//		return;
//	}
//	orderControlRequeue++;
//	pSemRequeue->WaitTimed(Pr_Time(5,0));
//	DEBUG("TASK_B_REQUEUE LOCKED SEM");
//	if (testTaskRequeueOk && 9!=orderControlRequeue)
//	{
//		DEBUG("FALSE TASK_B_REQUEUE 2");
//		testTaskRequeueOk=false;
//		return;
//	}
//	orderControlRequeue++;
//	DEBUG("END TASK_B_REQUEUE");
//
//}
//
//static Pr_TaskRV_t TASK_C_REQUEUE(Pr_TaskP_t)
//{
//	DEBUG("TASK_C_REQUEUE");
//	if (testTaskRequeueOk && 2!=orderControlRequeue)
//	{
//		DEBUG("FALSE TASK_C_REQUEUE 1");
//		testTaskRequeueOk=false;
//		return;
//	}
//	orderControlRequeue++;
//	pSemRequeue->WaitTimed(Pr_Time(5,0));
//	DEBUG("TASK_C_REQUEUE LOCKED SEM");
//	if (testTaskRequeueOk && 8!=orderControlRequeue)
//	{
//		DEBUG("FALSE TASK_C_REQUEUE 2");
//		testTaskRequeueOk=false;
//		return;
//	}
//	orderControlRequeue++;
//	DEBUG("END TASK_C_REQUEUE");
//
//}
//
//static Pr_TaskRV_t TASK_D_REQUEUE(Pr_TaskP_t)
//{
//	DEBUG("TASK_D_REQUEUE");
//	if (testTaskRequeueOk && 3!=orderControlRequeue)
//	{
//		DEBUG("FALSE TASK_D_REQUEUE 1");
//		testTaskRequeueOk=false;
//		return;
//	}
//	orderControlRequeue++;
//	pSemRequeue->WaitTimed(Pr_Time(5,0));
//	DEBUG("TASK_D_REQUEUE LOCKED SEM");
//	if (testTaskRequeueOk && 7!=orderControlRequeue)
//	{
//		DEBUG("FALSE TASK_D_REQUEUE 2");
//		testTaskRequeueOk=false;
//		return;
//	}
//	orderControlRequeue++;
//	DEBUG("END TASK_D_REQUEUE");
//
//}
//
//static Pr_TaskRV_t TASK_E_REQUEUE(Pr_TaskP_t)
//{
//	DEBUG("TASK_E_REQUEUE");
//	if (testTaskRequeueOk && 4!=orderControlRequeue)
//	{
//		DEBUG("FALSE TASK_E_REQUEUE 1");
//		testTaskRequeueOk=false;
//		return;
//	}
//	orderControlRequeue++;
//	pSemRequeue->WaitTimed(Pr_Time(5,0));
//	DEBUG("TASK_E_REQUEUE LOCKED SEM");
//	if (testTaskRequeueOk && 6!=orderControlRequeue)
//	{
//		DEBUG("FALSE TASK_E_REQUEUE 2");
//		testTaskRequeueOk=false;
//		return;
//	}
//	orderControlRequeue++;
//	DEBUG("END TASK_E_REQUEUE");
//
//}
//
//static Pr_TaskRV_t TASK_CONTROL_REQUEUE(Pr_TaskP_t)
//{
//	DEBUG("TASK_CONTROL_REQUEUE");
//	Pr_Task taskA(TASK_A_REQUEUE, EDROOMprioURGENT, 2048);
//	Pr_Task taskB(TASK_B_REQUEUE, EDROOMprioVeryHigh, 2048);
//	Pr_Task taskC(TASK_C_REQUEUE, EDROOMprioHigh, 2048);
//	Pr_Task taskD(TASK_D_REQUEUE, EDROOMprioNormal, 2048);
//	Pr_Task taskE(TASK_E_REQUEUE, EDROOMprioLow, 2048);
//	if (testTaskRequeueOk && 5!=orderControlRequeue)
//	{
//		DEBUG("FALSE TASK_CONTROL_REQUEUE 1");
//		testTaskRequeueOk=false;
//		return;
//	}
//	orderControlRequeue++;
//	taskA.SetPriority(EDROOMprioLow);
//	taskB.SetPriority(EDROOMprioNormal);
//	taskC.SetPriority(EDROOMprioHigh);
//	taskD.SetPriority(EDROOMprioVeryHigh);
//	taskE.SetPriority(EDROOMprioURGENT);
//	DEBUG("taskA.GetTaskPrio() %d\n",taskA.GetTaskPrio());
//	DEBUG("taskB.GetTaskPrio() %d\n",taskB.GetTaskPrio());
//	DEBUG("taskC.GetTaskPrio() %d\n",taskC.GetTaskPrio());
//	DEBUG("taskD.GetTaskPrio() %d\n",taskD.GetTaskPrio());
//	DEBUG("taskE.GetTaskPrio() %d\n",taskE.GetTaskPrio());
//
//	pSemRequeue->Signal();
//	if (testTaskRequeueOk && 11!=orderControlRequeue)
//	{
//		DEBUG("FALSE TASK_CONTROL_REQUEUE 2");
//		testTaskRequeueOk=false;
//		return;
//	}
//	DEBUG("END TASK_CONTROL_REQUEUE");
//
//}
//
//
//CUT_EDROOMBP_PR_TASK_TASK_REQUEUE::CUT_EDROOMBP_PR_TASK_TASK_REQUEUE(const char * id) :
//								CUTBase(id, true, true, 9873829),
//								kernel(),
//								taskControl(TASK_CONTROL_REQUEUE, EDROOMprioVeryLow, 2048),
//								semBin(0),
//								pTaskRequeueOk(&testTaskRequeueOk)
//{
//	pSemRequeue = &semBin;
//	testSendAndReceiveOk = true;
//
//}
//
//
//bool_t CUT_EDROOMBP_PR_TASK_TASK_REQUEUE::DoTest(void)
//{
//	Pr_DelayIn(Pr_Time(10,0));
//	return (*pTaskRequeueOk);
//
//}



/*----------------------------------------------------------------------------*/
/*--------------------------------Event Manager-------------------------------*/
/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_08_010 ------------------------------*/
/*----------------------------------------------------------------------------*/
//
//
//static Pr_TaskRV_t TASK_PRIORITY(Pr_TaskP_t)
//{
//
//
//}
//
//CUT_EDROOMBP_PR_TASK_EVENT_RECEIVE::CUT_EDROOMBP_PR_TASK_EVENT_RECEIVE(const char * id) :
//		CUTBase(id, true, true, 9873829),
//		kernel(),
//		taskPriority(TASK_PRIORITY, EDROOMprioLow, 512),
//		pTestOk(&testPriorityOk)
//{
//	testPriorityOk = true;
//	pTaskPriority = &taskPriority;
//
//}
//
//
//bool_t CUT_EDROOMBP_PR_TASK_EVENT_RECEIVE::DoTest(void)
//{
//	Pr_DelayIn(Pr_Time(10,0));
//	return (*pTestOk);
//
//}
//
//
//
//
//
//
//
//
//






