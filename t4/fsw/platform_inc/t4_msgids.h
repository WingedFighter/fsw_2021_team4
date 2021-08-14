/*=======================================================================================
** File Name:  t4_msgids.h
**
** Title:  Message ID Header File for T4 Application
**
** $Author:    fsw
** $Revision: 1.1 $
** $Date:      2021-08-11
**
** Purpose:  This header file contains declartions and definitions of all T4's 
**           Message IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2021-08-11 | fsw | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _T4_MSGIDS_H_
#define _T4_MSGIDS_H_

/***** TODO:  These Message ID values are default and may need to be changed by the developer  *****/
#define T4_CMD_MID            	0x18C0
#define T4_SEND_HK_MID        	0x18C1
#define T4_WAKEUP_MID        	0x18D0
#define T4_OUT_DATA_MID        	0x18D1

#define T4_HK_TLM_MID		0x08BB
#define T4_HEALTH_CRITICAL      0x08BD
#define T4_CAP_A_DISCHARGE      0x08BE
#define T4_CAP_B_DISCHARGE      0x08BF
#define T4_CAP_C_DISCHARGE      0x08C0

#endif /* _T4_MSGIDS_H_ */

/*=======================================================================================
** End of file t4_msgids.h
**=====================================================================================*/
    
