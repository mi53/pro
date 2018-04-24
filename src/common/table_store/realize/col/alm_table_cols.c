/*******************************************************************************
Description    : 定义表格列信息。
FileName       : alm_talbe_col.c
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLE_COL_H__
#define __ALM_TBLE_COL_H__

#include "alm_table_cols.inc"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */


/* 描述: 初始化单个列，不再使用应使用函数ALMStClsTblCol清理。可以不指定默认值。*/
static ALM_INT32 ALMTblColInit(OUT ALM_TBL_COL_STRU* pstCol,
                               CONST ALM_TBL_COL_DIM_STRU* pstColDim)
{
    ALM_UINT32 uiColNameLen;
    ALM_INT32 iRet;

    ALM_ChkRet_M(pstCol != ALM_NULL && pstColDim != ALM_NULL,
                 ALM_ERR_NULL_PTR,
                 (ALM_NULL));

    ALM_OsMemset0_M(pstCol, ALM_TBL_COL_STRU, 1);

    /*保存列索引*/
    pstCol->eIdx = pstColDim->eIdx;

    /* 保存col name */
    uiColNameLen = ALM_ToolStrLen(pstColDim->pcColName, sizeof(pstCol->acColName));

    ALM_ChkRet_M(uiColNameLen > 0 && uiColNameLen < sizeof(pstCol->acColName),
                 ALM_ERR_INVALID_PARA,
                 ("Col name len is invalid."));

    (ALM_VOID)ALM_strncpy_s(pstCol->acColName,
                            sizeof(pstCol->acColName) - 1,
                            pstColDim->pcColName,
                            uiColNameLen);

    pstCol->acColName[sizeof(pstCol->acColName) - 1] = '\0';
    pstCol->bNeedIndex = pstColDim->bNeedIndex;

    /* 保存col type */
    ALM_ChkRet_M(ALM_TblDataTypeCheck(pstColDim->eDataType),
                 ALM_ERR_INVALID_PARA,
                 ("Col data type is invalid."));
    pstCol->eDataType = pstColDim->eDataType;

    /* 保存默认值 */
    if (ALM_NULL != pstColDim->pcDfltVal)
    {
        iRet = ALM_TblDataInitByStrVal(&pstCol->stDfltVal,
                                       pstCol->eDataType,
                                       pstColDim->pcDfltVal);
        ALM_CallRet_M(iRet, ("Col deafult value is invalid."));

        pstCol->bExistDflt = ALM_TRUE;
    }
    else
    {
        pstCol->bExistDflt = ALM_FALSE;
    }

    return ALM_OK;
}

/* 清理已初始的列，清理后应重新初始化才能使用 */
static ALM_VOID ALMStClsTblCol(INOUT ALM_TBL_COL_STRU* pstCol)
{
    if (pstCol != ALM_NULL)
    {
        if (pstCol->bExistDflt)
        {
            ALM_StClsTblData(&pstCol->stDfltVal);
        }

        ALM_OsMemset0_M(pstCol, ALM_TBL_COL_STRU, 1);
    }
}




