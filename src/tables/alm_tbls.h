/*******************************************************************************
Description    : 本文件中定义全部表格。
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLS_H__
#define __ALM_TBLS_H__

#include "alm_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#if defined(ALM_PACK_4)
#pragma pack (4)
#elif defined(ALM_PACK_8)
#pragma pack (8)
#endif

/* 定义全部表格索引 */
typedef enum
{
    /* 存储产品注册的BBOM及其状态 */
    ALM_TBL_IDX_PRD_BBOMS,

    /* 存储注册的产品及其状态 */
    ALM_TBL_IDX_PRD_INFO,

    /* 存储License文件内容对象地址 */
    ALM_TBL_IDX_LIC,

    /* 存储License状态信息 */
    ALM_TBL_IDX_LIC_STATE,

    /* 存储License Bbom状态信息 */
    ALM_TBL_IDX_LIC_BBOM_STATE,

    /* 存储状态校验规则 */
    ALM_TBL_IDX_STATE_RULE,

    /* 持久化非单个产品的公共数据 */
    ALM_TBL_IDX_PS_COMM,

    /* 持久化存储产品状态信息 */
    ALM_TBL_IDX_PS_PRD,

    /* 持久化存储License信息 */
    ALM_TBL_IDX_PS_LIC,

    /* 计数 */
    ALM_TBL_IDX_NUM
} ALM_TBL_IDX_ENUM;


/* 定义表格列索引 */
typedef enum
{
    /************************************************************************
    * PRD_INFO
    ************************************************************************/
    ALM_TBL_COL_IDX_PRD_INFO_START,

    /* 基础信息 */
    ALM_TBL_COL_IDX_PRD_INFO_ID,
    ALM_TBL_COL_IDX_PRD_INFO_NAME,
    ALM_TBL_COL_IDX_PRD_INFO_VER,

    /* 扩展信息*/
    ALM_TBL_COL_IDX_PRD_INFO_SAFETIMEABLE,
    ALM_TBL_COL_IDX_PRD_INFO_USE_MINVAL_PARTOVER,
    ALM_TBL_COL_IDX_PRD_INFO_FIX_TIME_SUPPORT,
    ALM_TBL_COL_IDX_PRD_INFO_ESNVERMIS_ONCE,

    ALM_TBL_COL_IDX_PRD_INFO_FORCE_CMP_SUPPORT,
    ALM_TBL_COL_IDX_PRD_INFO_DISABLE,
    ALM_TBL_COL_IDX_PRD_INFO_R5C01DLFTLKUSE,

    /* 状态段 */
    ALM_TBL_COL_IDX_PRD_INFO_STATE,
    ALM_TBL_COL_IDX_PRD_INFO_ENTER,
    ALM_TBL_COL_IDX_PRD_INFO_LEFT_MINS,
    ALM_TBL_COL_IDX_PRD_INFO_PERM,
    ALM_TBL_COL_IDX_PRD_INFO_STATE_LSN,
    ALM_TBL_COL_IDX_PRD_INFO_ANY_DEMO_RUN_MINS,
    ALM_TBL_COL_IDX_PRD_INFO_ANY_DEMO_TOTAL_MINS,

    /* 密钥段 */
    ALM_TBL_COL_IDX_PRD_INFO_KEY00,
    ALM_TBL_COL_IDX_PRD_INFO_KEY01,
    ALM_TBL_COL_IDX_PRD_INFO_KEY02,
    ALM_TBL_COL_IDX_PRD_INFO_KEY03,
    ALM_TBL_COL_IDX_PRD_INFO_KEY04,
    ALM_TBL_COL_IDX_PRD_INFO_KEY05,
    ALM_TBL_COL_IDX_PRD_INFO_KEY06,
    ALM_TBL_COL_IDX_PRD_INFO_KEY07,
    ALM_TBL_COL_IDX_PRD_INFO_KEY08,
    ALM_TBL_COL_IDX_PRD_INFO_KEY09,
    ALM_TBL_COL_IDX_PRD_INFO_KEY10,
    ALM_TBL_COL_IDX_PRD_INFO_KEY11,
    ALM_TBL_COL_IDX_PRD_INFO_KEY12,
    ALM_TBL_COL_IDX_PRD_INFO_KEY13,
    ALM_TBL_COL_IDX_PRD_INFO_KEY14,

    ALM_TBL_COL_IDX_PRD_INFO_END,

    /************************************************************************
    * PRD_BBOMS
    ************************************************************************/
    ALM_TBL_COL_IDX_PRD_BBOMS_START,

    /* 基础信息 */
    ALM_TBL_COL_IDX_PRD_BBOMS_PRD_ID ,
    ALM_TBL_COL_IDX_PRD_BBOMS_BBOM_ID,
    ALM_TBL_COL_IDX_PRD_BBOMS_BBOM_NAME,
    ALM_TBL_COL_IDX_PRD_BBOMS_TYPE,
    ALM_TBL_COL_IDX_PRD_BBOMS_DFLT_VAL,
    ALM_TBL_COL_IDX_PRD_BBOMS_MIN_VAL,
    ALM_TBL_COL_IDX_PRD_BBOMS_MAX_VAL,

    /* 状态段 */
    ALM_TBL_COL_IDX_PRD_BBOMS_LSN,
    ALM_TBL_COL_IDX_PRD_BBOMS_STATE,
    ALM_TBL_COL_IDX_PRD_BBOMS_ENTER,
    ALM_TBL_COL_IDX_PRD_BBOMS_LEFT_MINS,
    ALM_TBL_COL_IDX_PRD_BBOMS_IS_PERM,
    ALM_TBL_COL_IDX_PRD_BBOMS_TOTALVALUE,
    ALM_TBL_COL_IDX_PRD_BBOMS_PERVALUE,
    ALM_TBL_COL_IDX_PRD_BBOMS_TMPVALUE,

    /*紧急LIC BBOM 标识*/
    ALM_TBL_COL_IDX_PRD_BBOMS_NORMALEM,
    ALM_TBL_COL_IDX_PRD_BBOMS_DFTEM,

    ALM_TBL_COL_IDX_PRD_BBOMS_END,
    /************************************************************************
    * LIC
    ************************************************************************/
    ALM_TBL_COL_IDX_LIC_START,

    ALM_TBL_COL_IDX_LIC_LSN,
    ALM_TBL_COL_IDX_LIC_OBJ_ADDR,

    ALM_TBL_COL_IDX_LIC_END,
    /************************************************************************
    * LIC_STATE
    ************************************************************************/
    ALM_TBL_COL_IDX_LIC_STATE_START,

    ALM_TBL_COL_IDX_LIC_STATE_PRD_ID,
    ALM_TBL_COL_IDX_LIC_STATE_LSN,

    /* 使用记录 */
    ALM_TBL_COL_IDX_LIC_STATE_RECOVER_USING,
    ALM_TBL_COL_IDX_LIC_STATE_IS_USING,
    ALM_TBL_COL_IDX_LIC_STATE_FREE_USE_COUNT,
    ALM_TBL_COL_IDX_LIC_STATE_USE_COUNT,
    ALM_TBL_COL_IDX_LIC_STATE_FLASH_TIME_STAMP,

    /* 状态部分 */
    ALM_TBL_COL_IDX_LIC_STATE_STATE,
    ALM_TBL_COL_IDX_LIC_STATE_RSN,
    ALM_TBL_COL_IDX_LIC_STATE_TOTAL_MINS,
    ALM_TBL_COL_IDX_LIC_STATE_LEFT_MINS,
    ALM_TBL_COL_IDX_LIC_STATE_VALID_PASS_MINS,
    ALM_TBL_COL_IDX_LIC_STATE_GRACE_PASS_MINS,
    ALM_TBL_COL_IDX_LIC_STATE_IS_PERM,
    ALM_TBL_COL_IDX_LIC_STATE_ENTER_TIME,
    ALM_TBL_COL_IDX_LIC_STATE_RVK_TIME,
    ALM_TBL_COL_IDX_LIC_STATE_ENTER_TRIAL_TIME,

    /* 校验项部分 */
    ALM_TBL_COL_IDX_LIC_STATE_ESN_MIS,
    ALM_TBL_COL_IDX_LIC_STATE_PRD_NAME_MIS,
    ALM_TBL_COL_IDX_LIC_STATE_PRD_VER_MIS,
    ALM_TBL_COL_IDX_LIC_STATE_HAVE_RVKED,
    ALM_TBL_COL_IDX_LIC_STATE_VALID_OVER,
    ALM_TBL_COL_IDX_LIC_STATE_GRACE_OVER,

    /* 上一次刷新时保留的校验项及激活记录 */
    ALM_TBL_COL_IDX_LIC_STATE_LAST_IS_USING,
    ALM_TBL_COL_IDX_LIC_STATE_LAST_ESN_MIS,
    ALM_TBL_COL_IDX_LIC_STATE_LAST_PRD_NAME_MIS,
    ALM_TBL_COL_IDX_LIC_STATE_LAST_PRD_VER_MIS,
    ALM_TBL_COL_IDX_LIC_STATE_LAST_HAVE_RVKED,
    ALM_TBL_COL_IDX_LIC_STATE_LAST_VALID_OVER,
    ALM_TBL_COL_IDX_LIC_STATE_LAST_GRACE_OVER,

    /* 用于状态校验或激活校验的文件内容 */
    ALM_TBL_COL_IDX_LIC_STATE_LIC_TYPE,
    ALM_TBL_COL_IDX_LIC_STATE_LIC_DEADLINE,
    ALM_TBL_COL_IDX_LIC_STATE_LIC_REL_DAYS,
    ALM_TBL_COL_IDX_LIC_STATE_LIC_GRACE_DAYS,
    ALM_TBL_COL_IDX_LIC_STATE_LIC_ALLOW_COUNT,

    ALM_TBL_COL_IDX_LIC_STATE_END,
    /************************************************************************
    * LIC_BBOM_STATE
    ************************************************************************/
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_START,

    ALM_TBL_COL_IDX_LIC_BBOM_STATE_PRD_ID,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_LSN,

    ALM_TBL_COL_IDX_LIC_BBOM_STATE_BBOM_ID,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_SAME_NAME_ID,

    ALM_TBL_COL_IDX_LIC_BBOM_STATE_STATE,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_TOTAL_MINS,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_LEFT_MINS,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_IS_PERM,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_ENTER_TIME,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_VALUE,

    ALM_TBL_COL_IDX_LIC_BBOM_STATE_VALID_PASS_MINS,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_GRACE_PASS_MINS,
    ALM_TBL_COL_IDX_LIC_BBOM_STATE_RSN,

    ALM_TBL_COL_IDX_LIC_BBOM_STATE_END,

    /************************************************************************
    * STATE_RULE
    ************************************************************************/
    ALM_TBL_COL_IDX_STATE_RULE_START,

    ALM_TBL_COL_IDX_STATE_RULE_PRD_ID,
    ALM_TBL_COL_IDX_STATE_RULE_ITEM,
    ALM_TBL_COL_IDX_STATE_RULE_IGNORE,
    ALM_TBL_COL_IDX_STATE_RULE_IGNORE_UP_AND_LOWER,
    ALM_TBL_COL_IDX_STATE_RULE_MIS_STATE,
    ALM_TBL_COL_IDX_STATE_RULE_MIS_RECOVER_STATE,

    ALM_TBL_COL_IDX_STATE_RULE_END,

    /************************************************************************
     * 以下持久化表格在增加字段时必须增加到该表格最后。
    ************************************************************************/

    /************************************************************************
    * PS_COMM: 非单个产品的相关数据。
    ************************************************************************/
    ALM_TBL_COL_IDX_PS_COMM_START,

    ALM_TBL_COL_IDX_PS_COMM_SAFETIME_BASETIME,
    ALM_TBL_COL_IDX_PS_COMM_SAFETIME_RUNTIME,
    ALM_TBL_COL_IDX_PS_COMM_SAFETIME_CHANGETIME,

    ALM_TBL_COL_IDX_PS_COMM_END,

    /************************************************************************
    * PS_PRD
    ************************************************************************/
    ALM_TBL_COL_IDX_PS_PRD_START,

    ALM_TBL_COL_IDX_PS_PRD_TIME_ID,
    ALM_TBL_COL_IDX_PS_PRD_ID,
    ALM_TBL_COL_IDX_PS_PRD_VER,
    ALM_TBL_COL_IDX_PS_PRD_ANY_DEMO_RUN_MINS,
    ALM_TBL_COL_IDX_PS_PRD_ANY_DEMO_TOTAL_MINS,
    ALM_TBL_COL_IDX_PS_PRD_FIXED_TIME_LSN,
    ALM_TBL_COL_IDX_PS_PRD_HAVE_AVTIVED_ESNVERMIS,

    ALM_TBL_COL_IDX_PS_PRD_END,

    /************************************************************************
    * PS_LIC
    ************************************************************************/
    ALM_TBL_COL_IDX_PS_LIC_START,

    ALM_TBL_COL_IDX_PS_LIC_TIME_ID,
    ALM_TBL_COL_IDX_PS_LIC_PRD_ID,
    ALM_TBL_COL_IDX_PS_LIC_TYPE,
    ALM_TBL_COL_IDX_PS_LIC_LSN,

    /* 使用记录 */
    ALM_TBL_COL_IDX_PS_LIC_BE_USING,
    ALM_TBL_COL_IDX_PS_LIC_BE_RECOVER_USING,
    ALM_TBL_COL_IDX_PS_LIC_USED_COUNT,
    ALM_TBL_COL_IDX_PS_LIC_FREE_USED_COUNT,
    ALM_TBL_COL_IDX_PS_LIC_BE_RVKED,

    ALM_TBL_COL_IDX_PS_LIC_STATE,
    ALM_TBL_COL_IDX_PS_LIC_ENTER_TIME,
    ALM_TBL_COL_IDX_PS_LIC_IS_PERM,
    ALM_TBL_COL_IDX_PS_LIC_VALID_PASS_MINS,
    ALM_TBL_COL_IDX_PS_LIC_GRACE_PASS_MINS,
    ALM_TBL_COL_IDX_PS_LIC_RVK_TIME,
    ALM_TBL_COL_IDX_PS_LIC_ENTER_TRIAL_TIME,

    ALM_TBL_COL_IDX_PS_LIC_END

} ALM_TBL_COL_IDX_ENUM;

/* 定义表格列 */
typedef struct
{

    ALM_UINT32 uiNeedSize;
    ALM_TBL_COL_DIM_STRU stCol;
} ALM_TBL_COL_DIM_EX_STRU;

/* 定义表格 */
typedef struct
{
    ALM_ENUM(ALM_TBL_IDX_ENUM) eTblIdx;
    CONST ALM_CHAR* pcTblName;
    CONST ALM_TBL_COL_DIM_EX_STRU* pstColDim;
    ALM_UINT32 uiStartIdx;
    ALM_UINT32 uiColNum;
} ALM_TBL_DIM_STRU;

/* 初始化建立所有表格 */
ALM_INT32 ALM_TblsMgrCreateAll();

/* 删除所有表格 */
ALM_VOID ALM_TblsMgrDropAll();

/* 得到表格定义 */
CONST ALM_TBL_DIM_STRU* ALM_TblsGetDim(ALM_ENUM(ALM_TBL_IDX_ENUM) eTblIdx);

/* 得到列定义 */
CONST ALM_TBL_COL_DIM_EX_STRU* ALM_TblsGetColDim(ALM_ENUM(ALM_TBL_COL_IDX_ENUM) eCol);


/************************************************************************
* 工具宏，表格整理完成移至.C文件中
************************************************************************/

/* 将表格名简写转换为表格名 */
#define ALM_TBL_NAME(TBL) "ALM_"#TBL

#if (defined(ALM_PACK_4) || defined(ALM_PACK_8))
#pragma pack ()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLS_H__ */
