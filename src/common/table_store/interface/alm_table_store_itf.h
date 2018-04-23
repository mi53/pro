/*******************************************************************************
Description    : 表格存储模块对外文件。
FileName       : alm_table_store.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLE_STORE_ITF_H__
#define __ALM_TBLE_STORE_ITF_H__

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

/* 创建表格，不能重名 */
ALM_INT32 ALM_TblNew(CONST ALM_CHAR* pcTblName,
                     ALM_UINT32 uiColNum,
                     CONST ALM_TBL_COL_DIM_STRU* pstColsDim);

/* 删除表格 */
ALM_VOID ALM_TblDel(CONST ALM_CHAR* pcTblName);

/**
* 描述: 向表格中插入数据。对以未指定的列采用默认值，若不存在默认值使用0填充。
* 参数:
* 1. pstTbl - 待插入数据的表格。
* 2. pstNewRow - 指定新插入数据行中数据列及列对应的值。
*/
ALM_INT32 ALM_TblInsert(CONST ALM_CHAR* pcTbl,
                        CONST ALM_TBL_SROW_STRU* pstNewRow);

/**
* 描述: 删除表格中符号要求的数据行。
* 参数:
* 1. pstTbl - 待删除数据的表格。
* 2. pstCdt - 待删除的数据行应符合的条件，若传入NULL删除全部数据。
*/
ALM_INT32 ALM_TblRmRows(CONST ALM_CHAR* pcTbl, CONST ALM_TBL_CDT_STRU* pstCdt);

/**
* 描述: 更新表格中数据行。
* 参数:
* 1. pstTbl - 待更新数据的表格。
* 2. pstCdt - 需要更新数据的数据行应满足的条件。
* 3. pstNewVal - 指定待更新的列及对应的新值。
*/
ALM_INT32 ALM_TblUpdate(CONST ALM_CHAR* pcTbl,
                        CONST ALM_TBL_CDT_STRU* pstCdt,
                        CONST ALM_TBL_SROW_STRU* pstNewVal);

/**
* 描述: 更新表格中数据行。
* 参数:
* 1. pstTbl - 待更新数据的表格。
* 2. pstCdt - 需要更新数据的数据行应满足的条件。
* 3. pstNewVal - 指定待更新的列及对应的新值。
*/
ALM_INT32 ALM_TblUpdateByColsVal(CONST ALM_CHAR* pstTbl,
                                 CONST ALM_TBL_CDT_STRU* pstCdt,
                                 CONST ALM_TBL_SROW_STRU* pstNewVal);

/**
* 描述: 查询表格中数据行。调用该函数后应清理查询结果。
* 参数:
* 1. pstTbl - 待更新数据的表格。
* 2. pstCdt - 所得数据行应满足的条件。
* 3. pstRslt - 查询得到的全部数据行，可以指定要查询的列。
*/
ALM_INT32 ALM_TblQuery(CONST ALM_CHAR* pcTbl,
                       CONST ALM_TBL_CDT_STRU* pstCdt,
                       INOUT ALM_TBL_RSLT_STRU* pstRslt);

/* 得到表格中数据总行数。*/
ALM_INT32 ALM_TblTotal(CONST ALM_CHAR* pcTbl, ALM_UINT32* puiRowCount);

/* 删除全部表格 */
ALM_VOID ALM_TblDropAll();

/* 清除全部表格内容 */
ALM_VOID ALM_TblClsAll();

#if (defined(ALM_PACK_4) || defined(ALM_PACK_8))
#pragma pack ()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_STORE_ITF_H__ */


