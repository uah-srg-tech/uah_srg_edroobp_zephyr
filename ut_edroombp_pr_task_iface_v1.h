/**
 * \file	ut_edroombp_pr_task_iface_v1.h
 *
 * \brief	This swinterface declares the test suites applied over the edroombp
 * 			Task Manager (Pr_Task) services for the RTEMSAPI 4.8 improvement
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>

 * \internal
 * Created: 	06/11/2013
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

#ifndef PUBLIC__UT_EDROOMBP_PR_TASK_IFACE_V1_H 
#define PUBLIC__UT_EDROOMBP_PR_TASK_IFACE_V1_H

#include "config.h"
#include "basic_types.h"

#include "ut_base_iface_v1.h"
#include "edroombp.h"

/*PROTECTED REGION ID(public_ut_edroombp_pr_task_iface_h) ENABLED START*/


/**
 * \brief	The test is responsible to check the correct functionality of the
 * 			creation and start services, i.e., the actions made by the Pr_Task
 * 			constructor. The test creates 3 tasks with different parameters and
 *			checks if the expected operations are executed:
 *			1. The first task has correct parameters and must be executed
 *			correctly.
 * 			2. The second task sets a NULL function parameter, this task must
 * 			be returning an error
 *			code.
 * 			3. The last task sets a stack size larger than the maximum allowed
 * 			and must be returning an error code.
 *
 */
class CUT_EDROOMBP_PR_TASK_CREATION_AND_START: public CUTBase
{
	friend class CUTManager;
public:
	Pr_Kernel kernel;

	/* tasks for the tests */
	/**
	 * \brief	This task must be created successfully
	 */
	Pr_Task taskSuccess;
	/**
	 * \brief	This task must not be created due to a stack size error
	 */
	Pr_Task taskStackSizeError;
	/**
	 * \brief	This task must not be executed due to its code is null
	 */
	Pr_Task taskCodeError;

	/* this pointers are used to get the values used by the task after to be
	 * executed*/
	bool_t *pTaskSuccessOk;
	bool_t *pTaskStackSizeErrorOk;/* request not satisfied message error is
		 equivalent to stack size error*/


	CUT_EDROOMBP_PR_TASK_CREATION_AND_START(const char * id);


	bool_t DoTest(void);

};


/**
 * \brief	The test creates a task and after the execution of this task, the
 * 			test tries to restart it and checks that was deleted.
 *
 */
class CUT_EDROOMBP_PR_TASK_DELETE_TASK: public CUTBase
{
	friend class CUTManager;
public:
	Pr_Kernel kernel;

	bool_t testOk;

	CUT_EDROOMBP_PR_TASK_DELETE_TASK(const char * id);

	bool_t DoTest(void);

};



/**
 * \brief	The test creates a task which changes its priorities, dynamic and
 * 			base, and checks that they were modified correctly
 *
 */
class CUT_EDROOMBP_PR_TASK_CHANGE_PRIORITIES_OPERATIONS: public CUTBase
{
	friend class CUTManager;
public:
	Pr_Kernel kernel;


	CUT_EDROOMBP_PR_TASK_CHANGE_PRIORITIES_OPERATIONS(const char * id);

	bool_t DoTest(void);
	/* tasks for the tests */
	/**
	 * \brief	This task must be created successfully
	 */
	Pr_Task taskPriority;

	/* this pointer is used to get the values used by the task after to be
	 * executed*/
	bool_t *pTestOk;

};



/**
 * \brief	This test creates 4 tasks, one of those tasks inverts the priority
 * 			of the rest while they are waiting to a shared resource, and the
 * 			modification of priorities makes that after to release the shared
 * 			resource, the tasks will be executed in the opposite order
 *
 */
class CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED: public CUTBase
{
	friend class CUTManager;
public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED(const char * id);

	bool_t DoTest(void);

	Pr_Task taskA;
	Pr_Task taskB;
	Pr_Task taskC;
	Pr_Task taskD;

	bool_t * pTaskBOk;
	bool_t * pTaskCOk;
	bool_t * pTaskDOk;

	Pr_SemaphoreRec semQueuePrio;


};


/**
 * \brief	This test creates a pair of tasks that send and receive data
 * 			between them
 */
