/*******************************************************************************
Description    : 定义表格列信息。
FileName       : alm_talbe_cols.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLE_COLS_INC__
#define __ALM_TBLE_COLS_INC__

#include "alm_table_cols.h"
#include "alm_table_tool.h"
#include "alm_store_tree.h"
#include <string.h>

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

/* 一个表格中最多允许多少个索引列 */
#define ALM_TBL_COL_INDEX_MAX_NUM 8

/* 描述: 初始化单个列，不再使用应使用函数ALMStClsTblCol清理。可以不指定默认值。*/
static ALM_INT32 ALMTblColInit(OUT ALM_TBL_COL_STRU* pstCol,
                               CONST ALM_TBL_COL_DIM_STRU* pstColsDim);

/* 清理已初始的列，清理后应重新初始化才能使用 */
static ALM_VOID ALMStClsTblCol(INOUT ALM_TBL_COL_STRU* pstCol);

#if (defined(ALM_PACK_4) || defined(ALM_PACK_8))
#pragma pack ()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_COLS_INC__ */
