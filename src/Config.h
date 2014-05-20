/*
 * Config.h
 *
 *  Created on: 20 nov. 2013
 *      Author: alienx
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define PEGASUS_VERSION PEGASUS_V2B

#if PEGASUS_VERSION == PEGASUS_VB
    #include "platform/include/PegasusSTM32VB.h"
#elif PEGASUS_VERSION == PEGASUS_V2B
	#include "platform/include/PegasusSTM32V2B.h"
#else
    #error This version is not implemented
#endif


#endif /* CONFIG_H_ */