class CUT_EDROOMBP_PR_TASK_SEND_AND_RECEIVE: public CUTBase
{
	friend class CUTManager;
public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_TASK_SEND_AND_RECEIVE(const char * id);

	bool_t DoTest(void);


	
	Pr_Task taskCReceiverSender;
	Pr_Task taskASenderReceiver;
	Pr_Task taskDReceiverSender;
	Pr_Task taskBSenderReceiver;
	bool_t *pTaskSendAndReceiveOk;

};


/**
 * \brief	This test creates a pair of tasks that send and receive data
 * 			between them
 */
class CUT_EDROOMBP_PR_TASK_RECEIVE_AND_SEND: public CUTBase
{
	friend class CUTManager;
public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_TASK_RECEIVE_AND_SEND(const char * id);

	bool_t DoTest(void);

//	Pr_Task taskSenderReceiver;
//	Pr_Task taskReceiverSender;

	Pr_Task taskCSenderReceiver;
	Pr_Task taskAReceiverSender;
	Pr_Task taskDSenderReceiver;
	Pr_Task taskBReceiverSender;

	bool_t *pTaskSendAndReceiveOk;


};



class CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED_FIFO: public CUTBase
{
	friend class CUTManager;
public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED_FIFO(const char * id);

	bool_t DoTest(void);

	Pr_Task taskA;
	Pr_Task taskB;
	Pr_Task taskC;
	Pr_Task taskD;

	bool_t * pTaskBOk;
	bool_t * pTaskCOk;
	bool_t * pTaskDOk;

	Pr_SemaphoreBin semQueuePrio;


};


class CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED_CEILING: public CUTBase
{
	friend class CUTManager;
public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED_CEILING(const char * id);

	bool_t DoTest(void);

	Pr_Task taskA;
	Pr_Task taskB;
	Pr_Task taskC;
	Pr_Task taskD;

	bool_t * pTaskBOk;
	bool_t * pTaskCOk;
	bool_t * pTaskDOk;

	Pr_SemaphoreRec semQueuePrio;


};


class CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED_INHERIT: public CUTBase
{
	friend class CUTManager;
public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_TASK_CHANGE_TASK_PRIORITY_QUEUED_INHERIT(const char * id);

	bool_t DoTest(void);

	Pr_Task taskA;
	Pr_Task taskB;
	Pr_Task taskC;
	Pr_Task taskD;

	bool_t * pTaskBOk;
	bool_t * pTaskCOk;
	bool_t * pTaskDOk;

	Pr_SemaphoreRec semQueuePrio;


};





class CUT_EDROOMBP_PR_TASK_TASK_PRIORITY_WAIT_CEILING: public CUTBase
{
	friend class CUTManager;
public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_TASK_TASK_PRIORITY_WAIT_CEILING(const char * id);

	bool_t DoTest(void);

	Pr_Task task;

	bool_t * pTestOk;



};



///**
// * \brief	this test validates the functionality of the sending and
// * 			receiving mechanism implemented by the edroombp Task Manager.
// */
//class CUT_EDROOMBP_PR_TASK_TASK_REQUEUE: public CUTBase
//{
//	friend class CUTManager;
//public:
//	Pr_Kernel kernel;
//
//	CUT_EDROOMBP_PR_TASK_TASK_REQUEUE(const char * id);
//
//	bool_t DoTest(void);
//
//	Pr_Task taskControl;
//
//	Pr_SemaphoreBin semBin;
//
//	bool_t *pTaskRequeueOk;
//
//
//};


/**
 * \brief	The test creates a task which changes its priorities, dynamic and
 * 			base, and checks that they were modified correctly
 *
 */
class CUT_EDROOMBP_PR_TASK_EVENT_RECEIVE: public CUTBase
{
	friend class CUTManager;
public:
	Pr_Kernel kernel;


	CUT_EDROOMBP_PR_TASK_EVENT_RECEIVE(const char * id);

	bool_t DoTest(void);
	/* tasks for the tests */
	/**
	 * \brief	This task must be created successfully
	 */
	Pr_Task taskPriority;

	/* this pointer is used to get the values used by the task after to be
	 * executed*/
	bool_t *pTestOk;

};



/*PROTECTED REGION END*/

#endif // PUBLIC__UT_EDROOMBP_PR_TASK_IFACE_V1_H
