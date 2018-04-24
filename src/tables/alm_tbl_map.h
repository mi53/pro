/*******************************************************************************
Description    : 表格映射对象，便于操作。
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBL_MAP_H__
#define __ALM_TBL_MAP_H__

#include "alm_tbls.h"

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

/* 涉及列所在对象位置 */
typedef struct
{
    ALM_ENUM(ALM_TBL_COL_IDX_ENUM) eCol;
    ALM_UINT32 uiOffset;
    ALM_UINT32 uiSize;
} ALM_TBL_MAP_COL_STRU;

/* 用于初始化MAP列的工具宏 */
#define ALM_TBL_MAP_COL(idx, type, member) {\
        idx, \
        ALM_TOOL_STRU_MEBR_OFFSET(type, member), \
        ALM_TOOL_STRU_MEBR_SIZE(type, member)}

/* 保存多个映射列 */
typedef struct
{
    ALM_BOOL bNeedCheck;
    CONST ALM_TBL_MAP_COL_STRU* pstArray;
    ALM_UINT32 uiNum;
} ALM_TBL_MAP_COLS_STRU;


/*
* 表格对象关联到表格，由于映射列只会关联到单个结构体，可能难于满足全部列映射，
* 操作时，允许指定附加列，附加列只做条件列，且条件列优先使用附加列。
* 附加列不再该结构中展现，在对应的操作函数中指定。
*/
typedef struct
{
    /* 关联表格 */
    ALM_ENUM(ALM_TBL_IDX_ENUM) eTbl;

    /* 关联对象大小 */
    ALM_UINT32 uiObjSize;

    /* 列映射 */
    ALM_TBL_MAP_COLS_STRU* pstColMaps;

    /* 条件列，若未指定不限制条件 */
    CONST ALM_ENUM(ALM_TBL_COL_IDX_ENUM)* pstCdtCols;
    ALM_UINT32 uiCdtColNum;

    /* 当前涉及列，若为NULL，默认涉及全部已映射列 */
    /* 当前操作的列除条件列外均只操作涉及列 */
    CONST ALM_ENUM(ALM_TBL_COL_IDX_ENUM)* pstCols;
    ALM_UINT32 uiColNum;

} ALM_TBL_MAP_STRU;

/* 存储查询结果 */
typedef struct
{
    ALM_VOID* pstArray;
    ALM_UINT32 uiNum;
    ALM_UINT32 uiCap;
} ALM_TBL_MAP_RSLT_STRU;

/* 附加列映射，优先从附加列中获取数据 */
typedef struct
{
    ALM_ENUM(ALM_TBL_COL_IDX_ENUM) eCol;
    CONST ALM_VOID* pData;
    ALM_UINT32 uiSize;
} ALM_TBL_MAP_COL_EX_STRU;


/* 清理ALM_TBL_MAP_RSLT_STRU对象 */
ALM_VOID ALM_StClsTblMapRslt(INOUT ALM_TBL_MAP_RSLT_STRU* pstRslt);

/* 校验映射对象的有效性 */
ALM_INT32 ALM_TblMapCheck(CONST ALM_TBL_MAP_STRU* pstTblMap);

/* 使用映射对象实现添加数据 */
ALM_INT32 ALM_TblMapAdd(CONST ALM_TBL_MAP_STRU* pstTblMap,
                        CONST ALM_VOID* pObj,
                        CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
                        ALM_UINT32 uiExNum);

/* 使用映射对象实现删除数据，附加列只作条件 */
/* pObjCdt - 若为NULL，不限制条件 */
ALM_INT32 ALM_TblMapRm(CONST ALM_TBL_MAP_STRU* pstTblMap,
                       CONST ALM_VOID* pObjCdt,
                       CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
                       ALM_UINT32 uiExNum);

/* 使用映射对象实现更新，附加列只作条件，将pObjNew作为条件关联对象 */
ALM_INT32 ALM_TblMapUpdate(CONST ALM_TBL_MAP_STRU* pstTblMap,
                           CONST ALM_VOID* pObjNew,
                           CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
                           ALM_UINT32 uiExNum);

/* 使用映射对象实现查询，附加列只作条件，obj中其他数据用0填充，注意清理输出 */
/* pObjCdt - 若为NULL，不限制条件 */
ALM_INT32 ALM_TblMapQuery(CONST ALM_TBL_MAP_STRU* pstTblMap,
                          CONST ALM_VOID* pObjCdt,
                          CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
                          ALM_UINT32 uiExNum,
                          OUT ALM_TBL_MAP_RSLT_STRU* pstRslt);

/*
* 使用映射对象实现更新，将pObjOld作为条件关联对象，附加列只作条件，
* 条件列优先使用附加列。
* pbChange - 若不为NULL，将得到是否存在数据变更的信息。
*/
ALM_INT32 ALM_TblMapUpdateByChange(CONST ALM_TBL_MAP_STRU* pstTblMap,
                                   CONST ALM_VOID* pObjOld,
                                   CONST ALM_VOID* pObjNew,
                                   CONST ALM_TBL_MAP_COL_EX_STRU* pstColEx,
                                   ALM_UINT32 uiExNum,
                                   OUT ALM_BOOL* pbChange);

#if (defined(ALM_PACK_4) || defined(ALM_PACK_8))
#pragma pack ()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBL_MAP_H__ */
