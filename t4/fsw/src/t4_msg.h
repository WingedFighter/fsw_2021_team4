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
#define T4_SET_ACTIVE_CAP_CC       2
#define T4_SET_HEALTH_CC           3
#define T4_SET_OBS_TLD_CC          4
#define T4_SET_LVR_TLD_CC          5
#define T4_SET_CRT_TLD_CC          6
#define T4_SET_HEAT_TLD_CC         7
#define T4_SET_VERBOSITY_CC        8
#define T4_SET_OBS_STATE_CC        9
#define T4_SET_LVR_STATE_CC        10
#define T4_SET_HEAT_STATE_CC       11
#define T4_DISCHARGE_CAP_CC        12
#define T4_REBOOT_CC               13
#define T4_GET_OBS_AVAIL_CC        14
#define WISE_CAP_ACTIVE_CC         1
#define WISE_CAP_DISCHARGE_CC	   2
#define WISE_HTR_TOGGLE_CC	   3
#define WISE_LVR_TOGGLE_CC         4
#define WISE_OBS_START_CC          5
#define WISE_OBS_STOP_CC           6
#define WISE_POWER_SBC_CC          7
#define WISE_SEND_OBS_CC           8
#define WISE_TLM_RESET_CNTS_CC     9

/*
** Local Structure Declarations
*/
typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              usCmdCnt;
    uint8              usCmdErrCnt;

    /* TODO:  Add declarations for additional housekeeping data here */
    uint16             health;
    uint16             louver;
    uint16             obs;
    uint16             heat;
    uint16             active_cap;
    uint16             heat_threshold;
    uint16             critical_threshold;
    uint16             obs_threshold;
    uint16             louver_threshold;
    uint16             health_threshold;

} T4_HkTlm_t;

typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              usCmdCnt;
    uint8              usCmdErrCnt;

    uint16             wiseSbcState;
    uint16             wiseDamage;
    uint16             wiseActiveCap;
    uint16             wiseCapA_State;
    uint16             wiseCapA_Charge;
    uint16             wiseCapB_State;
    uint16             wiseCapB_Charge;
    uint16             wiseCapC_State;
    uint16             wiseCapC_Charge;
    uint16             wiseHtrA_State;
    uint16             wiseHtrB_State;
    uint16             wiseLvrA_State;
    uint16             wiseLvrB_State;
    uint16             wiseSSR_Count;
    int16              wiseTemp;
} T4_Wise_Tlm_t;

typedef struct
{
    uint8              CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16             target;
} T4_ParmCmd_t;

typedef struct
{
    uint8              CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16             target;
} T4_WISE_ParmCmd_t;

#endif /* _T4_MSG_H_ */

/*=======================================================================================
** End of file t4_msg.h
**=====================================================================================*/
    
