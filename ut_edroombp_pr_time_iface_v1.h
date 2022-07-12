/**
 * \file	ut_edroombp_pr_time_iface_v1.h
 *
 * \brief	This swinterface declares the test suites applied over the edroombp
 * 			Time Manager (Pr_Time) services for the RTEMSAPI 4.8 improvement
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>

 * \internal
 * Created: 	13-may-2014
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

#ifndef PUBLIC__UT_EDROOMBP_PR_TIME_IFACE_V1_H
#define PUBLIC__UT_EDROOMBP_PR_TIME_IFACE_V1_H

#include "config.h"
#include "basic_types.h"

/*PROTECTED REGION ID(public_ut_edroombp_pr_time_iface_h) ENABLED START*/


#include "ut_base_iface_v1.h"
#include "edroombp.h"

/*PROTECTED REGION ID(public_ut_edroombp_pr_task_iface_h) ENABLED START*/




class CUT_EDROOMBP_PR_TIME_OVERLOADED_OPERATORS: public CUTBase
{
	friend class CUTManager;
public:
	Pr_Kernel kernel;
	bool_t * pTestTimeOk;

	CUT_EDROOMBP_PR_TIME_OVERLOADED_OPERATORS(const char * id);

	bool_t DoTest(void);

};

class CUT_EDROOMBP_PR_TIME_OVERLOADED_OPERATORS_2: public CUTBase
{
	friend class CUTManager;
public:
	Pr_Kernel kernel;
	bool_t * pTestTimeOk;

	CUT_EDROOMBP_PR_TIME_OVERLOADED_OPERATORS_2(const char * id);

	bool_t DoTest(void);

};




/**
 * \brief	The test creates a task and after the execution of this task, the
 * 			test tries to restart it and checks that was deleted.
 *
 */
class CUT_EDROOMBP_PR_TIME_DELAY_IN_DELAY_AT: public CUTBase
{
	friend class CUTManager;
public:
	Pr_Kernel kernel;

	Pr_Task TaskDelayIn;
	Pr_Task TaskDelayAt;

	bool_t *pTestTimeDelayIn;
	bool_t *pTestTimeDelayAt;

	CUT_EDROOMBP_PR_TIME_DELAY_IN_DELAY_AT(const char * id);

	bool_t DoTest(void);

};




/*PROTECTED REGION END*/

#endif // PUBLIC__UT_EDROOMBP_PR_TIME_IFACE_V1_H
