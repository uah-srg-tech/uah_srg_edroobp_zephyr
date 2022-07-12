/**
 * \file	ut_edroombp_pr_semaphore_iface_v1.h
 * \brief	This swinterface declares the test suites applied over the edroombp
 * 			IRQ Handler Manager (Pr_IRQ_Handler) and IRQ Event Manager
 * 			(Pr_IRQ_Event) services for the RTEMSAPI 4.8 improvement
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>

 * \internal
 * Created: 	09/09/2014
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

#ifndef PUBLIC__UT_EDROOMBP_PR_IRQ_IFACE_V1_H
#define PUBLIC__UT_EDROOMBP_PR_IRQ_IFACE_V1_H

#include "config.h"
#include "basic_types.h"

// Required software interfaces
#include "ut_base_iface_v1.h"
#include "edroombp.h"

/*PROTECTED REGION ID(public_ut_edroombp_pr_irq_iface_h) ENABLED START*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*---------------------------- Pr_IRQManager Tests ---------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/**
 * \brief
 *
 */
class CUT_EDROOMBP_PR_IRQ_MANAGER_INSTALL_HANDLER : public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;
	bool_t *pTestOk;

	CUT_EDROOMBP_PR_IRQ_MANAGER_INSTALL_HANDLER(const char * id);
	bool_t DoTest(void);




};

/**
 * \brief
 *
 */
class CUT_EDROOMBP_PR_IRQ_MANAGER_RELEASE_SEMAPHORE : public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;
	bool_t *pTestOk;

	Pr_SemaphoreBin semBin;
	Pr_SemaphoreRec semRecCeiling;
	Pr_SemaphoreRec semRecInherit;
	Pr_Task taskA;
	Pr_Task taskB;


	CUT_EDROOMBP_PR_IRQ_MANAGER_RELEASE_SEMAPHORE(const char * id);
	bool_t DoTest(void);




};


/**
 * \brief
 *
 */
class CUT_EDROOMBP_PR_IRQ_MANAGER_TIMER_OPERATIONS : public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;
	bool_t *pTestOk;

	Pr_Task task;


	CUT_EDROOMBP_PR_IRQ_MANAGER_TIMER_OPERATIONS(const char * id);
	bool_t DoTest(void);




};



/**
 * \brief
 *
 */
class CUT_EDROOMBP_PR_IRQ_MANAGER_ENABLE_DISABLE : public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;
	bool_t *pTestIRQOk;
	bool_t *pTestOk;
	Pr_Task task;


	CUT_EDROOMBP_PR_IRQ_MANAGER_ENABLE_DISABLE(const char * id);
	bool_t DoTest(void);

};








/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------- Pr_IRQEvent Tests ----------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/**
 * \brief
 *
 */
class CUT_EDROOMBP_PR_IRQ_EVENT_SEND_AND_RECEIVE_WAIT : public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;
	bool_t *pTestOk;

	Pr_IRQEvent irqEvent;
	Pr_Task taskA;
	Pr_Task taskB;


	CUT_EDROOMBP_PR_IRQ_EVENT_SEND_AND_RECEIVE_WAIT(const char * id);
	bool_t DoTest(void);

};


/**
 * \brief
 *
 */
class CUT_EDROOMBP_PR_IRQ_EVENT_SEND_AND_RECEIVE_WAIT_COND : public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;
	bool_t *pTestOk;

	Pr_IRQEvent irqEvent;
	Pr_Task taskA;
	Pr_Task taskB;


	CUT_EDROOMBP_PR_IRQ_EVENT_SEND_AND_RECEIVE_WAIT_COND(const char * id);
	bool_t DoTest(void);

};


/**
 * \brief
 *
 */
class CUT_EDROOMBP_PR_IRQ_EVENT_SEND_AND_RECEIVE_WAIT_TIMED_OUT : public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;
	bool_t *pTestOk;

	Pr_IRQEvent irqEvent;
	Pr_Task taskA;
	Pr_Task taskB;


	CUT_EDROOMBP_PR_IRQ_EVENT_SEND_AND_RECEIVE_WAIT_TIMED_OUT(const char * id);
	bool_t DoTest(void);

};




/**
 * \brief
 *
 */
class CUT_EDROOMBP_PR_IRQ_EVENT_RECEIVE_WAIT : public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;
	bool_t *pTestOk;

	Pr_IRQEvent irqEvent;
	Pr_Task taskWait;


	CUT_EDROOMBP_PR_IRQ_EVENT_RECEIVE_WAIT(const char * id);
	bool_t DoTest(void);

};




/**
 * \brief
 *
 */
class CUT_EDROOMBP_PR_IRQ_EVENT_RECEIVE_WAIT_COND : public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;
	bool_t *pTestOk;

	Pr_IRQEvent irqEvent;
	Pr_Task taskWait;


	CUT_EDROOMBP_PR_IRQ_EVENT_RECEIVE_WAIT_COND(const char * id);
	bool_t DoTest(void);

};




/**
 * \brief
 *
 */
class CUT_EDROOMBP_PR_IRQ_EVENT_RECEIVE_WAIT_TIMED : public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;
	bool_t *pTestOk;

	Pr_IRQEvent irqEvent;
	Pr_Task taskWait;


	CUT_EDROOMBP_PR_IRQ_EVENT_RECEIVE_WAIT_TIMED(const char * id);
	bool_t DoTest(void);

};

/**
 * \brief
 *
 */
class CUT_EDROOMBP_PR_IRQ_EVENT_SEND_WAIT : public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;
	bool_t *pTestOk;

	Pr_IRQEvent irqEvent;
	Pr_Task taskSignal;


	CUT_EDROOMBP_PR_IRQ_EVENT_SEND_WAIT(const char * id);
	bool_t DoTest(void);

};


/**
 * \brief
 *
 */
class CUT_EDROOMBP_PR_IRQ_EVENT_SEND_WAIT_COND : public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;
	bool_t *pTestOk;

	Pr_IRQEvent irqEvent;
	Pr_Task taskSignal;


	CUT_EDROOMBP_PR_IRQ_EVENT_SEND_WAIT_COND(const char * id);
	bool_t DoTest(void);

};

/**
 * \brief
 *
 */
class CUT_EDROOMBP_PR_IRQ_EVENT_SEND_WAIT_TIMED : public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;
	bool_t *pTestOk;

	Pr_IRQEvent irqEvent;
	Pr_Task taskSignal;


	CUT_EDROOMBP_PR_IRQ_EVENT_SEND_WAIT_TIMED(const char * id);
	bool_t DoTest(void);

};



/*PROTECTED REGION END*/




#endif // PUBLIC__UT_EDROOMBP_PR_IRQ_IFACE_V1_H

