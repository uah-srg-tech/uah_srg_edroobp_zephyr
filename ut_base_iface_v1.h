#ifndef PUBLIC__UT_BASE_IFACE_V1_H
#define PUBLIC__UT_BASE_IFACE_V1_H

#include "config.h"
#include "basic_types.h"
#include <stdlib.h>	//necesaria para "size_t"

/*PROTECTED REGION ID(public_ut_base_iface_h) ENABLED START*/

/*! \class CUTBase  UTManager.h "UTManager.h"
*   \author Oscar Rodriguez Polo
*   \brief base class of unitary test
*/
class CUTBase
{

    //! the CUTManager manages all the unitary test
    friend class CUTManager;

    protected:


        //**********************************************************************
        //!{ Handling a list of CUT classes

        //! static pointer to the first of a list of CUT classes
        static CUTBase * sp_firstTest;
        //! static pointer to the last of a list of CUT classes
        static CUTBase * sp_lastTest;

        //! pointer to the next of a list of CUT classes
        CUTBase * mp_next;
        //! pointer to the previous of a list of CUT classes
        CUTBase * mp_prev;

        //!}
        //**********************************************************************


        //! Test identificator
        const char * mp_ID;

        //! Seed at test init
        unsigned m_seed;


    protected:

        //**********************************************************************
        //!{ Handling CUT SubSystem


        //! Test Configuration
        unsigned GetSeed(){return m_seed;}

        const char * GetID(){return mp_ID;};

        //!}
        //**********************************************************************


        /*! Constructor. Inits the unitary test identificator and
         *    adds this object to the CUT classes list
         *    \param id test identificator
         *   \param hasPriority, set to true if the test has priority
         *    in execution
         */
        CUTBase(const char * id, bool_t hasPriority=false, bool_t setSeed=false,
                int32_t seed=0);



        /*! Compare Bool
                 *    \param vaitedVal    waited Value
                 *    \param val            value
                 *   \param errorMsg error msg
                 */
        bool_t CompareBool(const bool_t & waitedVal
                                        , const bool_t & val
                                        , const char * errorMsg);

        /*! Compare Int8
         *    \param vaitedVal    waited Value
         *    \param val            value
         *   \param errorMsg error msg
         */
        bool_t CompareInt8(const int8_t & waitedVal
                                , const int8_t & val
                                , const char * errorMsg);


        /*! Compare Int16
         *    \param vaitedVal    waited Value
         *    \param val            value
         *   \param errorMsg error msg
         */
        bool_t CompareInt16(const int16_t & waitedVal
                                , const int16_t & val
                                , const char * errorMsg);

        /*! Compare Int32
         *    \param vaitedVal    waited Value
         *    \param val            value
         *   \param errorMsg error msg
         */
        bool_t CompareInt32(const int32_t & waitedVal
                                , const int32_t & val
                                , const char * errorMsg);

        /*! Compare Int64
         *    \param vaitedVal    waited Value
         *    \param val            value
         *   \param errorMsg error msg
         */
        bool_t CompareInt64(const int64_t & waitedVal
                                , const int64_t & val
                                , const char * errorMsg);


        /*! Compare UInt8
         *    \param vaitedVal    waited Value
         *    \param val            value
         *   \param errorMsg error msg
         */
        bool_t CompareUInt8(const uint8_t & waitedVal
                                , const uint8_t & val
                                , const char * errorMsg);


        /*! Compare UInt16
         *    \param vaitedVal    waited Value
         *    \param val            value
         *   \param errorMsg error msg
         */
        bool_t CompareUInt16(const uint16_t & waitedVal
                                , const uint16_t & val
                                , const char * errorMsg);

        /*! Compare UInt32
         *    \param vaitedVal    waited Value
         *    \param val            value
         *   \param errorMsg error msg
         */
        bool_t CompareUInt32(const uint32_t & waitedVal
                                , const uint32_t & val
                                , const char * errorMsg);

        /*! Compare UInt64
         *    \param vaitedVal    waited Value
         *    \param val            value
         *   \param errorMsg error msg
         */
        bool_t CompareUInt64(const uint64_t & waitedVal
                                , const uint64_t & val
                                , const char * errorMsg);


        /*! Compare Bytes
         *    \param vaitedVal    waited Value
         *    \param val            value
         *   \param errorMsg error msg
         */
        bool_t CompareByte(const byte_t & waitedVal
                                , const byte_t & val
                                , const char * errorMsg);


        /*! Compare Word16
         *    \param vaitedVal    waited Value
         *    \param val            value
         *   \param errorMsg error msg
         */
        bool_t CompareWord16(const word16_t & waitedVal
                                    , const word16_t & val
                                    , const char * errorMsg);


        /*! Compare Word32
         *    \param vaitedVal    waited Value
         *    \param val            value
         *   \param errorMsg error msg
         */
        bool_t CompareWord32(const word32_t & waitedVal
                                    , const word32_t & val
                                    , const char * errorMsg);

        /*! Compare Float
         *    \param vaitedVal    waited Value
         *    \param val            value
         *   \param errorMsg error msg
         */
        bool_t CompareFloat(const float & waitedVal
                                    , const float & val
                                    , const char * errorMsg);

    public:

        //! All CUT classes must define the method DoTest
        bool_t DoTest(void);

        //! Test Configuration
        void ConfigTest();

        //! Prints the result of the test
        void PrintReport(bool_t result);
};


/*PROTECTED REGION END*/

#endif // PUBLIC__UT_BASE_IFACE_V1_H
