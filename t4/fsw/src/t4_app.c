/*=======================================================================================
** File Name:  t4_app.c
**
** Title:  Function Definitions for T4 Application
**
** $Author:    fsw
** $Revision: 1.1 $
** $Date:      2021-08-11
**
** Purpose:  This source file contains all necessary function definitions to run T4
**           application.
**
** Functions Defined:
**    Function X - Brief purpose of function X
**    Function Y - Brief purpose of function Y
**    Function Z - Brief purpose of function Z
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to all functions in the file.
**    2. List the external source(s) and event(s) that can cause the funcs in this
**       file to execute.
**    3. List known limitations that apply to the funcs in this file.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2021-08-11 | fsw | Build #: Code Started
**
**=====================================================================================*/

/*
** Pragmas
*/

/*
** Include Files
*/
#include <string.h>

#include "cfe.h"

#include "t4_platform_cfg.h"
#include "t4_mission_cfg.h"
#include "t4_app.h"
#include "t4_msg.h"
#include "wise_msgids.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/

/*
** External Global Variables
*/

/*
** Global Variables
*/
T4_AppData_t  g_T4_AppData;

/*
** Local Variables
*/

/*
** Local Function Definitions
*/
    
/*=====================================================================================
** Name: T4_InitEvent
**
** Purpose: To initialize and register event table for T4 application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_EVS_Register
**    CFE_ES_WriteToSysLog
**
** Called By:
**    T4_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_T4_AppData.EventTbl
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw 
**
** History:  Date Written  2021-08-11
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 T4_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;

    /* Create the event table */
    memset((void*)g_T4_AppData.EventTbl, 0x00, sizeof(g_T4_AppData.EventTbl));

    g_T4_AppData.EventTbl[0].EventID = T4_RESERVED_EID;
    g_T4_AppData.EventTbl[1].EventID = T4_INF_EID;
    g_T4_AppData.EventTbl[2].EventID = T4_INIT_INF_EID;
    g_T4_AppData.EventTbl[3].EventID = T4_ILOAD_INF_EID;
    g_T4_AppData.EventTbl[4].EventID = T4_CDS_INF_EID;
    g_T4_AppData.EventTbl[5].EventID = T4_CMD_INF_EID;

    g_T4_AppData.EventTbl[ 6].EventID = T4_ERR_EID;
    g_T4_AppData.EventTbl[ 7].EventID = T4_INIT_ERR_EID;
    g_T4_AppData.EventTbl[ 8].EventID = T4_ILOAD_ERR_EID;
    g_T4_AppData.EventTbl[ 9].EventID = T4_CDS_ERR_EID;
    g_T4_AppData.EventTbl[10].EventID = T4_CMD_ERR_EID;
    g_T4_AppData.EventTbl[11].EventID = T4_PIPE_ERR_EID;
    g_T4_AppData.EventTbl[12].EventID = T4_MSGID_ERR_EID;
    g_T4_AppData.EventTbl[13].EventID = T4_MSGLEN_ERR_EID;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(g_T4_AppData.EventTbl,
                               T4_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("T4 - Failed to register with EVS (0x%08X)\n", iStatus);
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: T4_InitPipe
**
** Purpose: To initialize all message pipes and subscribe to messages for T4 application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_CreatePipe
**    CFE_SB_Subscribe
**    CFE_ES_WriteToSysLog
**
** Called By:
**    T4_InitApp
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_T4_AppData.usSchPipeDepth
**    g_T4_AppData.cSchPipeName
**    g_T4_AppData.SchPipeId
**    g_T4_AppData.usCmdPipeDepth
**    g_T4_AppData.cCmdPipeName
**    g_T4_AppData.CmdPipeId
**    g_T4_AppData.usTlmPipeDepth
**    g_T4_AppData.cTlmPipeName
**    g_T4_AppData.TlmPipeId
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw 
**
** History:  Date Written  2021-08-11
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 T4_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe */
    g_T4_AppData.usSchPipeDepth = T4_SCH_PIPE_DEPTH;
    memset((void*)g_T4_AppData.cSchPipeName, '\0', sizeof(g_T4_AppData.cSchPipeName));
    strncpy(g_T4_AppData.cSchPipeName, "T4_SCH_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to Wakeup messages */
    iStatus = CFE_SB_CreatePipe(&g_T4_AppData.SchPipeId,
                                 g_T4_AppData.usSchPipeDepth,
                                 g_T4_AppData.cSchPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(T4_WAKEUP_MID, g_T4_AppData.SchPipeId, CFE_SB_Default_Qos, 1);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("T4 - Sch Pipe failed to subscribe to T4_WAKEUP_MID. (0x%08X)\n", iStatus);
            goto T4_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("T4 - Failed to create SCH pipe (0x%08X)\n", iStatus);
        goto T4_InitPipe_Exit_Tag;
    }

    /* Init command pipe */
    g_T4_AppData.usCmdPipeDepth = T4_CMD_PIPE_DEPTH ;
    memset((void*)g_T4_AppData.cCmdPipeName, '\0', sizeof(g_T4_AppData.cCmdPipeName));
    strncpy(g_T4_AppData.cCmdPipeName, "T4_CMD_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&g_T4_AppData.CmdPipeId,
                                 g_T4_AppData.usCmdPipeDepth,
                                 g_T4_AppData.cCmdPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(T4_CMD_MID, g_T4_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("T4 - CMD Pipe failed to subscribe to T4_CMD_MID. (0x%08X)\n", iStatus);
            goto T4_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_Subscribe(T4_SEND_HK_MID, g_T4_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("T4 - CMD Pipe failed to subscribe to T4_SEND_HK_MID. (0x%08X)\n", iStatus);
            goto T4_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("T4 - Failed to create CMD pipe (0x%08X)\n", iStatus);
        goto T4_InitPipe_Exit_Tag;
    }

    /* Init telemetry pipe */
    g_T4_AppData.usTlmPipeDepth = T4_TLM_PIPE_DEPTH;
    memset((void*)g_T4_AppData.cTlmPipeName, '\0', sizeof(g_T4_AppData.cTlmPipeName));
    strncpy(g_T4_AppData.cTlmPipeName, "T4_TLM_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to telemetry messages on the telemetry pipe */
    iStatus = CFE_SB_CreatePipe(&g_T4_AppData.TlmPipeId,
                                 g_T4_AppData.usTlmPipeDepth,
                                 g_T4_AppData.cTlmPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* TODO:  Add CFE_SB_Subscribe() calls for other apps' output data here.
        **
        ** Examples:
        **     CFE_SB_Subscribe(GNCEXEC_OUT_DATA_MID, g_T4_AppData.TlmPipeId);
        */
        CFE_SB_Subscribe(WISE_HK_TLM_MID, g_T4_AppData.TlmPipeId);
    }
    else
    {
        CFE_ES_WriteToSysLog("T4 - Failed to create TLM pipe (0x%08X)\n", iStatus);
        goto T4_InitPipe_Exit_Tag;
    }

T4_InitPipe_Exit_Tag:
    return (iStatus);
}
    
/*=====================================================================================
** Name: T4_InitData
**
** Purpose: To initialize global variables used by T4 application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_InitMsg
**
** Called By:
**    T4_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_T4_AppData.InData
**    g_T4_AppData.OutData
**    g_T4_AppData.HkTlm
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw 
**
** History:  Date Written  2021-08-11
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 T4_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init input data */
    memset((void*)&g_T4_AppData.InData, 0x00, sizeof(g_T4_AppData.InData));

    /* Init output data */
    memset((void*)&g_T4_AppData.OutData, 0x00, sizeof(g_T4_AppData.OutData));
    CFE_SB_InitMsg(&g_T4_AppData.OutData,
                   T4_OUT_DATA_MID, sizeof(g_T4_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&g_T4_AppData.HkTlm, 0x00, sizeof(g_T4_AppData.HkTlm));
    CFE_SB_InitMsg(&g_T4_AppData.HkTlm,
                   T4_HK_TLM_MID, sizeof(g_T4_AppData.HkTlm), TRUE);

    memset((void*)&wise_tlm, 0x00, sizeof(wise_tlm));

    return (iStatus);
}
    
/*=====================================================================================
** Name: T4_InitApp
**
** Purpose: To initialize all data local to and used by T4 application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_WriteToSysLog
**    CFE_EVS_SendEvent
**    OS_TaskInstallDeleteHandler
**    T4_InitEvent
**    T4_InitPipe
**    T4_InitData
**
** Called By:
**    T4_AppMain
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw 
**
** History:  Date Written  2021-08-11
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 T4_InitApp()
{
    int32  iStatus=CFE_SUCCESS;

    g_T4_AppData.uiRunStatus = CFE_ES_APP_RUN;

    iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("T4 - Failed to register the app (0x%08X)\n", iStatus);
        goto T4_InitApp_Exit_Tag;
    }

    if ((T4_InitEvent() != CFE_SUCCESS) || 
        (T4_InitPipe() != CFE_SUCCESS) || 
        (T4_InitData() != CFE_SUCCESS))
    {
        iStatus = -1;
        goto T4_InitApp_Exit_Tag;
    }

    /* Install the cleanup callback */
    OS_TaskInstallDeleteHandler((void*)&T4_CleanupCallback);

T4_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(T4_INIT_INF_EID, CFE_EVS_INFORMATION,
                          "T4 - Application initialized");
    }
    else
    {
        CFE_ES_WriteToSysLog("T4 - Application failed to initialize\n");
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: T4_CleanupCallback
**
** Purpose: To handle any neccesary cleanup prior to application exit
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    TBD
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw 
**
** History:  Date Written  2021-08-11
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void T4_CleanupCallback()
{
    /* TODO:  Add code to cleanup memory and other cleanup here */
}
    
/*=====================================================================================
** Name: T4_RcvMsg
**
** Purpose: To receive and process messages for T4 application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization 
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**    CFE_ES_PerfLogEntry
**    CFE_ES_PerfLogExit
**    T4_ProcessNewCmds
**    T4_ProcessNewData
**    T4_SendOutData
**
** Called By:
**    T4_Main
**
** Global Inputs/Reads:
**    g_T4_AppData.SchPipeId
**
** Global Outputs/Writes:
**    g_T4_AppData.uiRunStatus
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw 
**
** History:  Date Written  2021-08-11
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 T4_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(T4_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, g_T4_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(T4_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case T4_WAKEUP_MID:
                T4_ProcessNewCmds();
                T4_ProcessNewData();

                /* TODO:  Add more code here to handle other things when app wakes up */
                T4_ManageCaps();

                /* The last thing to do at the end of this Wakeup cycle should be to
                   automatically publish new output. */
                T4_SendOutData();
                break;

            default:
                CFE_EVS_SendEvent(T4_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "T4 - Recvd invalid SCH msgId (0x%08X)", MsgId);
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* If there's no incoming message, you can do something here, or nothing */
    }
    else
    {
        /* This is an example of exiting on an error.
        ** Note that a SB read error is not always going to result in an app quitting.
        */
        CFE_EVS_SendEvent(T4_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "T4: SB pipe read error (0x%08X), app will exit", iStatus);
        g_T4_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: T4_ProcessNewData
**
** Purpose: To process incoming data subscribed by T4 application
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**
** Called By:
**    T4_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    None
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw 
**
** History:  Date Written  2021-08-11
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void T4_ProcessNewData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   TlmMsgPtr=NULL;
    CFE_SB_MsgId_t  TlmMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&TlmMsgPtr, g_T4_AppData.TlmPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            TlmMsgId = CFE_SB_GetMsgId(TlmMsgPtr);
            switch (TlmMsgId)
            {
                /* TODO:  Add code to process all subscribed data here 
                **
                ** Example:
                **     case NAV_OUT_DATA_MID:
                **         T4_ProcessNavData(TlmMsgPtr);
                **         break;
                */
                case WISE_HK_TLM_MID:;
                    T4_Wise_Tlm_t* temp = ((T4_Wise_Tlm_t *) TlmMsgPtr);
                    wise_tlm = *temp;

                    CFE_EVS_SendEvent(T4_CMD_INF_EID, CFE_EVS_INFORMATION,
                                      "T4 - wiseSbcState (%u)",
                                      wise_tlm.wiseSbcState);

                    CFE_EVS_SendEvent(T4_CMD_INF_EID, CFE_EVS_INFORMATION,
                                      "T4 - wiseDamage (%u)",
                                      wise_tlm.wiseDamage);

                    CFE_EVS_SendEvent(T4_CMD_INF_EID, CFE_EVS_INFORMATION,
                                      "T4 - wiseCapACharge (%u)",
                                      wise_tlm.wiseCapA_Charge);

                    CFE_EVS_SendEvent(T4_CMD_INF_EID, CFE_EVS_INFORMATION,
                                      "T4 - wiseCapBCharge (%u)",
                                      wise_tlm.wiseCapB_Charge);

                    CFE_EVS_SendEvent(T4_CMD_INF_EID, CFE_EVS_INFORMATION,
                                      "T4 - wiseCapCCharge (%u)",
                                      wise_tlm.wiseCapC_Charge);

                    break;

                default:
                    CFE_EVS_SendEvent(T4_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "T4 - Recvd invalid TLM msgId (0x%08X)", TlmMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(T4_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "T4: CMD pipe read error (0x%08X)", iStatus);
            g_T4_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: T4_ProcessNewCmds
**
** Purpose: To process incoming command messages for T4 application
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**    T4_ProcessNewAppCmds
**    T4_ReportHousekeeping
**
** Called By:
**    T4_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    None
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw 
**
** History:  Date Written  2021-08-11
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void T4_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, g_T4_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case T4_CMD_MID:
                    T4_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                case T4_SEND_HK_MID:
                    T4_ReportHousekeeping();
                    break;

                /* TODO:  Add code to process other subscribed commands here
                **
                ** Example:
                **     case CFE_TIME_DATA_CMD_MID:
                **         T4_ProcessTimeDataCmd(CmdMsgPtr);
                **         break;
                */

                default:
                    CFE_EVS_SendEvent(T4_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "T4 - Recvd invalid CMD msgId (0x%08X)", CmdMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(T4_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "T4: CMD pipe read error (0x%08X)", iStatus);
            g_T4_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: T4_ProcessNewAppCmds
**
** Purpose: To process command messages targeting T4 application
**
** Arguments:
**    CFE_SB_Msg_t*  MsgPtr - new command message pointer
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_GetCmdCode
**    CFE_EVS_SendEvent
**
** Called By:
**    T4_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_T4_AppData.HkTlm.usCmdCnt
**    g_T4_AppData.HkTlm.usCmdErrCnt
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw 
**
** History:  Date Written  2021-08-11
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void T4_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;
    T4_ParmCmd_t *CmdPtr = NULL;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case T4_NOOP_CC:
                g_T4_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(T4_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "T4 - Recvd NOOP cmd (%d)", uiCmdCode);
                break;

            case T4_RESET_CC:
                g_T4_AppData.HkTlm.usCmdCnt = 0;
                g_T4_AppData.HkTlm.usCmdErrCnt = 0;
                CFE_EVS_SendEvent(T4_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "T4 - Recvd RESET cmd (%d)", uiCmdCode);
                break;

            /* TODO:  Add code to process the rest of the T4 commands here */

            case T4_SET_CAP_A_STATE_CC:; //Set Cap State to specific value
                CmdPtr = (T4_ParmCmd_t*) MsgPtr;
                //g_T4_AppData.HkTlm.cap_a_state = CmdPtr->target;
                break;
            case T4_SET_CAP_B_STATE_CC:; //Set Cap State to specific value
                CmdPtr = (T4_ParmCmd_t*) MsgPtr;
                //g_T4_AppData.HkTlm.cap_b_state = CmdPtr->target;
                break;
            case T4_SET_CAP_C_STATE_CC:; //Set Cap State to specific value
                CmdPtr = (T4_ParmCmd_t*) MsgPtr;
                //g_T4_AppData.HkTlm.cap_c_state = CmdPtr->target;
                break;
            case T4_SET_ACTIVE_CAP_CC:; //Set Active Cap to specific value
                CmdPtr = (T4_ParmCmd_t*) MsgPtr;
                //g_T4_AppData.HkTlm.active_cap = CmdPtr->target;
                break;
            case T4_SET_HEALTH_CC:; //Set Health state to specific value
                CmdPtr = (T4_ParmCmd_t*) MsgPtr;
                //g_T4_AppData.HkTlm.health = CmdPtr->target;
                break;
            case T4_SET_OBS_TLD_CC:; //Set Obs threshold to specific value
                CmdPtr = (T4_ParmCmd_t*) MsgPtr;
                //g_T4_AppData.HkTlm.obs_threshold = CmdPtr->target;
                break;
            case T4_SET_LVR_TLD_CC:; //Set louver threshold to specific value
                CmdPtr = (T4_ParmCmd_t*) MsgPtr;
                //g_T4_AppData.HkTlm.louver_threshold = CmdPtr->target;
                break;
            case T4_SET_CRT_TLD_CC:; //Set health critical threshold to specific value
                CmdPtr = (T4_ParmCmd_t*) MsgPtr;
                //g_T4_AppData.HkTlm.critical_threshold = CmdPtr->target;
                break;
            case T4_SET_HEAT_TLD_CC:; //Set heat threshold to specific value
                CmdPtr = (T4_ParmCmd_t*) MsgPtr;
                //g_T4_AppData.HkTlm.heat_threshold = CmdPtr->target;
                break;
            case T4_SET_VERBOSITY_CC:; //Change the verbosity of the app
                break;
            case T4_SET_OBS_STATE_CC:; //Set obs state
                CmdPtr = (T4_ParmCmd_t*) MsgPtr;
                //g_T4_AppData.HkTlm.obs = CmdPtr->target;
            case T4_SET_LVR_STATE_CC:; //Set louver state
                CmdPtr = (T4_ParmCmd_t*) MsgPtr;
                //g_T4_AppData.HkTlm.louver = CmdPtr->target;
            case T4_SET_HEAT_STATE_CC:; //Set heat state
                CmdPtr = (T4_ParmCmd_t*) MsgPtr;
                //g_T4_AppData.HkTlm.heat = CmdPtr->target;
            case T4_GET_CAP_A_CHARGE_CC:; //Update capacitor charge values (A)
            case T4_GET_CAP_B_CHARGE_CC:; //Update capacitor charge values (B)
            case T4_GET_CAP_C_CHARGE_CC:; //Update capacitor charge values (C)
            case T4_GET_OBS_STATE_CC:; //Get obs state
            case T4_GET_LVR_STATE_CC:; //Get louver state
            case T4_GET_HEAT_STATE_CC:; //Get heat state
            default:
                g_T4_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(T4_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "T4 - Recvd invalid cmdId (%d)", uiCmdCode);
                break;
        }
    }
}
    
/*=====================================================================================
** Name: T4_ReportHousekeeping
**
** Purpose: To send housekeeping message
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    T4_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  GSFC, fsw
**
** History:  Date Written  2021-08-11
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void T4_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_T4_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_T4_AppData.HkTlm);
}
    
/*=====================================================================================
** Name: T4_SendOutData
**
** Purpose: To publish 1-Wakeup cycle output data
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    T4_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw
**
** History:  Date Written  2021-08-11
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void T4_SendOutData()
{
    /* TODO:  Add code to update output data, if needed, here.  */

    T4_ReportHousekeeping();

    //CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_T4_AppData.OutData);
    //CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_T4_AppData.OutData);
}
    
/*=====================================================================================
** Name: T4_VerifyCmdLength
**
** Purpose: To verify command length for a particular command message
**
** Arguments:
**    CFE_SB_Msg_t*  MsgPtr      - command message pointer
**    uint16         usExpLength - expected command length
**
** Returns:
**    boolean bResult - result of verification
**
** Routines Called:
**    TBD
**
** Called By:
**    T4_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw 
**
** History:  Date Written  2021-08-11
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
boolean T4_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
{
    boolean bResult=FALSE;
    uint16  usMsgLen=0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            CFE_EVS_SendEvent(T4_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "T4 - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            g_T4_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}
    
/*=====================================================================================
** Name: T4_AppMain
**
** Purpose: To define T4 application's entry point and main process loop
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_RunLoop
**    CFE_ES_PerfLogEntry
**    CFE_ES_PerfLogExit
**    CFE_ES_ExitApp
**    CFE_ES_WaitForStartupSync
**    T4_InitApp
**    T4_RcvMsg
**
** Called By:
**    TBD
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw 
**
** History:  Date Written  2021-08-11
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void T4_AppMain()
{
    /* Register the application with Executive Services */
    CFE_ES_RegisterApp();

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(T4_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (T4_InitApp() != CFE_SUCCESS)
    {
        g_T4_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    } else {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(T4_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(T4_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(T4_MAIN_TASK_PERF_ID);
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&g_T4_AppData.uiRunStatus) == TRUE)
    {
        T4_RcvMsg(CFE_SB_PEND_FOREVER);
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(T4_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(g_T4_AppData.uiRunStatus);
}

void T4_ManageCaps()
{

    if (calcActiveCap() != -1 && calcActiveCap() != wise_tlm.wiseActiveCap)
    {
        T4_WISE_ParmCmd_t temp_parm;
        CFE_SB_InitMsg(&temp_parm, WISE_CMD_MID, sizeof(temp_parm), TRUE);
        temp_parm.target = g_T4_AppData.HkTlm.active_cap;
        CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&temp_parm, WISE_CAP_ACTIVE_CC);
        CFE_SB_SendMsg((CFE_SB_Msg_t*)&temp_parm);
    }

    //if (getActiveCharge() > g_T4_AppData.HkTlm.obs_threshold)
    if (getActiveCharge() > 8500)
    {
        T4_WISE_ParmCmd_t temp_parm;
        CFE_SB_InitMsg(&temp_parm, WISE_CMD_MID, sizeof(temp_parm), TRUE);
        CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&temp_parm, WISE_OBS_START_CC);
        CFE_SB_SendMsg((CFE_SB_Msg_t*)&temp_parm);
        g_T4_AppData.HkTlm.obs = 1;
    }

    if (getActiveCharge() < g_T4_AppData.HkTlm.critical_threshold && g_T4_AppData.HkTlm.obs == 1)
    {
        g_T4_AppData.HkTlm.health = 1;
        g_T4_AppData.HkTlm.obs = 0;
        T4_WISE_ParmCmd_t temp_parm;
        CFE_SB_InitMsg(&temp_parm, WISE_CMD_MID, sizeof(temp_parm), TRUE);
        CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&temp_parm, WISE_OBS_STOP_CC);
        CFE_SB_SendMsg((CFE_SB_Msg_t*)&temp_parm);
    }
    else if (getActiveCharge() > g_T4_AppData.HkTlm.critical_threshold)
    {
        g_T4_AppData.HkTlm.health = 0;
    }
    else
    {
        g_T4_AppData.HkTlm.health = 1;
    }

    T4_dischargeCaps();

}

void T4_dischargeCaps()
{
    if (g_T4_AppData.HkTlm.active_cap == 0 && wise_tlm.wiseActiveCap == 0)
    {
        if (wise_tlm.wiseCapB_Charge > 8500 && wise_tlm.wiseCapB_State < 2)
        {
            CFE_EVS_SendEvent(T4_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "ACT: A, DIS: B");
            T4_WISE_ParmCmd_t temp_parm;
            CFE_SB_InitMsg(&temp_parm, WISE_CMD_MID, sizeof(temp_parm), TRUE);
            temp_parm.target = 1;
            CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&temp_parm, WISE_CAP_DISCHARGE_CC);
            CFE_SB_SendMsg((CFE_SB_Msg_t*)&temp_parm);
        }
        if (wise_tlm.wiseCapC_Charge > 8500 && wise_tlm.wiseCapC_State < 2)
        {
            CFE_EVS_SendEvent(T4_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "ACT: A, DIS: C");
            T4_WISE_ParmCmd_t temp_parm;
            CFE_SB_InitMsg(&temp_parm, WISE_CMD_MID, sizeof(temp_parm), TRUE);
            temp_parm.target = 2;
            CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&temp_parm, WISE_CAP_DISCHARGE_CC);
            CFE_SB_SendMsg((CFE_SB_Msg_t*)&temp_parm);
        }
    }
    else if (g_T4_AppData.HkTlm.active_cap == 1 && wise_tlm.wiseActiveCap == 1)
    {
        if (wise_tlm.wiseCapA_Charge > 8500 && wise_tlm.wiseCapA_State < 2)
        {
            CFE_EVS_SendEvent(T4_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "ACT: B, DIS: A");
            T4_WISE_ParmCmd_t temp_parm;
            CFE_SB_InitMsg(&temp_parm, WISE_CMD_MID, sizeof(temp_parm), TRUE);
            temp_parm.target = 0;
            CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&temp_parm, WISE_CAP_DISCHARGE_CC);
            CFE_SB_SendMsg((CFE_SB_Msg_t*)&temp_parm);
        }
        else if (wise_tlm.wiseCapC_Charge > 8500 && wise_tlm.wiseCapC_State < 2)
        {
            CFE_EVS_SendEvent(T4_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "ACT: B, DIS: C");
            T4_WISE_ParmCmd_t temp_parm;
            CFE_SB_InitMsg(&temp_parm, WISE_CMD_MID, sizeof(temp_parm), TRUE);
            temp_parm.target = 2;
            CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&temp_parm, WISE_CAP_DISCHARGE_CC);
            CFE_SB_SendMsg((CFE_SB_Msg_t*)&temp_parm);
        }
    }
    else if (g_T4_AppData.HkTlm.active_cap == 2 && wise_tlm.wiseActiveCap == 2)
    {
        if (wise_tlm.wiseCapB_Charge > 8500 && wise_tlm.wiseCapB_State < 2)
        {
            CFE_EVS_SendEvent(T4_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "ACT: C, DIS: B");
            T4_WISE_ParmCmd_t temp_parm;
            CFE_SB_InitMsg(&temp_parm, WISE_CMD_MID, sizeof(temp_parm), TRUE);
            temp_parm.target = 1;
            CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&temp_parm, WISE_CAP_DISCHARGE_CC);
            CFE_SB_SendMsg((CFE_SB_Msg_t*)&temp_parm);
        }
        else if (wise_tlm.wiseCapA_Charge > 8500 && wise_tlm.wiseCapA_State < 2)
        {
            CFE_EVS_SendEvent(T4_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "ACT: C, DIS: A");
            T4_WISE_ParmCmd_t temp_parm;
            CFE_SB_InitMsg(&temp_parm, WISE_CMD_MID, sizeof(temp_parm), TRUE);
            temp_parm.target = 0;
            CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&temp_parm, WISE_CAP_DISCHARGE_CC);
            CFE_SB_SendMsg((CFE_SB_Msg_t*)&temp_parm);
        }
    }
}

