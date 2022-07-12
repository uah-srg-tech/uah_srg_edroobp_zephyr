#include "config.h"
#include "basic_types.h"
#include "ut_base_iface_v1.h"
#include "rand_iface_c99.h"
#include "seed.h"
#include "strlen_iface_c99.h"

//#include <string.h> //CAMBIAR POR EL COMENTADO!!!

#include <stdio.h>

#include <sys/printk.h>

CUTBase * CUTBase::sp_firstTest=NULL;
CUTBase * CUTBase::sp_lastTest=NULL;

static bool_t SetSeed;


//********************************************************************
// Constructor

CUTBase::CUTBase(const char * id,
				bool_t hasPriority,
				bool_t setSeed,
				int32_t seed)
{

	mp_ID=id;
	mp_next=NULL;
	SetSeed = setSeed;

	if(sp_firstTest==NULL)
	{
		sp_firstTest=this;
	    sp_lastTest=this;
        mp_prev=NULL;
	}
	else
	{
		if(hasPriority)
		{
			mp_next=sp_firstTest;
			sp_firstTest->mp_prev=this;
			sp_firstTest=this;
	    	mp_prev=NULL;

		}
		else
		{
    		mp_prev=sp_lastTest;
	    	sp_lastTest->mp_next=this;
        	sp_lastTest=this;
		}
	}

	if (setSeed)
	{
		m_seed=seed;
	}
	else
	{
		m_seed = get_random_seed();
	}

}

//********************************************************************
// Destructor
/*
CUTBase::~CUTBase()
{

	if(mp_next)
	{
		mp_next->mp_prev=mp_prev;
	}
	else
	{
    	sp_lastTest=mp_prev;
	}

	if(mp_prev)
	{
		mp_prev->mp_next=mp_next;
	}
	else
	{
		sp_firstTest=mp_next;
	}

}
*/
//********************************************************************
// Destructor

void CUTBase::ConfigTest()
{
	srand(m_seed);
}

//********************************************************************

bool_t CUTBase::CompareInt8(const int8_t & waitedVal
								, const int8_t & val
								, const char * errorMsg)
{
	bool_t testOK=true;

	if(waitedVal!=val)
	{
		testOK=false;
		printk("%s. Waited Value: %d. Value %d.\n", errorMsg, waitedVal, val);
	}

	return testOK;
}


bool_t CUTBase::CompareBool(const bool_t & waitedVal
                                , const bool_t & val
                                , const char * errorMsg){

	bool_t testOK=true;

	if(waitedVal!=val)
	{
		testOK=false;
		printk("%s. Waited Value: %d. Value %d.\n", errorMsg, waitedVal, val);
	}

	return testOK;

}

bool_t CUTBase::CompareInt16(const int16_t & waitedVal
								, const int16_t & val
								, const char * errorMsg)
{
	bool_t testOK=true;

	if(waitedVal!=val)
	{
		testOK=false;
		printk("%s. Waited Value: %d. Value %d.\n", errorMsg, waitedVal, val);
	}

	return testOK;
}

bool_t CUTBase::CompareInt32(const int32_t & waitedVal
								, const int32_t & val
								, const char * errorMsg)
{
	bool_t testOK=true;

	if(waitedVal!=val)
	{
		testOK=false;
		printk("%s. Waited Value: %d. Value %d.", errorMsg, waitedVal, val);
	}

	return testOK;
}

bool_t CUTBase::CompareInt64(const int64_t & waitedVal
								, const int64_t & val
								, const char * errorMsg)
{
	bool_t testOK=true;

	if(waitedVal!=val)
	{
		testOK=false;
		printk("%s: Waited Value: %lld  Value %lld\n", errorMsg, waitedVal, val);
	}

	return testOK;
}

bool_t CUTBase::CompareUInt8(const uint8_t & waitedVal
								, const uint8_t & val
								, const char * errorMsg)
{
	bool_t testOK=true;

	if(waitedVal!=val)
	{
		testOK=false;
		printk("%s. Waited Value: %u. Value %u.\n", errorMsg, waitedVal, val);
	}

	return testOK;
}

bool_t CUTBase::CompareUInt16(const uint16_t & waitedVal
								 , const uint16_t & val
								 , const char * errorMsg)
{
	bool_t testOK=true;

	if(waitedVal!=val)
	{
		testOK=false;
		printk("%s. Waited Value: %u. Value %u.\n", errorMsg, waitedVal, val);
	}

	return testOK;
}

bool_t CUTBase::CompareUInt32(const uint32_t & waitedVal
								 , const uint32_t & val
								 , const char * errorMsg)
{
	bool_t testOK=true;

	if(waitedVal!=val)
	{
		testOK=false;
		printk("%s. Waited Value: %u. Value %u.\n", errorMsg, waitedVal, val);
	}

	return testOK;
}

bool_t CUTBase::CompareUInt64(const uint64_t & waitedVal
								 , const uint64_t & val
								 , const char * errorMsg)
{
	bool_t testOK=true;

	if(waitedVal!=val)
	{
		testOK=false;
		printk("%s. Waited Value: %llu. Value %llu.\n", errorMsg, waitedVal, val);
	}

	return testOK;
}

// CompareByte

bool_t CUTBase::CompareByte(const byte_t & waitedVal,
								const byte_t & val,
								const char * errorMsg)
{
	bool_t testOK=true;

	if(waitedVal!=val)
	{
		testOK=false;
		printk("%s. Waited Value: %x. Value %x.\n", errorMsg, waitedVal, val);
	}

	return testOK;
}


bool_t CUTBase::CompareWord16(const word16_t & waitedVal
								 , const word16_t & val
								 , const char * errorMsg)
{
	bool_t testOK=true;

	if(waitedVal!=val)
	{
		testOK=false;
		printk("%s. Waited Value: %x. Value %x.\n", errorMsg, waitedVal, val);
	}

	return testOK;
}

bool_t CUTBase::CompareWord32(const word32_t & waitedVal
							, const word32_t & val
							, const char * errorMsg)
{
	bool_t testOK=true;

	if(waitedVal!=val)
	{
		testOK=false;
		printk("%s. Waited Value: %x. Value %x.\n", errorMsg, waitedVal, val);
	}

	return testOK;
}

bool_t CUTBase::CompareFloat(const float & waitedVal
							, const float & val
							, const char * errorMsg)
{
	bool_t testOK=true;

	if(waitedVal!=val)
	{
		testOK=false;
		printk("%s. Waited Value: %f. Value %f.\n", errorMsg, waitedVal, val);
	}

	return testOK;
}

void CUTBase::PrintReport(bool_t result) {

	int32_t length;

	if (result) {
		printk("%s", this->mp_ID);
		length = strlen(this->mp_ID);
		for (; length < 70; length++) {
			printk(" ");
		}
		printk(": OK\n");
	} else {
		printk("%s", this->mp_ID);
		length = strlen(this->mp_ID);
		for (; length < 70; length++) {
			printk(" ");
		}
		printk(": FAIL. seed=%u \n", this->GetSeed());
	}


}

