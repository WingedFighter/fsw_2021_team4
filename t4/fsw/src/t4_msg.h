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
#define T4_SET_CAP_STATE_CC        2
#define T4_SET_ACTIVE_CAP_CC       3
#define T4_SET_HEALTH_CC           4
#define T4_SET_OBS_TLD_CC          5
#define T4_SET_LVR_TLD_CC          6
#define T4_SET_CRT_TLD_CC          7
#define T4_SET_HEAT_TLD_CC         8
#define T4_SET_VERBOSITY_CC        9
#define T4_SET_OBS_STATE_CC        10
#define T4_SET_LVR_STATE_CC        11
#define T4_SET_HEAT_STATE_CC       12
#define T4_GET_CAP_CHARGE_CC       13
#define T4_GET_OBS_STATE_CC        14
#define T4_GET_LVR_STATE_CC        15
#define T4_GET_HEAT_STATE_CC       16

/*
** Local Structure Declarations
*/
typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              usCmdCnt;
    uint8              usCmdErrCnt;

    /* TODO:  Add declarations for additional housekeeping data here */
    uint8              cap_charge;
    uint8              cap_state;
    uint8              health;
    uint8              louver;
    uint8              obs;
    uint8              heat;
    uint8              active_cap;
    uint8              heat_threshold;
    uint8              critical_threshold;
    uint8              obs_threshold;
    uint8              louver_threshold;
    uint8              health_threshold;

} T4_HkTlm_t;

typedef struct
{
    uint8              CmdHeader[CFE_SB_CMD_HDR_SIZE];

    uint8              cap_state;
} T4_CapState_t;

typedef struct
{
    uint8              CmdHeader[CFE_SB_CMD_HDR_SIZE];

    uint8              health;
} T4_Health_t;

typedef struct
{
    uint8              CmdHeader[CFE_SB_CMD_HDR_SIZE];

    uint8              heat_threshold;
} T4_HeatThreshold_t;

typedef struct
{
    uint8              CmdHeader[CFE_SB_CMD_HDR_SIZE];

    uint8              critical_threshold;
} T4_CriticalThreshold_t;

typedef struct
{
    uint8              CmdHeader[CFE_SB_CMD_HDR_SIZE];

    uint8              active_cap;
} T4_ActiveCap_t;

typedef struct
{
    uint8              CmdHeader[CFE_SB_CMD_HDR_SIZE];

    uint8              obs_threshold;
} T4_ObsThreshold_t;

typedef struct
{
    uint8              CmdHeader[CFE_SB_CMD_HDR_SIZE];

    uint8              louver_threshold;
} T4_LouverThreshold_t;

typedef struct
{
    uint8              CmdHeader[CFE_SB_CMD_HDR_SIZE];

    uint8              health_threshold;
} T4_HealthThreshold_t;

typedef struct
{
    uint8              CmdHeader[CFE_SB_CMD_HDR_SIZE];

    uint8              obs;
} T4_ObsState_t;

typedef struct
{
    uint8              CmdHeader[CFE_SB_CMD_HDR_SIZE];

    uint8              louver;
} T4_LouverState_t;

typedef struct
{
    uint8              CmdHeader[CFE_SB_CMD_HDR_SIZE];

    uint8              heat;
} T4_HeatState_t;

#endif /* _T4_MSG_H_ */

/*=======================================================================================
** End of file t4_msg.h
**=====================================================================================*/
    
