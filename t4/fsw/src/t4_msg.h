/*=======================================================================================
** File Name:  t4_msg.h
**
** Title:  Message Definition Header File for T4 Application
**
** $Author:    fsw
** $Revision: 1.1 $
** $Date:      2021-08-11
**
** Purpose:  To define T4's command and telemetry message defintions 
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2021-08-11 | fsw | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _T4_MSG_H_
#define _T4_MSG_H_

/*
** Pragmas
*/

/*
** Include Files
*/



/*
** Local Defines
*/

/*
** T4 command codes
*/
#define T4_NOOP_CC                 0
#define T4_RESET_CC                1

/*
** Local Structure Declarations
*/
typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              usCmdCnt;
    uint8              usCmdErrCnt;

    /* TODO:  Add declarations for additional housekeeping data here */

} T4_HkTlm_t;


#endif /* _T4_MSG_H_ */

/*=======================================================================================
** End of file t4_msg.h
**=====================================================================================*/
    