uint16 getActiveCharge()
{
    if (g_T4_AppData.HkTlm.active_cap == 0)
    {
        return wise_tlm.wiseCapA_Charge;
    }
    else if (g_T4_AppData.HkTlm.active_cap == 1)
    {
        return wise_tlm.wiseCapB_Charge;
    }
    else
    {
        return wise_tlm.wiseCapC_Charge;
    }
}

int calcActiveCap()
{
    if (wise_tlm.wiseActiveCap == 0)
    {
        if (wise_tlm.wiseCapB_Charge > wise_tlm.wiseCapA_Charge)
        {
            if (wise_tlm.wiseCapC_Charge > wise_tlm.wiseCapB_Charge && 
                wise_tlm.wiseCapC_State < 2)
            {
                g_T4_AppData.HkTlm.active_cap = 2;
                return 2;
            }
            else if (wise_tlm.wiseCapB_State < 2)
            {
                g_T4_AppData.HkTlm.active_cap = 1;
                return 1;
            }
        }
        else if (wise_tlm.wiseCapC_Charge > wise_tlm.wiseCapA_Charge && 
                 wise_tlm.wiseCapC_State < 2)
        {
            g_T4_AppData.HkTlm.active_cap = 2;
            return 2;
        }
    }
    else if (wise_tlm.wiseActiveCap == 1)
    {
        if (wise_tlm.wiseCapA_Charge > wise_tlm.wiseCapB_Charge)
        {
            if (wise_tlm.wiseCapC_Charge > wise_tlm.wiseCapA_Charge && 
                wise_tlm.wiseCapC_State < 2)
            {
                g_T4_AppData.HkTlm.active_cap = 2;
                return 2;
            }
            else if (wise_tlm.wiseCapA_State < 2)
            {
                g_T4_AppData.HkTlm.active_cap = 0;
                return 0;
            }
        }
        else if (wise_tlm.wiseCapC_Charge > wise_tlm.wiseCapB_Charge && 
                 wise_tlm.wiseCapC_State < 2)
        {
            g_T4_AppData.HkTlm.active_cap = 2;
            return 2;
        }
    }
    else if (wise_tlm.wiseActiveCap == 2)
    {
        if (wise_tlm.wiseCapA_Charge > wise_tlm.wiseCapC_Charge)
        {
            if (wise_tlm.wiseCapB_Charge > wise_tlm.wiseCapA_Charge && 
                wise_tlm.wiseCapB_State < 2)
            {
                g_T4_AppData.HkTlm.active_cap = 1;
                return 1;
            }
            else if (wise_tlm.wiseCapA_State < 2)
            {
                g_T4_AppData.HkTlm.active_cap = 0;
                return 0;
            }
        }
        else if (wise_tlm.wiseCapB_Charge > wise_tlm.wiseCapC_Charge && 
                 wise_tlm.wiseCapB_State < 2)
        {
            g_T4_AppData.HkTlm.active_cap = 1;
            return 1;
        }
    }
    return -1;
}
    
/*=======================================================================================
** End of file t4_app.c
**=====================================================================================*/
    
