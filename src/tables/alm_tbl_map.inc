/*******************************************************************************
Description    : 表格映射对象，便于操作。
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBL_MAP_INC__
#define __ALM_TBL_MAP_INC__

#include "alm_tbl_map.h"

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

/* 将obj转换为条件，若有附加列先查找附加列作为条件，条件列优先使用附加列 */
/* pObj - 允许为空，可以再附加列中指定，也可以不指定条件 */
static ALM_INT32 ALMTblMapObjToCdt(CONST ALM_TBL_MAP_STRU* pstTblMap,
                                   CONST ALM_VOID* pObj,
                                   CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
                                   ALM_UINT32 uiExNum,
                                   INOUT ALM_TBL_CDT_STRU* pstCdt);

/* 将obj变动新转换为插入或更新数据行 */
/* pObjOld - 若为NULL，表示数据全部变动。 */
static ALM_INT32 ALMTblMapObjChangeToSRow(CONST ALM_TBL_MAP_STRU* pstTblMap,
        CONST ALM_VOID* pObjOld,
        CONST ALM_VOID* pObjNew,
        INOUT ALM_TBL_SROW_STRU* pstRow);

/* 将涉及列添加到查询结果中 */
static ALM_INT32 ALMTblMapAddColToRslt(CONST ALM_TBL_MAP_STRU* pstTblMap,
                                       INOUT ALM_TBL_RSLT_STRU* pstRslt);

/* 将查询结果转换为obj */
static ALM_INT32 ALMTblMapRsltToObj(CONST ALM_TBL_MAP_STRU* pstTblMap,
                                    OUT ALM_VOID* pObj,
                                    CONST ALM_TBL_RSLT_STRU* pstRslt,
                                    CONST ALM_VOID* pRsltRow);

/* 得到映射列 */
static CONST ALM_TBL_MAP_COL_STRU* ALMTblMapGetCol(
    CONST ALM_TBL_MAP_STRU* pstTblMap,
    ALM_ENUM(ALM_TBL_COL_IDX_ENUM) eCol);

/* 得到附加列 */
static CONST ALM_TBL_MAP_COL_EX_STRU* ALMTblMapExGetCol(
    CONST ALM_TBL_MAP_COL_EX_STRU* pstExs,
    ALM_UINT32 uiExNum,
    ALM_ENUM(ALM_TBL_COL_IDX_ENUM) eCol);

#if (defined(ALM_PACK_4) || defined(ALM_PACK_8))
#pragma pack ()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBL_MAP_INC__ */
