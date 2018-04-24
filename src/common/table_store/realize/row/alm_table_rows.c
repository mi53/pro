/*******************************************************************************
Description    : 定义表格数据行。
FileName       : alm_talbe_rows.c
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_table_rows.h"
#include "alm_table_row_store.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

/* 初始化表格数据行对象，应使用函数ALM_StClsTblRows清理 */
ALM_INT32 ALM_TblRowsInit(OUT ALM_TBL_ROWS_STRU* pstRows,
                          CONST ALM_TBL_COLS_STRU* pstCols)
{
    ALM_INT32 iRet;
    ALM_STORE_TREE_CFG_STRU stRowsStoreInfo;

    ALM_ChkRet_M(pstRows != ALM_NULL && pstCols != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_ChkRet_M(pstCols->uiColNum > 0 && pstCols->pstCols != ALM_NULL,
                 ALM_ERR_INVALID_PARA, ("No col to init rows."));

    ALM_OsMemset0_M(&stRowsStoreInfo, ALM_STORE_TREE_CFG_STRU, 1);

    /* 使用外部定义cols */
    pstRows->pstCols = pstCols;

    /* 指定row store初始化信息 */
    stRowsStoreInfo.stAdapterFuncs.fnDataInit =
        (ALM_STORE_TREE_INIT_DATA_FUNC)ALM_TblRowStoreInit;

    stRowsStoreInfo.stAdapterFuncs.fnDataCls =
        (ALM_STORE_TREE_CLS_DATA_FUNC)ALM_TblRowStoreCls;

    stRowsStoreInfo.stAdapterFuncs.fnDataCpy =
        (ALM_STORE_TREE_CPY_DATA_FUNC)ALM_TblRowStoreCpy;

    stRowsStoreInfo.stAdapterFuncs.fnGetPort =
        (ALM_STORE_TREE_GET_PORT_FUNC)ALM_TblRowStoreGetPort;

    stRowsStoreInfo.usPortNum = pstCols->usPortNum;
    stRowsStoreInfo.bIncSame = ALM_TRUE;
    stRowsStoreInfo.uiUnitSize = sizeof(ALM_TBL_ROW_STRU);
    stRowsStoreInfo.pEx = (CONST ALM_VOID*)pstCols;

    /* 使用多叉树存储 */
    iRet = ALM_StoreTreeInit(&pstRows->stRows, &stRowsStoreInfo);
    ALM_CallRet_M(iRet, ("Init rows store fail."));

    return ALM_OK;
}


/* 清理已初始化的数据行，清理后应重新初始化再使用 */
ALM_VOID ALM_StClsTblRows(INOUT ALM_TBL_ROWS_STRU* pstRows)
{
    if (pstRows != ALM_NULL)
    {
        ALM_StClsStoreTree(&pstRows->stRows);
        ALM_OsMemset0_M(pstRows, ALM_TBL_ROWS_STRU, 1);
    }
}


/**
* 描述: 向rows对象中插入数据。对以未指定的列采用默认值，若不存在默认
* 值使用0填充。
* 参数:
* 1. pstRows - 待插入数据的rows对象。
* 2. pstNewRow - 指定新插入数据行中数据列及列对应的值。
*/
ALM_INT32 ALM_TblRowsInsert(INOUT ALM_TBL_ROWS_STRU* pstRows,
                            CONST ALM_TBL_SROW_STRU* pstNewRow)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_TBL_ROW_STRU stTmpRow;

    ALM_ChkRet_M(pstRows != ALM_NULL && pstNewRow != ALM_NULL
                 && pstRows->pstCols != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_OsMemset0_M(&stTmpRow, ALM_TBL_ROW_STRU, 1);

    iRet = ALM_TblRowInit(&stTmpRow, pstRows->pstCols);
    ALM_CallLab_M(LAB_END, (ALM_NULL));

    iRet = ALM_TblSRowsToUpdateRow(pstNewRow,
                                   pstRows->pstCols,
                                   &stTmpRow);
    ALM_CallLab_M(LAB_END, (ALM_NULL));

    iRet = ALM_StoreTreeAdd(&pstRows->stRows, &stTmpRow);
    ALM_CallLab_M(LAB_END, (ALM_NULL));

LAB_END:
    ALM_StClsTblRow(&stTmpRow);
    return iRet;
}

/**
* 描述: 删除rows对象中符合要求的数据行。
* 参数:
* 1. pstRows - 待删除数据的rows对象。
* 2. pstCdt - 待删除的数据行应符合的条件，若传入NULL删除全部数据。
*/
ALM_INT32 ALM_TblRowsDel(INOUT ALM_TBL_ROWS_STRU* pstRows,
                         CONST ALM_TBL_CDT_STRU* pstCdt)
{
    ALM_INT32 iRet;

    ALM_ChkRet_M(pstRows != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    if (ALM_NULL == pstCdt || 0 == pstCdt->stCols.uiMapsNum)
    {
        ALM_StoreTreeDelAll(&pstRows->stRows);
    }
    else
    {
        iRet = ALM_StoreTreeDel(&pstRows->stRows, pstCdt);
        ALM_CallRet_M(iRet, ("Row delete fail."));
    }

    return ALM_OK;
}

/**
* 描述: 更新rows对象中数据行。
* 参数:
* pstRows - 待更新数据的rows对象。
* pstCdt - 需要更新数据的数据行应满足的条件，若传入NULL更新全部数据。
* pstNewVal - 指定待更新的列及对应的新值。
*/
ALM_INT32 ALM_TblRowsUpdate(INOUT ALM_TBL_ROWS_STRU* pstRows,
                            CONST ALM_TBL_CDT_STRU* pstCdt,
                            CONST ALM_TBL_SROW_STRU* pstNewVal)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_STORE_TREE_NODES_STRU stQuery;
    ALM_TBL_ROW_STRU stNew;
    ALM_UINT32 uiIdx;

    ALM_ChkRet_M(pstRows != ALM_NULL && pstNewVal != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_OsMemset0_M(&stQuery, ALM_STORE_TREE_NODES_STRU, 1);
    ALM_OsMemset0_M(&stNew, ALM_TBL_ROW_STRU, 1);
    /* 找到要更新的数据 */
    iRet = ALM_StoreTreeGetNodes(&pstRows->stRows, pstCdt, &stQuery);
    ALM_CallRet_M(iRet, ("Rows update fail."));

    if (0 == stQuery.uiNum)
    {
        return ALM_OK;
    }

    /* 更新数据 */
    iRet = ALM_TblRowStoreInit(&stNew, pstRows->pstCols);
    ALM_CallLab_M(LAB_END, (ALM_NULL));

    for (uiIdx = 0; uiIdx < stQuery.uiNum; ++uiIdx)
    {
        iRet = ALM_TblRowStoreCpy(&stNew,
                                  (ALM_TBL_ROW_STRU*)stQuery.pstArray[uiIdx]->pData,
                                  pstRows->pstCols);
        ALM_CallLab_M(LAB_END, (ALM_NULL));

        iRet = ALM_TblSRowsToUpdateRow(pstNewVal, pstRows->pstCols, &stNew);
        ALM_CallLab_M(LAB_END, (ALM_NULL));

        iRet = ALM_StoreTreeUpdateNode(&pstRows->stRows,
                                       stQuery.pstArray[uiIdx],
                                       &stNew);
        ALM_CallLab_M(LAB_END, (ALM_NULL));
    }

LAB_END:
    ALM_ArrayObjCls_M(stQuery);
    ALM_AdapterBaseMemFree(stNew.pData);

    return iRet;
}

/**
* 描述: 查询rows对象中数据行。调用该函数后应外部清理查询结果。
* 参数:
* pstRows - 待更新数据的表格。
* pstCdt - 所得数据行应满足的条件，若为NULL则无条件限制。
* pstRslt - 查询得到的全部数据行，会得到指定列的对应的数据值，若没指定列得到全部列的值。
*/
ALM_INT32 ALM_TblRowsQuery(CONST ALM_TBL_ROWS_STRU* pstRows,
                           CONST ALM_TBL_CDT_STRU* pstCdt,
                           INOUT ALM_TBL_RSLT_STRU* pstRslt)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_STORE_TREE_NODES_STRU stQuery;
    ALM_TBL_DATA_STRU stTmp;
    ALM_UINT32 uiIdx;
    ALM_UINT32 uiIdxCol;

    ALM_ChkRet_M(pstRows != ALM_NULL && pstRslt != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_OsMemset0_M(&stQuery, ALM_STORE_TREE_NODES_STRU, 1);
    ALM_OsMemset0_M(&stTmp, ALM_TBL_DATA_STRU, 1);

    /* 若没有指定列，指定全部列 */
    if (0 == pstRslt->uiColNum || ALM_NULL == pstRslt->pstCols)
    {
        iRet = ALM_TblRsltInit(pstRslt, pstRows->pstCols);
        ALM_CallLab_M(LAB_END, (ALM_NULL));
    }

    /* 找到要查询的数据 */
    iRet = ALM_StoreTreeGetNodes(&pstRows->stRows, pstCdt, &stQuery);
    ALM_CallLab_M(LAB_END, (ALM_NULL));

    /* 转换结果 */
    pstRslt->uiNum = 0;

    if (pstRslt->uiCap < stQuery.uiNum)
    {
        iRet = ALM_ToolArrayExpand((ALM_VOID**)&pstRslt->pDatas,
                                   pstRslt->uiNodeDataLen,
                                   pstRslt->uiNum,
                                   stQuery.uiNum);
        ALM_CallLab_M(LAB_END, (ALM_NULL));
        pstRslt->uiCap = stQuery.uiNum;
    }

    for (uiIdx = 0; uiIdx < stQuery.uiNum; ++uiIdx)
    {
        for (uiIdxCol = 0; uiIdxCol < pstRslt->uiColNum; ++uiIdxCol)
        {
            stTmp.eType = pstRslt->pstCols[uiIdxCol].eDataType;
            stTmp.uiDataSize = pstRslt->pstCols[uiIdxCol].uiDataSize;
            stTmp.pData = ALM_TBL_RSLT_IDX_OFFSET(pstRslt, uiIdx,
                                                  pstRslt->pstCols[uiIdxCol].uiOffSet);

            iRet = ALM_TblRowGetColVal(
                       (ALM_TBL_ROW_STRU*)stQuery.pstArray[uiIdx]->pData,
                       pstRows->pstCols,
                       pstRslt->pstCols[uiIdxCol].pcColName,
                       &stTmp);
            ALM_CallLab_M(LAB_END, (ALM_NULL));
        }

        ++pstRslt->uiNum;
    }

LAB_END:
    ALM_ArrayObjCls_M(stQuery);
    return iRet;
}

/* 得到表格中总数据行数。*/
ALM_INT32 ALM_TblRowsTotal(CONST ALM_TBL_ROWS_STRU* pstRows,
                           ALM_UINT32* puiRowTotal)
{
    ALM_INT32 iRet;

    ALM_ChkRet_M(pstRows != ALM_NULL && puiRowTotal != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    *puiRowTotal = 0;

    iRet = ALM_StoreTreeGetTotal(&pstRows->stRows, puiRowTotal);
    ALM_CallRet_M(iRet, ("Get row count fail."));


    return ALM_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */
