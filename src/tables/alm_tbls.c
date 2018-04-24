/*******************************************************************************
Description    : 本文件中定义全部表格。
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_tbls.inc"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */


/************************************************************************
* 函数的实现。
************************************************************************/

/* 排序表格及列定义*/
static ALM_VOID ALMTblsSortDim()
{
    ALM_UINT32 uiIdx;
    ALM_UINT32 uiIdx2;
    ALM_TBL_DIM_STRU stTmp;
    ALM_TBL_COL_DIM_EX_STRU stTmpCol;

    ALM_OsMemset0_M(&stTmp, ALM_TBL_DIM_STRU, 1);
    ALM_OsMemset0_M(&stTmpCol, ALM_TBL_COL_DIM_EX_STRU, 1);

    /* 排序表格定义 */
    for (uiIdx = 1;
         uiIdx < sizeof(s_astTbls) / sizeof(ALM_TBL_DIM_STRU);
         ++uiIdx)
    {
        for (uiIdx2 = 0; uiIdx2 < uiIdx; ++uiIdx2)
        {
            if (s_astTbls[uiIdx2].eTblIdx > s_astTbls[uiIdx].eTblIdx)
            {
                ALM_OsMemcpy_M(&stTmp, 1, &s_astTbls[uiIdx2], 1, ALM_TBL_DIM_STRU);

                ALM_OsMemcpy_M(&s_astTbls[uiIdx2], 1, &s_astTbls[uiIdx], 1,
                               ALM_TBL_DIM_STRU);

                ALM_OsMemcpy_M(&s_astTbls[uiIdx], 1, &stTmp, 1, ALM_TBL_DIM_STRU);
            }
        }
    }

    /* 排序列定义 */
    for (uiIdx = 1;
         uiIdx < sizeof(s_astTblCols) / sizeof(ALM_TBL_COL_DIM_EX_STRU);
         ++uiIdx)
    {
        for (uiIdx2 = 0; uiIdx2 < uiIdx; ++uiIdx2)
        {
            if (s_astTblCols[uiIdx2].stCol.eIdx > s_astTblCols[uiIdx].stCol.eIdx)
            {
                ALM_OsMemcpy_M(&stTmpCol, 1, &s_astTblCols[uiIdx2], 1,
                               ALM_TBL_COL_DIM_EX_STRU);

                ALM_OsMemcpy_M(&s_astTblCols[uiIdx2], 1, &s_astTblCols[uiIdx], 1,
                               ALM_TBL_COL_DIM_EX_STRU);

                ALM_OsMemcpy_M(&s_astTblCols[uiIdx], 1, &stTmpCol, 1,
                               ALM_TBL_COL_DIM_EX_STRU);
            }
        }
    }
}

/* 校验表格及列定义 */
static ALM_BOOL ALMTblsCheckDim()
{
    ALM_UINT32 uiIdx;
    ALM_UINT32 uiIdx2;
    ALM_UINT32 uiIdx3;
    ALM_INT32 iCmp;
    CONST ALM_TBL_COL_DIM_EX_STRU* pstCols;
    ALM_UINT32 uiSize;
    ALM_INT32 iRet;
    ALM_UINT32 uiColIdx;

    /* 校验表格。 */
    for (uiIdx = 0;
         uiIdx < sizeof(s_astTbls) / sizeof(ALM_TBL_DIM_STRU);
         ++uiIdx)
    {
        /* 索引必须与数组索引一致 */
        ALM_ChkRet_M((ALM_UINT32)s_astTbls[uiIdx].eTblIdx == uiIdx, ALM_FALSE,
                     ("Table index is differ with dim:%d.", uiIdx));

        /* 表格名不能重复 */
        for (uiIdx2 = 0; uiIdx2 < uiIdx; ++uiIdx2)
        {
            iCmp = strcmp(s_astTbls[uiIdx2].pcTblName,
                          s_astTbls[uiIdx].pcTblName);

            ALM_ChkRet_M(iCmp != 0, ALM_FALSE, ("Same table name \"%s\": %d - %d.",
                                                s_astTbls[uiIdx].pcTblName, uiIdx, uiIdx2));
        }

        /* 校验表列 */
        ALM_ChkRet_M(s_astTbls[uiIdx].pstColDim == s_astTblCols, ALM_FALSE, (ALM_NULL));
        ALM_ChkRet_M(s_astTbls[uiIdx].uiColNum > 0, ALM_FALSE, (ALM_NULL));
        ALM_ChkRet_M(s_astTbls[uiIdx].uiStartIdx + s_astTbls[uiIdx].uiColNum
                     < sizeof(s_astTblCols) / sizeof(ALM_TBL_COL_DIM_EX_STRU),
                     ALM_FALSE, (ALM_NULL));

        /* 校验列 */
        pstCols = &(s_astTbls[uiIdx].pstColDim[s_astTbls[uiIdx].uiStartIdx]);

        for (uiIdx2 = 0; uiIdx2 < s_astTbls[uiIdx].uiColNum; ++uiIdx2)
        {
            uiColIdx = s_astTbls[uiIdx].uiStartIdx + uiIdx2;

            /* 列索引与列序号保持一致 */
            ALM_ChkRet_M((ALM_UINT32)s_astTblCols[uiColIdx].stCol.eIdx == uiColIdx,
                         ALM_FALSE, ("Col index is differ with dim: %d.", uiColIdx));

            /* 数据类型能容纳所需大小 */
            ALM_ChkRet_M(s_astTblCols[uiColIdx].uiNeedSize > 0, ALM_FALSE,
                         ("Col need size is zero: %d.", uiColIdx));

            iRet = ALM_TblDataSize(s_astTblCols[uiColIdx].stCol.eDataType,
                                   &uiSize);
            ALM_CallRet_M(ALM_FALSE, (ALM_NULL));

            ALM_ChkRet_M(s_astTblCols[uiColIdx].uiNeedSize <= uiSize, ALM_FALSE,
                         ("Col is not enough to store data: %d.", uiColIdx));

            /* 列名不能重复 */
            for (uiIdx3 = 0; uiIdx3 < uiIdx2; ++uiIdx3)
            {
                iCmp = strcmp(pstCols[uiIdx2].stCol.pcColName,
                              pstCols[uiIdx3].stCol.pcColName);

                ALM_ChkRet_M(iCmp != 0, ALM_FALSE, ("Same col name \"%s: %s\".",
                                                    s_astTbls[uiIdx].pcTblName,
                                                    pstCols[uiIdx2].stCol.pcColName));
            }
        }

        /* 表格间列索引不能有交集 */
        for (uiIdx2 = 0; uiIdx2 < uiIdx; ++uiIdx2)
        {
            if (s_astTbls[uiIdx2].uiStartIdx < s_astTbls[uiIdx].uiStartIdx)
            {
                ALM_ChkRet_M(
                    s_astTbls[uiIdx2].uiStartIdx + s_astTbls[uiIdx2].uiColNum
                    < s_astTbls[uiIdx].uiStartIdx,
                    ALM_FALSE, ("Table col index accoss\"%s: %s\".",
                                s_astTbls[uiIdx].pcTblName,
                                s_astTbls[uiIdx2].pcTblName));
            }
            else
            {
                ALM_ChkRet_M(
                    s_astTbls[uiIdx].uiStartIdx + s_astTbls[uiIdx].uiColNum
                    < s_astTbls[uiIdx2].uiStartIdx,
                    ALM_FALSE, ("Table col index accoss\"%s: %s\".",
                                s_astTbls[uiIdx].pcTblName,
                                s_astTbls[uiIdx2].pcTblName));
            }
        }
    }

    return ALM_TRUE;
}

