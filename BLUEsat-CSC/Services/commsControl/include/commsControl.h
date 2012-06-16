 /**
 *  \file commsControl.h
 *
 *  \brief control task for message sending
 *  This task sends message to switching circuit control
 *  to switch to the modem, then send message to modem
 *
 *  \author $Author: Sam Jiang $
 *  \version 1.0
 *
 *  $Date: 2/6/2012
 */

#ifndef COMMS_H_
#define COMMS_H_

void vComms_Init(unsigned portBASE_TYPE uxPriority);


#endif /* COMMS_H_ */