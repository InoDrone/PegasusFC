/*
 * Config.h
 *
 *  Created on: 20 nov. 2013
 *      Author: alienx
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define PEGASUS_VERSION PEGASUS_VB

#if PEGASUS_VERSION == PEGASUS_VB
    #include "platform/include/PegasusSTM32VB.h"
#else
    #error This version is not implemented
#endif


#endif /* CONFIG_H_ */
