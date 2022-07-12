/**
 * \file	ut_edroombp_pr_time.cpp
 *
 * \brief	This swpackage implements the test suites applied over the edroombp
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

#include "ut_edroombp_pr_time_iface_v1.h"

#include <stdio.h>


#define ERROR_NOTIFY()	CUT_EDROOMBP::WriteFail("error", __LINE__, __FILE__)

/*
 * The next set of tests are identified under the rtems improvement validation
 * test suites, but each of these tests has been adapted to be applied to  one
 * or more of the edroombp Task Manager service
 */


bool_t testTimeOverloadedOperatorsOk=false;


CUT_EDROOMBP_PR_TIME_OVERLOADED_OPERATORS::CUT_EDROOMBP_PR_TIME_OVERLOADED_OPERATORS(const char * id) :
						CUTBase(id, true, true, 9873829),
						kernel(),
						pTestTimeOk(&testTimeOverloadedOperatorsOk)
{
	Pr_Time time1(1,0);
	Pr_Time time2(2,5);
	Pr_Time timeAux=time1;

	timeAux+=time2;
	if ((timeAux)!=Pr_Time(3,5))
	{
		return;
	}

	timeAux-=time1;
	if (!((timeAux)==time2))
	{
		return;
	}

	if (!(time1<time2))
	{
		return;
	}

	if (!(time2>time1))
	{
		return;
	}

	if (!(time1<=time2))
	{
		return;
	}

	if (!(time2>=time1))
	{
		return;
	}

	if (!(time2<=time2))
	{
		return;
	}

	if (!(time2>=time2))
	{
		return;
	}

	{
		Pr_Time time3(1,500001);
		Pr_Time time4(2,600000);

		time3.RoudMicrosToTicks();

		time3+=time4;

		time3-=time4;
		time3-=time4;

		if (time3==time4)
		{
			return;
		}
		time3=time4;
		if (time3!=time4)
		{
			return;
		}
	}

	{
		Pr_Time time3(2,500000);
		Pr_Time time4(2,600000);

		if (time3==time4)
		{
			return;
		}

		if (!(time3!=time4))
		{
			return;
		}
	}



	testTimeOverloadedOperatorsOk=true;
}


bool_t CUT_EDROOMBP_PR_TIME_OVERLOADED_OPERATORS::DoTest(void)
{
	Pr_DelayIn(Pr_Time(5,0));

	return (*pTestTimeOk);

}



CUT_EDROOMBP_PR_TIME_OVERLOADED_OPERATORS_2::CUT_EDROOMBP_PR_TIME_OVERLOADED_OPERATORS_2(const char * id) :
						CUTBase(id, true, true, 9873829),
						kernel(),
						pTestTimeOk(&testTimeOverloadedOperatorsOk)
{
	Pr_Time initialTime;
	Pr_Time auxTime;
	initialTime.GetTime();

	for (uint8_t i=0;i<10;i++)
	{
		Pr_DelayIn(Pr_Time(1,0));
	}



}

bool_t CUT_EDROOMBP_PR_TIME_OVERLOADED_OPERATORS_2::DoTest(void)
{
	Pr_DelayIn(Pr_Time(20,0));

	return (*pTestTimeOk);

}



/*----------------------------------------------------------------------------*/
/*------------------------------- val_02_03_040 ------------------------------*/
/*----------------------------------------------------------------------------*/


bool_t testTimeDelayIn=false;
bool_t testTimeDelayAt=false;

static Pr_TaskRV_t TASK_DELAY_IN(Pr_TaskP_t)
{
	
	uint32_t expectedTicks;
	Pr_Time time1;
	Pr_Time time2;
	time2.GetTime();

	Pr_DelayIn(Pr_Time(3,0));
	time2+=Pr_Time(3,0);

	expectedTicks=time1.GetTicks();
	/* We are having problems because sometimes the
		ticks expected are one unit less that the expected ticks. In the RTEMS CPU
		Kit Manual, we read: "A characteristic of interval timing is that the actual
		interval period may be a fraction of a tick less than the interval
		requested. This occurs because the time at which the delay timer is set
		up occurs at some time between two clock ticks. Therefore, the first
		countdown tick occurs in less than the complete time interval for a tick.
		This can be a problem if the clock granularity is large.*/

	//Se ha puesto un +1 en la primera comprobación para poder pasar el test
	if (expectedTicks-time2.Ticks()<5 || expectedTicks+1==time2.Ticks())
	{
		testTimeDelayIn=true;
	}
	else
	{
		printf("IN time1 %d\n",expectedTicks);
		printf("IN time2 %d\n",time2.Ticks());
	}

}

static Pr_TaskRV_t TASK_DELAY_AT(Pr_TaskP_t)
{
	
	uint32_t expectedTicks;
	Pr_Time time1;
	Pr_Time time2;
	time2.GetTime();
	time2+=Pr_Time(3,0);

	Pr_DelayAt(time2);

	expectedTicks=time1.GetTicks();
	/* We are having problems because sometimes the
		ticks expected are one unit less that the expected ticks. In the RTEMS CPU
		Kit Manual, we read: "A characteristic of interval timing is that the actual
		interval period may be a fraction of a tick less than the interval
		requested. This occurs because the time at which the delay timer is set
		up occurs at some time between two clock ticks. Therefore, the first
		countdown tick occurs in less than the complete time interval for a tick.
		This can be a problem if the clock granularity is large.*/

	if (expectedTicks-time2.Ticks()<5 || expectedTicks+1==time2.Ticks())
	{
		testTimeDelayAt=true;
	}
	else
	{
		printf("AT time1 %d\n",expectedTicks);
		printf("AT time2 %d\n",time2.Ticks());
	}

}

CUT_EDROOMBP_PR_TIME_DELAY_IN_DELAY_AT::CUT_EDROOMBP_PR_TIME_DELAY_IN_DELAY_AT(const char * id) :
						CUTBase(id, true, true, 9873829),
						kernel(),
						TaskDelayIn(TASK_DELAY_IN, EDROOMprioHigh, 2048),
						TaskDelayAt(TASK_DELAY_AT, EDROOMprioNormal, 2048),
						pTestTimeDelayIn(&testTimeDelayIn),
						pTestTimeDelayAt(&testTimeDelayAt)
{

}


bool_t CUT_EDROOMBP_PR_TIME_DELAY_IN_DELAY_AT::DoTest(void)
{
	Pr_DelayIn(Pr_Time(10,0));
	return ((*pTestTimeDelayIn)&&(*pTestTimeDelayAt));

}


