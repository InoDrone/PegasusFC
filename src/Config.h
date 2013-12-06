/*
 * Config.h
 *
 *  Created on: 20 nov. 2013
 *      Author: alienx
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "UserConfig.h"

/***************************************************************
 * CONFIG CHECK
 ***************************************************************/
#ifndef FRAME_TYPE
    #error Frame type is not defined (FRAME_TYPE)
#endif

#ifndef PEGASUS_VERSION
    #error FC version is not defined (PEGASUS_VERSION)
#endif


#if PEGASUS_VERSION == PEGASUS_VB
    #include "platform/include/PegasusSTM32VB.h"
#else
    #error This version is not implemented
#endif

#endif /* CONFIG_H_ */