/* 初始化全部列定义，不再使用应使用函数ALM_TblColsCls清理。 */
ALM_INT32 ALM_TblColsInit(OUT ALM_TBL_COLS_STRU* pstCols)
{
    ALM_ChkRet_M(pstCols != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    pstCols->uiColNum = 0;
    pstCols->pstCols = ALM_NULL;
    pstCols->usPortNum = 0;
    pstCols->pusPortCode = ALM_NULL;

    return ALM_OK;
}

/* 清理已初始的列，清理后应重新初始化才能使用 */
ALM_VOID ALMStClsTblCols(INOUT ALM_TBL_COLS_STRU* pstCols)
{
    ALM_UINT32 uiIdx;

    if (ALM_NULL != pstCols)
    {
        if (ALM_NULL != pstCols->pstCols)
        {
            for (uiIdx = 0; uiIdx < pstCols->uiColNum; ++uiIdx)
            {
                ALMStClsTblCol(pstCols->pstCols + uiIdx);
            }

            ALM_AdapterBaseMemFree(pstCols->pstCols);
        }

        ALM_AdapterBaseMemFree(pstCols->pusPortCode);

        ALM_OsMemset0_M(pstCols, ALM_TBL_COLS_STRU, 1);
    }
}

/* 增加列 */
ALM_INT32 ALM_TblColsAddCol(INOUT ALM_TBL_COLS_STRU* pstCols,
                            ALM_UINT32 uiColNum,
                            CONST ALM_TBL_COL_DIM_STRU* pstColsDim)
{
    ALM_UINT32 uiIdx;
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiAddIdx = 0;
    ALM_UINT8 ubIndexColNum = 0;
    ALM_UINT16 usPortNum;

    ALM_ChkRet_M(pstCols != ALM_NULL && pstColsDim != ALM_NULL && uiColNum > 0,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    /* 扩增存储空间 */
    ALM_ArrayExpandRet_M(pstCols->pstCols,
                         ALM_TBL_COL_STRU,
                         pstCols->uiColNum,
                         ALM_NULL,
                         uiColNum);

    /* 增加新列 */
    uiAddIdx = pstCols->uiColNum, ubIndexColNum = pstCols->ubIndexColNum;

    for (uiIdx = 0; uiIdx < uiColNum; ++uiIdx)
    {
        iRet = ALMTblColInit(pstCols->pstCols + uiAddIdx, pstColsDim + uiIdx);
        ALM_CallLab_M(LAB_END, ("Add invalid col."));

        if (pstCols->pstCols[uiAddIdx].bNeedIndex)
        {
            pstCols->pstCols[uiAddIdx].uiIndexId = ubIndexColNum;
            ++ubIndexColNum;

            ALM_Chk_M(ubIndexColNum <= ALM_TBL_COL_INDEX_MAX_NUM,
                      ("Index col is too many."));
        }

        ++uiAddIdx;
    }

    pstCols->uiColNum += uiColNum;
    pstCols->ubIndexColNum = ubIndexColNum;

    /* 将索引转换为port转换码，每一个索引列有三个值（等于、大于、小于） */
    /* 现阶段固定每个列有三个值，后续限定值不重复的列只有两个值 */
    /* 至少有一个port，否则无法存储多个数据 */
    for (uiIdx = 0, usPortNum = 1; uiIdx < ubIndexColNum; ++uiIdx)
    {
        usPortNum *= ALM_STORE_TREE_RSLT_MAX_NUM;
    }

    if (usPortNum > pstCols->usPortNum)
    {
        ALM_ArrayExpandRet_M(pstCols->pusPortCode,
                             ALM_UINT16,
                             pstCols->usPortNum,
                             ALM_NULL,
                             usPortNum - pstCols->usPortNum);
        pstCols->usPortNum = usPortNum;

        for (uiIdx = 0; uiIdx < pstCols->usPortNum; ++uiIdx)
        {
            pstCols->pusPortCode[uiIdx] =
                ALM_TreePortIdx2Code((ALM_UINT16)uiIdx);
        }
    }

LAB_END:

    /* 移除新增列，额外扩增的内存不会被访问可以不用释放 */
    if (iRet != ALM_OK)
    {
        for (uiIdx = pstCols->uiColNum; uiIdx < uiAddIdx; ++uiIdx)
        {
            ALMStClsTblCol(pstCols->pstCols + uiIdx);
        }
    }

    return iRet;
}


/**
* 描述:获取列索引。
* 参数:
* 1. pstCols - 列存储对象。
* 2. pcColName - 待获取的列面。
* 3. piColIdx - 得到的对应列的索引。列不存在得到负值。
*/
ALM_INT32 ALM_TblColsGetColIdx(CONST ALM_TBL_COLS_STRU* pstCols,
                               CONST ALM_CHAR* pcColName,
                               OUT ALM_INT32* piColIdx)
{
    ALM_UINT32 uiIdx;
    ALM_INT32 iCmp;

    ALM_ChkRet_M(pstCols != ALM_NULL && pcColName != ALM_NULL && piColIdx != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    *piColIdx = -1;

    for (uiIdx = 0; uiIdx < pstCols->uiColNum; ++uiIdx)
    {
        iCmp = strcmp(pstCols->pstCols[uiIdx].acColName, pcColName);

        if (0 == iCmp)
        {
            *piColIdx = (ALM_INT32)uiIdx;
            break;
        }
    }

    return ALM_OK;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_COL_H__ */
