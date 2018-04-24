/*******************************************************************************
Description    : 定义表格数据行。
FileName       : alm_talbe_rows.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLE_ROWS_H__
#define __ALM_TBLE_ROWS_H__

#include "alm_store_tree.h"
#include "alm_table_row.h"
#include "alm_table_tool.h"

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
* 描述: 用于存储表格数据行。
* 成员:
* 1. pstCols - 对应的表格列信息；使用外部定义的cols.
* 2. stRows - 全部数据行。
*/
typedef struct
{
    CONST ALM_TBL_COLS_STRU* pstCols;
    ALM_STORE_TREE_STRU_T(ALM_TBL_ROW_STRU) stRows;
} ALM_TBL_ROWS_STRU;

/* 初始化表格数据行对象，应使用函数ALM_StClsTblRows清理 */
ALM_INT32 ALM_TblRowsInit(OUT ALM_TBL_ROWS_STRU* pstRows,
                          CONST ALM_TBL_COLS_STRU* pstCols);

/* 清理已初始化的数据行，清理后应重新初始化再使用 */
ALM_VOID ALM_StClsTblRows(INOUT ALM_TBL_ROWS_STRU* pstRows);

/**
* 描述: 向rows对象中插入数据。对以未指定的列采用默认值，若不存在默认
* 值使用0填充。
* 参数:
* 1. pstRows - 待插入数据的rows对象。
* 2. pstNewRow - 指定新插入数据行中数据列及列对应的值。
*/
ALM_INT32 ALM_TblRowsInsert(INOUT ALM_TBL_ROWS_STRU* pstRows,
                            CONST ALM_TBL_SROW_STRU* pstNewRow);

/**
* 描述: 删除rows对象中符合要求的数据行。
* 参数:
* 1. pstRows - 待删除数据的rows对象。
* 2. pstCdt - 待删除的数据行应符合的条件，若传入NULL删除全部数据。
*/
ALM_INT32 ALM_TblRowsDel(INOUT ALM_TBL_ROWS_STRU* pstRows,
                         CONST ALM_TBL_CDT_STRU* pstCdt);

/**
* 描述: 更新rows对象中数据行。
* 参数:
* 1. pstRows - 待更新数据的rows对象。
* 2. pstCdt - 需要更新数据的数据行应满足的条件，若传入NULL更新全部数据。
* 3. pstNewVal - 指定待更新的列及对应的新值。
*/
ALM_INT32 ALM_TblRowsUpdate(INOUT ALM_TBL_ROWS_STRU* pstRows,
                            CONST ALM_TBL_CDT_STRU* pstCdt,
                            CONST ALM_TBL_SROW_STRU* pstNewVal);

/**
* 描述: 查询rows对象中数据行。调用该函数后应外部清理查询结果。
* 参数:
* 1. pstRows - 待更新数据的表格。
* 2. pstCdt - 所得数据行应满足的条件，若为NULL则无条件限制。
* 3. pstRslt - 查询得到的全部数据行，会得到指定列的对应的数据值，若没指定列得到全部列的值。
*/
ALM_INT32 ALM_TblRowsQuery(CONST ALM_TBL_ROWS_STRU* pstRows,
                           CONST ALM_TBL_CDT_STRU* pstCdt,
                           INOUT ALM_TBL_RSLT_STRU* pstRslt);

/* 得到表格中总数据行数。*/
ALM_INT32 ALM_TblRowsTotal(CONST ALM_TBL_ROWS_STRU* pstRows,
                           ALM_UINT32* puiRowTotal);

#if (defined(ALM_PACK_4) || defined(ALM_PACK_8))
#pragma pack ()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_ROWS_H__ */