/*初始化建立所有表格*/
ALM_INT32 ALM_TblsMgrCreateAll()
{
    ALM_INT32 iRet = ALM_OK;
    ALM_TBL_COL_DIM_STRU* pstCols = ALM_NULL;
    ALM_UINT32 uiColNum = 0;
    ALM_UINT32 uiIdx;
    ALM_UINT32 uiIdxCol;

    ALMTblsSortDim();

    ALM_ChkRet_M(ALMTblsCheckDim(), ALM_ERR_UNKNOW, (ALM_NULL));

    for (uiIdx = 0; uiIdx < sizeof(s_astTbls) / sizeof(ALM_TBL_DIM_STRU);
         ++uiIdx)
    {
        if (s_astTbls[uiIdx].uiColNum > uiColNum)
        {
            ALM_AdapterBaseMemFree(pstCols);

            uiColNum = s_astTbls[uiIdx].uiColNum;
            pstCols = ALM_OsMemAlloc_M(ALM_FALSE, ALM_TBL_COL_DIM_STRU, uiColNum);

            if (ALM_NULL == pstCols)
            {
                iRet = ALM_ERR_OS_MALLOC_FAILED;
                goto LAB_END;
            }
        }

        for (uiIdxCol = 0; uiIdxCol < s_astTbls[uiIdx].uiColNum && ALM_NULL != pstCols;
             ++uiIdxCol)
        {
            ALM_OsMemcpy_M(pstCols + uiIdxCol, 1,
                           &s_astTbls[uiIdx].pstColDim[\
                                   s_astTbls[uiIdx].uiStartIdx + uiIdxCol].stCol,
                           1, ALM_TBL_COL_DIM_STRU);
        }

        iRet = ALM_TblNew(s_astTbls[uiIdx].pcTblName,
                          s_astTbls[uiIdx].uiColNum,
                          pstCols);
        ALM_CallLab_M(LAB_END, (ALM_NULL));
    }

LAB_END:
    ALM_AdapterBaseMemFree(pstCols);

    if (iRet != ALM_OK)
    {
        ALM_TblsMgrDropAll();
    }

    return iRet;
}

/*删除所有表格*/
ALM_VOID ALM_TblsMgrDropAll()
{
    ALM_TblDropAll();
}

/* 得到表格定义 */
CONST ALM_TBL_DIM_STRU* ALM_TblsGetDim(ALM_ENUM(ALM_TBL_IDX_ENUM) eTblIdx)
{
    ALM_ChkRet_M(eTblIdx < (ALM_INT32)(sizeof(s_astTbls) / sizeof(ALM_TBL_DIM_STRU)),
                 ALM_NULL, (ALM_NULL));

    return s_astTbls + eTblIdx;
}

/* 得到列定义 */
CONST ALM_TBL_COL_DIM_EX_STRU* ALM_TblsGetColDim(ALM_ENUM(ALM_TBL_COL_IDX_ENUM) eCol)
{
    ALM_ChkRet_M(eCol < (ALM_INT32)(sizeof(s_astTblCols) / sizeof(ALM_TBL_COL_DIM_EX_STRU)),
                 ALM_NULL, (ALM_NULL));

    return s_astTblCols + eCol;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */
