/*******************************************************************************
Description    : 定义表格列信息。
FileName       : alm_talbe_cols.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLE_COLS_H__
#define __ALM_TBLE_COLS_H__

#include "alm_table_tool_itf.h"

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

/**
* 描述: 存储表格单列。
* 成员:
* eIdx      -表格列索引。
* acColName - 列名。
* eDataType - 该列存储的数据对应的数据类型。
* bExistDflt- 是否存在默认值，TRUE: 存在。标记成员stDfltVal的有效性。
* stDfltVal - 列默认值，插入数据行若未指定列值使用默认值。可以为NULL。
* bNeedIndex - 是否需要建立索引。
* uiIndexId - 索引编号。
*/
typedef struct
{
    ALM_UINT32 eIdx;
    ALM_CHAR acColName[ALM_CONST_NAME_MAX_LEN + 1];
    ALM_ENUM(ALM_TBL_DATA_TYPE_ENUM) eDataType;
    ALM_BOOL bExistDflt;
    ALM_TBL_DATA_STRU stDfltVal;
    ALM_BOOL bNeedIndex;
    ALM_UINT32 uiIndexId;
} ALM_TBL_COL_STRU;


/* 定义表格全部列 */
typedef struct
{
    ALM_TBL_COL_STRU* pstCols;
    ALM_UINT32 uiColNum;

    /* 保存用于端口校验的端口码:索引转换为2bit一列的数值码 */
    ALM_UINT16* pusPortCode;
    ALM_UINT16 usPortNum;

    /* 索引列计数，不允许超过8列 */
    ALM_UINT8 ubIndexColNum;
} ALM_TBL_COLS_STRU;

/* 初始化全部列定义，不再使用应使用函数ALM_TblColsCls清理。 */
ALM_INT32 ALM_TblColsInit(OUT ALM_TBL_COLS_STRU* pstCols);

/* 清理已初始的列，清理后应重新初始化才能使用 */
ALM_VOID ALMStClsTblCols(INOUT ALM_TBL_COLS_STRU* pstCols);

/* 增加列 */
ALM_INT32 ALM_TblColsAddCol(INOUT ALM_TBL_COLS_STRU* pstCols,
                            ALM_UINT32 uiColNum,
                            CONST ALM_TBL_COL_DIM_STRU* pstColsDim);

/**
* 描述:获取列索引。
* 参数:
* 1. pstCols - 列存储对象。
* 2. pcColName - 待获取的列面。
* 3. piColIdx - 得到的对应列的索引。列不存在得到负值。
*/
ALM_INT32 ALM_TblColsGetColIdx(CONST ALM_TBL_COLS_STRU* pstCols,
                               CONST ALM_CHAR* pcColName,
                               OUT ALM_INT32* piColIdx);


#if (defined(ALM_PACK_4) || defined(ALM_PACK_8))
#pragma pack ()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_COLS_H__ */
