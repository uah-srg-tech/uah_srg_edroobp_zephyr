/**
 * \file	ut_edroombp_pr_semaphore_iface_v1.h
 * \brief	This swinterface declares the test suites applied over the edroombp
 * 			Task Manager (Pr_Task) services for the RTEMSAPI 4.8 improvement
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>

 * \internal
 * Created: 	12/11/2013
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

#ifndef PUBLIC__UT_EDROOMBP_PR_SEMAPHORE_IFACE_V1_H
#define PUBLIC__UT_EDROOMBP_PR_SEMAPHORE_IFACE_V1_H

#include "config.h"
#include "basic_types.h"

#include "ut_base_iface_v1.h"
#include "edroombp.h"

/*PROTECTED REGION ID(public_ut_edroombp_pr_semaphore_iface_h) ENABLED START*/

/**
 * \brief	this function is used to compare a pair of task priorities
 *
 * \param expectedPrio	the priority value expected
 * \param currentPrio	the real priority value of the task
 * \param nameTask	a message to show in case of the priorities were different
 *
 * \return	-# true if the priorities are equal
 * 			-# false if not
 */
bool_t CheckPriorities(TEDROOMPriority expectedPrio,
		TEDROOMPriority currentPrio, const char * nameTask);



class CUT_EDROOMBP_PR_SEMAPHORE_WAIT_AND_SIGNAL_2: public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_SEMAPHORE_WAIT_AND_SIGNAL_2(const char * id);
	bool_t DoTest(void);


	Pr_Task taskC;

	Pr_SemaphoreRec semCeiling;
	Pr_SemaphoreRec semInherit;

	bool_t * pTestOk;



};




class CUT_EDROOMBP_PR_SEMAPHORE_WAIT_AND_SIGNAL: public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_SEMAPHORE_WAIT_AND_SIGNAL(const char * id);
	bool_t DoTest(void);

	/**
	 * This semaphore is used to sleep the Task A, which must be executed first
	 */
	Pr_SemaphoreBin semAux;
	/**
	 * This semaphore is to check the correct operation of the Wait and Signal
	 * services for SemaphoreBin class
	 */
	Pr_SemaphoreBin semBin;
	/**
	 * This semaphore is to check the correct operation of the Wait and Signal
	 * services for SemaphoreRec with Inherit Priority class
	 */
	Pr_SemaphoreRec semRec;
	/**
	 * This semaphore is to check the correct operation of the Wait and Signal
	 * services for SemaphoreRec with Ceiling Priority class
	 */
	Pr_SemaphoreRec semRecCeiling;

	/**
	 * This task uses several shared resources which will be protected, each of
	 * these resources, by a different kind of semaphore
	 */
	Pr_Task taskA;
	/**
	 * This task uses several shared resources which will be protected, each of
	 * these resources, by a different kind of semaphore
	 */
	Pr_Task taskB;


	bool_t * pTaskAOk;
	bool_t * pTaskBOk;



};

/**
 * \brief
 *
 */
class CUT_EDROOMBP_PR_SEMAPHORE_WAIT_COND : public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_SEMAPHORE_WAIT_COND(const char * id);
	bool_t DoTest(void);

	/* semaphores */
	Pr_SemaphoreBin semBin;
	Pr_SemaphoreRec semRec;
	Pr_SemaphoreRec semRecCeiling;

	Pr_Task taskA;
	Pr_Task taskB;

	bool_t * pSemBinLockedOk;
	bool_t * pSemRecLockedOk;
	bool_t * pSemRecCeilingLockedOk;


};



/**
 * \brief
 */
class CUT_EDROOMBP_PR_SEMAPHORE_WAIT_TIMED : public CUTBase
{

	friend class CUTManager;

public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_SEMAPHORE_WAIT_TIMED(const char * id);
	bool_t DoTest(void);

	/* semaphores */
	Pr_SemaphoreBin semBin;


	Pr_Task taskA;
	Pr_Task taskB;

	bool_t * pSemBinTimedOk;
	bool_t * pSemBinLockedOk;


};


/**
 * \brief
 */
class CUT_EDROOMBP_PR_SEMAPHORE_DELETE_SEMAPHORES : public CUTBase
{
	friend class CUTManager;

public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_SEMAPHORE_DELETE_SEMAPHORES(const char * id);
	bool_t DoTest(void);

	/* semaphores */
	Pr_SemaphoreBin semBin;
	Pr_SemaphoreRec semRec;

	Pr_Task taskUseBin;
	Pr_Task taskDelBin;
	Pr_Task taskUseDelRec;
	Pr_Task taskUseRec;

	bool_t * pSemBinOk;
	bool_t * pSemRecDelOk;
	bool_t * pSemRecOk;

};



/**
	 * \brief
	 *
	 */
class CUT_EDROOMBP_PR_SEMAPHORE_INCREASED_CEILING : public CUTBase
{
	friend class CUTManager;

public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_SEMAPHORE_INCREASED_CEILING(const char * id);
	bool_t DoTest(void);
		/* semaphores */
		Pr_SemaphoreBin semBinSuspendA;
		Pr_SemaphoreBin semBinSuspendB;
		Pr_SemaphoreRec semRecCeiling;

		Pr_Task taskA;
		Pr_Task taskB;
		Pr_Task taskC;

		bool_t * pExpectedBVal;
		bool_t * pTestOk;



	};


class CUT_EDROOMBP_PR_SEMAPHORE_PRIORITY_INHERITANCE_1 : public CUTBase
{
	friend class CUTManager;

public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_SEMAPHORE_PRIORITY_INHERITANCE_1(const char * id);
	bool_t DoTest(void);

