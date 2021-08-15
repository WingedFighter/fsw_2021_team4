/*=======================================================================================
** File Name:  t4_app.h
**
** Title:  Header File for T4 Application
**
** $Author:    fsw
** $Revision: 1.1 $
** $Date:      2021-08-11
**
** Purpose:  To define T4's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2021-08-11 | fsw | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _T4_APP_H_
#define _T4_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "t4_platform_cfg.h"
#include "t4_mission_cfg.h"
#include "t4_private_ids.h"
#include "t4_private_types.h"
#include "t4_perfids.h"
#include "t4_msgids.h"
#include "t4_msg.h"



/*
** Local Defines
*/
#define T4_TIMEOUT_MSEC    1000

/*
** Local Structure Declarations
*/
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[T4_EVT_CNT];

    /* CFE scheduling pipe */
    CFE_SB_PipeId_t  SchPipeId; 
    uint16           usSchPipeDepth;
    char             cSchPipeName[OS_MAX_API_NAME];

    /* CFE command pipe */
    CFE_SB_PipeId_t  CmdPipeId;
    uint16           usCmdPipeDepth;
    char             cCmdPipeName[OS_MAX_API_NAME];
    
    /* CFE telemetry pipe */
    CFE_SB_PipeId_t  TlmPipeId;
    uint16           usTlmPipeDepth;
    char             cTlmPipeName[OS_MAX_API_NAME];

    /* Task-related */
    uint32  uiRunStatus;
    
    /* Input data - from I/O devices or subscribed from other apps' output data.
       Data structure should be defined in t4/fsw/src/t4_private_types.h */
    T4_InData_t   InData;

    /* Output data - to be published at the end of a Wakeup cycle.
       Data structure should be defined in t4/fsw/src/t4_private_types.h */
    T4_OutData_t  OutData;

    /* Housekeeping telemetry - for downlink only.
       Data structure should be defined in t4/fsw/src/t4_msg.h */
    T4_HkTlm_t  HkTlm;
} T4_AppData_t;

/*
** External Global Variables
*/

/*
** Global Variables
*/

/*
** Local Variables
*/

T4_Wise_Tlm_t wise_tlm;

/*
** Local Function Prototypes
*/
int32  T4_InitApp(void);
int32  T4_InitEvent(void);
int32  T4_InitData(void);
int32  T4_InitPipe(void);

void  T4_AppMain(void);

void  T4_CleanupCallback(void);

int32  T4_RcvMsg(int32 iBlocking);

void  T4_ProcessNewData(void);
void  T4_ProcessNewCmds(void);
void  T4_ProcessNewAppCmds(CFE_SB_Msg_t*);

void  T4_ReportHousekeeping(void);
void  T4_SendOutData(void);

void  T4_ManageCaps(void);
void  T4_dischargeCaps(void);

uint16 getActiveCharge(void);
int   calcActiveCap(void);

boolean  T4_VerifyCmdLength(CFE_SB_Msg_t*, uint16);

#endif /* _T4_APP_H_ */

/*=======================================================================================
** End of file t4_app.h
**=====================================================================================*/
    
