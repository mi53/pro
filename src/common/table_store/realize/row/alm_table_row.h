/*******************************************************************************
Description    : 定义row对象模块内可见结构及函数。
FileName       : alm_talbe_row_in.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLE_ROW_H__
#define __ALM_TBLE_ROW_H__

#include "alm_table_cols.h"

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

/* 表格单个数据行 */
typedef struct
{
    ALM_UINT32 uiSize;
    ALM_VOID* pData;
} ALM_TBL_ROW_STRU;

/* 依据数据列初始化数据行 */
ALM_INT32 ALM_TblRowInit(ALM_TBL_ROW_STRU* pstRow, CONST ALM_TBL_COLS_STRU* pstCols);

/* 清理已初始化的数据行，清理后应重新初始化再使用 */
ALM_VOID ALM_StClsTblRow(ALM_TBL_ROW_STRU* pstRow);


/**
* 描述: 从数据行中获取某一列的值。
* 参数:
* 1. pstRow - 获取数据的行。
* 2. pstCols - 与指定行对应的列信息。
* 3. pcColName - 需要获取的列。
* 4. pstData - 得到的数据拷贝。
* 返回值:
* 1. ALM_OK - 成功。
* 2. 其他 - 失败返回相应的错误码。
*/
ALM_INT32 ALM_TblRowGetColVal(CONST ALM_TBL_ROW_STRU* pstRow,
                              CONST ALM_TBL_COLS_STRU* pstCols,
                              CONST ALM_CHAR* pcColName,
                              INOUT ALM_TBL_DATA_STRU* pstData);

/* 更新数据行中某一列的值。 */
ALM_INT32 ALM_TblRowUpdate(INOUT ALM_TBL_ROW_STRU* pstRow,
                           CONST ALM_TBL_COLS_STRU* pstCols,
                           CONST ALM_CHAR* pcColName,
                           CONST ALM_TBL_DATA_STRU* pstData);

/* 更新数据行中某一列的值。 */
ALM_INT32 ALM_TblRowUpdateByIdx(INOUT ALM_TBL_ROW_STRU* pstRow,
                                CONST ALM_TBL_COLS_STRU* pstCols,
                                ALM_UINT32 uiColIdx,
                                CONST ALM_TBL_DATA_STRU* pstData);

/* 使用部分数据初始化数据行 */
ALM_INT32 ALM_TblRowInitByPartData(OUT ALM_TBL_ROW_STRU* pstRow,
                                   CONST ALM_TBL_COLS_STRU* pstCols,
                                   CONST ALM_TBL_COLS_STRU* pstPartCols,
                                   CONST ALM_TBL_ROW_STRU* pstPartData);

/* 数据行中获取指定列对应的内存起始位及大小 */
ALM_INT32 ALM_TblRowGetColData(CONST ALM_TBL_ROW_STRU* pstRow,
                               CONST ALM_TBL_COLS_STRU* pstCols,
                               ALM_UINT32 uiColIdx,
                               OUT ALM_UINT32* puiSize,
                               OUT ALM_VOID** ppData);


#if (defined(ALM_PACK_4) || defined(ALM_PACK_8))
#pragma pack ()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_ROW_H__ */