		/* semaphores */
		Pr_SemaphoreBin semBinSuspendA;
		Pr_SemaphoreBin semBinSuspendB;
		Pr_SemaphoreBin semBinSuspendC;
		Pr_SemaphoreRec semRecInheritance;

		Pr_Task taskA;
		Pr_Task taskB;
		Pr_Task taskC;
		Pr_Task taskD;

		bool_t * pExpectedAVal;
		bool_t * pExpectedBVal;
		bool_t * pExpectedCVal;
		bool_t * pTestOk;


	};


class CUT_EDROOMBP_PR_SEMAPHORE_PRIORITY_INHERITANCE_2 : public CUTBase
{
	friend class CUTManager;

public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_SEMAPHORE_PRIORITY_INHERITANCE_2(const char * id);
	bool_t DoTest(void);

	/* semaphores */
	Pr_SemaphoreBin semBinSuspendA;
	Pr_SemaphoreBin semBinSuspendB;
	Pr_SemaphoreBin semBinSuspendC;
	Pr_SemaphoreRec semRecInheritanceA;
	Pr_SemaphoreRec semRecInheritanceB;

	Pr_Task taskA;
	Pr_Task taskB;
	Pr_Task taskC;
	Pr_Task taskD;

	bool_t * pExpectedAVal;
	bool_t * pExpectedBVal;
	bool_t * pExpectedCVal;
	bool_t * pTestOk;


};


/**
 * \brief
 *
 */
/*
 * TODO: this test seems a little hummm, we can say strange or useless ->
 * review if is really useful
 */
class CUT_EDROOMBP_PR_SEMAPHORE_SEMAPHORE_NESTED_OBTAIN : public CUTBase
{
	friend class CUTManager;

public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_SEMAPHORE_SEMAPHORE_NESTED_OBTAIN(const char * id);
	bool_t DoTest(void);

	Pr_Task mainTask;
	bool_t * pTestOk;


};


/**
 * \todo to complete
 */
class CUT_EDROOMBP_PR_SEMAPHORE_QUEUEING_DISCIPLINE : public CUTBase
{
	friend class CUTManager;

public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_SEMAPHORE_QUEUEING_DISCIPLINE(const char * id);
	bool_t DoTest(void);
	/* semaphores */
	Pr_SemaphoreBin semCountingFIFO;
	Pr_SemaphoreRec semBinaryPriority;

	//			Pr_Task taskA;
	//			Pr_Task taskB;
	Pr_Task taskControl;

	bool_t * pTestOk;

};


/**
//	 * \brief
//	 *
//	 */
class CUT_EDROOMBP_PR_SEMAPHORE_PRIORITY_CEILING : public CUTBase
{
	friend class CUTManager;

public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_SEMAPHORE_PRIORITY_CEILING(const char * id);
	bool_t DoTest(void);

	/* semaphores */
	Pr_SemaphoreBin semSuspendA;
	Pr_SemaphoreBin semSuspendB;
	Pr_SemaphoreRec semCeilingA;
	Pr_SemaphoreRec semCeilingB;

	Pr_Task taskA;
	Pr_Task taskB;

	bool_t * pTestAOk;
	bool_t * pTestBOk;

};


class CUT_EDROOMBP_PR_SEMAPHORE_CEILING_NESTED_OBTAIN : public CUTBase
{
	friend class CUTManager;

public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_SEMAPHORE_CEILING_NESTED_OBTAIN(const char * id);
	bool_t DoTest(void);

		Pr_SemaphoreRec semA;
		Pr_SemaphoreRec semB;
		Pr_SemaphoreRec semC;

		Pr_SemaphoreBin semBinSuspendA;
		Pr_SemaphoreBin semBinSuspendB;
		Pr_SemaphoreBin semBinSuspendC;

		Pr_Task taskA;
		Pr_Task taskB;
		Pr_Task taskC;
		Pr_Task taskD;

		bool_t * pExpectedAVal;
		bool_t * pExpectedBVal;
		bool_t * pExpectedCVal;
		bool_t * pExpectedDVal;


};



class CUT_EDROOMBP_PR_SEMAPHORE_TASK_WITH_HIGHER_PRIORITY_THAN_CEILING : public CUTBase
{
	friend class CUTManager;

public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_SEMAPHORE_TASK_WITH_HIGHER_PRIORITY_THAN_CEILING(const char * id);
	bool_t DoTest(void);

	Pr_Task taskHigherPrio;
	Pr_SemaphoreRec semCeiling;
	bool_t * pExpectedVal;


};


class CUT_EDROOMBP_PR_SEMAPHORE_TASK_PRIORITY_CEILING_SEMAPHORE : public CUTBase
{
	friend class CUTManager;

public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_SEMAPHORE_TASK_PRIORITY_CEILING_SEMAPHORE(const char * id);
	bool_t DoTest(void);

	Pr_Task taskA;

	bool_t * pExpectedVal;


};



class CUT_EDROOMBP_PR_SEMAPHORE_COUNTING : public CUTBase
{
	friend class CUTManager;

public:
	Pr_Kernel kernel;

	CUT_EDROOMBP_PR_SEMAPHORE_COUNTING(const char * id);
	bool_t DoTest(void);

	Pr_SemaphoreBin sem;
	Pr_Task taskA;
	Pr_Task taskB;
	Pr_Task taskC;
	Pr_Task taskD;

	bool_t * pExpectedA;
	bool_t * pExpectedB;
	bool_t * pExpectedC;
	bool_t * pExpectedD;


};




/*PROTECTED REGION END*/

#endif // PUBLIC__UT_EDROOMBP_PR_SEMAPHORE_IFACE_V1_H
