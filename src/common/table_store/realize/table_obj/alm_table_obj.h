/*******************************************************************************
Description    : 表格对象操作。
FileName       : alm_table_obj.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLE_OBJ_H__
#define __ALM_TBLE_OBJ_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#include "alm_table_cols.h"
#include "alm_table_rows.h"
#include "alm_table_tool.h"


#if defined(ALM_PACK_4)
#pragma pack (4)
#elif defined(ALM_PACK_8)
#pragma pack (8)
#endif

/**
* 描述: 存储表格定义及内容。
* 成员:
* 1. acTblName - 表名，临时表格允许为空。
* 2. stCols - 存储表格列定义。
* 3. stRows - 存储表格全部数据行。
*/
typedef struct
{
    ALM_CHAR acTblName[ALM_CONST_NAME_MAX_LEN + 1];
    ALM_TBL_COLS_STRU stCols;
    ALM_TBL_ROWS_STRU stRows;
} ALM_TBL_OBJ_STRU;

/* 初始化一个表格 */
ALM_INT32 ALM_TblObjInit(OUT ALM_TBL_OBJ_STRU* pstTbl,
                         CONST ALM_CHAR* pcName,
                         ALM_UINT32 uiColNum,
                         CONST ALM_TBL_COL_DIM_STRU* pstColsDim);

/* 清理表格，清理后应重新初始化才能使用 */
ALM_VOID ALM_StClsTblObj(INOUT ALM_TBL_OBJ_STRU* pstTbl);

/**
* 描述: 向表格中插入数据。对以未指定的列采用默认值，若不存在默认值使用0填充。
* 参数:
* 1. pstTbl - 待插入数据的表格。
* 2. pstNewRow - 指定新插入数据行中数据列及列对应的值。
*/
ALM_INT32 ALM_TblObjInsert(INOUT ALM_TBL_OBJ_STRU* pstTbl,
                           CONST ALM_TBL_SROW_STRU* pstNewRow);

/**
* 描述: 删除表格中符合要求的数据行。
* 参数:
* 1. pstTbl - 待删除数据的表格。
* 2. pstCdt - 待删除的数据行应符合的条件，若传入NULL删除全部数据。
*/
ALM_INT32 ALM_TblObjDel(INOUT ALM_TBL_OBJ_STRU* pstTbl,
                        CONST ALM_TBL_CDT_STRU* pstCdt);

/**
* 描述: 更新表格中数据行。
* 参数:
* 1. pstTbl - 待更新数据的表格。
* 2. pstCdt - 需要更新数据的数据行应满足的条件，若传入NULL更新全部数据。
* 3. pstNewVal - 指定待更新的列及对应的新值。
*/
ALM_INT32 ALM_TblObjUpdate(INOUT ALM_TBL_OBJ_STRU* pstTbl,
                           CONST ALM_TBL_CDT_STRU* pstCdt,
                           CONST ALM_TBL_SROW_STRU* pstNewVal);

/**
* 描述: 查询表格中数据行。调用该函数后应外部清理查询结果。
* 参数:
* 1. pstTbl - 待更新数据的表格。
* 2. pstCdt - 所得数据行应满足的条件。
* 3. pstRslt - 查询得到的全部数据行，会得到指定列的对应的数据值，若没指定列得到全部列的值。
*/
ALM_INT32 ALM_TblObjQuery(CONST ALM_TBL_OBJ_STRU* pstTbl,
                          CONST ALM_TBL_CDT_STRU* pstCdt,
                          INOUT ALM_TBL_RSLT_STRU* pstRslt);

/* 得到表格中总数据行数。*/
ALM_INT32 ALM_TblObjTotal(CONST ALM_TBL_OBJ_STRU* pstTbl, ALM_UINT32* puiRowTotal);


#if (defined(ALM_PACK_4) || defined(ALM_PACK_8))
#pragma pack ()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_OBJ_H__ */
