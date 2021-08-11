/*=======================================================================================
** File Name:  t4_private_types.h
**
** Title:  Type Header File for T4 Application
**
** $Author:    fsw
** $Revision: 1.1 $
** $Date:      2021-08-11
**
** Purpose:  This header file contains declarations and definitions of all T4's private
**           data structures and data types.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2021-08-11 | fsw | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _T4_PRIVATE_TYPES_H_
#define _T4_PRIVATE_TYPES_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/

typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} T4_NoArgCmd_t;


typedef struct
{
    uint32  counter;

    /* TODO:  Add input data to this application here, such as raw data read from I/O
    **        devices or data subscribed from other apps' output data.
    */

} T4_InData_t;

typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  uiCounter;
} T4_OutData_t;

/* TODO:  Add more private structure definitions here, if necessary. */

/*
** External Global Variables
*/

/*
** Global Variables
*/

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/

#endif /* _T4_PRIVATE_TYPES_H_ */

/*=======================================================================================
** End of file t4_private_types.h
**=====================================================================================*/
    