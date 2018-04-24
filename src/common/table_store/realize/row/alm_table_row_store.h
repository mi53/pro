/*******************************************************************************
Description    : rows中存储row需要指定的一些函数。
FileName       : alm_talbe_row_store.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLE_ROW_STORE_H__
#define __ALM_TBLE_ROW_STORE_H__

#include "alm_table_row.h"
#include "alm_store_tree.h"

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

/* 初始化ROW对象 */
ALM_INT32 ALM_TblRowStoreInit(OUT ALM_TBL_ROW_STRU* pstRow,
                              CONST ALM_TBL_COLS_STRU* pstCols);

/* 清理ROW对象 */
ALM_VOID ALM_TblRowStoreCls(INOUT ALM_TBL_ROW_STRU* pstRow,
                            CONST ALM_TBL_COLS_STRU* pstCols);

/* Row拷贝 */
ALM_INT32 ALM_TblRowStoreCpy(OUT ALM_TBL_ROW_STRU* pstDestRow,
                             CONST ALM_TBL_ROW_STRU* pstSrcRow,
                             CONST ALM_TBL_COLS_STRU* pstCols);

/*
* 描述: 用于比较指定数据是否相同，并得到下层需要迭代的端口号。得到的端口号必须小于端口总数目。
* pCmpL - 比较左数，一般为mark或数据。
* bFindByMark - 比较左数为mark还是数据。
* pstRow - 比较右数。
* pstRslt - 得到的比较结果。
* pEx - 初始化存储对象时指定的扩展成员。
*/
ALM_INT32 ALM_TblRowStoreGetPort(CONST ALM_VOID* pCmpL,
                                 ALM_BOOL bFindByMark,
                                 CONST ALM_TBL_ROW_STRU* pstRow,
                                 INOUT ALM_STORE_TREE_CMP_RSLT_STRU* pstRslt,
                                 CONST ALM_TBL_COLS_STRU* pstCols);

#if (defined(ALM_PACK_4) || defined(ALM_PACK_8))
#pragma pack ()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_ROW_STORE_H__ */
