/*******************************************************************************
Description    : 定义表格数据行。
FileName       : alm_talbe_row.c
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_table_row.h"
#include "alm_table_tool.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

/* 存储获取多个列所需数据大小 */
static ALM_INT32 ALMTblRowGetColsTotalSize(CONST ALM_TBL_COL_STRU* pstCols,
        ALM_UINT32 uiColNum,
        ALM_UINT32* puiTotalSize);

/* 依据数据列初始化数据行 */
ALM_INT32 ALM_TblRowInit(ALM_TBL_ROW_STRU* pstRow,
                         CONST ALM_TBL_COLS_STRU* pstCols)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiIdx;

    ALM_ChkRet_M(pstRow != ALM_NULL && pstCols != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));
    ALM_ChkRet_M(pstCols->uiColNum > 0 && pstCols->pstCols != ALM_NULL,
                 ALM_ERR_INVALID_PARA, ("no column to init row."));

    ALM_OsMemset0_M(pstRow, ALM_TBL_ROW_STRU, 1);

    /* 获取数据行大小 */
    iRet = ALMTblRowGetColsTotalSize(pstCols->pstCols,
                                     pstCols->uiColNum,
                                     &pstRow->uiSize);
    ALM_CallRet_M(iRet, ("Get cols total size fail."));
    ALM_ChkRet_M(pstRow->uiSize > 0, ALM_ERR_INVALID_PARA,
                 ("row size is zero."));

    /* 分配空间 */
    pstRow->pData = ALM_OsMemAlloc_M(ALM_TRUE, ALM_INT8, pstRow->uiSize);
    ALM_ChkRet_M(pstRow->pData != ALM_NULL, ALM_ERR_OS_MALLOC_FAILED,
                 ("malloc fail."));

    /* 初始化默认值 */
    for (uiIdx = 0; uiIdx < pstCols->uiColNum; ++uiIdx)
    {
        if (pstCols->pstCols[uiIdx].bExistDflt)
        {
            iRet = ALM_TblRowUpdateByIdx(pstRow,
                                         pstCols,
                                         uiIdx,
                                         &pstCols->pstCols[uiIdx].stDfltVal);
            ALM_CallLab_M(LAB_END, ("Update col default value fail."));
        }
    }

LAB_END:

    if (iRet != ALM_OK)
    {
        ALM_StClsTblRow(pstRow);
        return iRet;
    }

    return iRet;
}

/* 清理已初始化的数据行，清理后应重新初始化再使用 */
ALM_VOID ALM_StClsTblRow(ALM_TBL_ROW_STRU* pstRow)
{
    if (pstRow != ALM_NULL)
    {
        if (pstRow != ALM_NULL)
        {
            ALM_AdapterBaseMemFree(pstRow->pData);
            ALM_OsMemset0_M(pstRow, ALM_TBL_ROW_STRU, 1);
        }
    }
}

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
                              INOUT ALM_TBL_DATA_STRU* pstData)
{
    ALM_INT32 iRet;
    ALM_INT32 iColIdx;
    ALM_UINT32 uiSize;
    ALM_VOID* pDataStart;

    ALM_ChkRet_M(pstRow != ALM_NULL && pstCols != ALM_NULL && pcColName != ALM_NULL
                 && pstData != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    /* 找到col索引，数据行中数据项索引与列索引一致 */
    iRet = ALM_TblColsGetColIdx(pstCols, pcColName, &iColIdx);
    ALM_CallRet_M(iRet, ("Can not find col: %s.", pcColName));
    ALM_ChkRet_M(iColIdx >= 0, ALM_ERR_INVALID_PARA,
                 ("Can not find col: %s.", pcColName));

    /* 得到数据存储内存段 */
    iRet = ALM_TblRowGetColData(pstRow, pstCols, iColIdx, &uiSize, &pDataStart);
    ALM_CallRet_M(iRet, ("Get col store fail."));

    /* 得到数据 */
    iRet = ALM_TblDataWriteByPtr(pstData,
                                 pstCols->pstCols[iColIdx].eDataType,
                                 pDataStart,
                                 uiSize);
    ALM_CallRet_M(iRet, ("Copy data fail."));


    return ALM_OK;
}

/* 更新数据行中某一列的值。 */
ALM_INT32 ALM_TblRowUpdate(INOUT ALM_TBL_ROW_STRU* pstRow,
                           CONST ALM_TBL_COLS_STRU* pstCols,
                           CONST ALM_CHAR* pcColName,
                           CONST ALM_TBL_DATA_STRU* pstData)
{
    ALM_INT32 iRet;
    ALM_INT32 iColIdx;

    ALM_ChkRet_M(pstRow != ALM_NULL && pstCols != ALM_NULL
                 && pcColName != ALM_NULL && pstData != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    /* 找到col索引，数据行中数据项索引与列索引一致 */
    iRet = ALM_TblColsGetColIdx(pstCols, pcColName, &iColIdx);
    ALM_CallRet_M(iRet, ("Can not find col: %s.", pcColName));
    ALM_ChkRet_M(iColIdx >= 0, ALM_ERR_INVALID_PARA,
                 ("Can not find col: %s.", pcColName));

    /* 更新索引列 */
    iRet = ALM_TblRowUpdateByIdx(pstRow, pstCols, iColIdx, pstData);
    ALM_CallRet_M(iRet, ("Update col value failed."));

    return ALM_OK;
}

/* 更新数据行中某一列的值。 */
ALM_INT32 ALM_TblRowUpdateByIdx(INOUT ALM_TBL_ROW_STRU* pstRow,
                                CONST ALM_TBL_COLS_STRU* pstCols,
                                ALM_UINT32 uiColIdx,
                                CONST ALM_TBL_DATA_STRU* pstData)
{
    ALM_INT32 iRet;
    ALM_UINT32 uiSize;
    ALM_VOID* pDataStart;

    ALM_ChkRet_M(pstRow != ALM_NULL && pstCols != ALM_NULL && pstData != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_ChkRet_M(uiColIdx < pstCols->uiColNum, ALM_ERR_INVALID_PARA, (ALM_NULL));

    ALM_ChkRet_M(pstData->eType == pstCols->pstCols[uiColIdx].eDataType,
                 ALM_ERR_INVALID_PARA, ("Data type is diffrent with the col data type."));

    /* 得到数据存储内存段 */
    iRet = ALM_TblRowGetColData(pstRow, pstCols, uiColIdx, &uiSize, &pDataStart);
    ALM_CallRet_M(iRet, ("Get col store fail."));
    ALM_ChkRet_M(pstData->uiDataSize == uiSize,
                 ALM_ERR_INVALID_PARA, ("Data len is diffrent with the col data len."));

    /* 更新数据 */
    (ALM_VOID)ALM_memcpy_s(pDataStart, uiSize, pstData->pData, pstData->uiDataSize);

    return ALM_OK;
}

/* 使用部分数据初始化数据行 */
ALM_INT32 ALM_TblRowInitByPartData(OUT ALM_TBL_ROW_STRU* pstRow,
                                   CONST ALM_TBL_COLS_STRU* pstCols,
                                   CONST ALM_TBL_COLS_STRU* pstPartCols,
                                   CONST ALM_TBL_ROW_STRU* pstPartData)
{
    ALM_INT32 iRet;
    ALM_UINT32 uiIdx;
    ALM_UINT32 uiSize;
    ALM_VOID* pDataSrc;
    ALM_VOID* pDataDest;
    ALM_INT32 iColIdx;

    ALM_ChkRet_M(pstRow != ALM_NULL && pstCols != ALM_NULL
                 && pstPartCols != ALM_NULL && pstPartData != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    iRet = ALM_TblRowInit(pstRow, pstCols);
    ALM_CallRet_M(iRet, ("Init row fail."));

    for (uiIdx = 0; uiIdx < pstPartCols->uiColNum; ++uiIdx)
    {
        /* 获取源内存段 */
        iRet = ALM_TblRowGetColData(pstPartData,
                                    pstPartCols,
                                    uiIdx,
                                    &uiSize,
                                    &pDataSrc);
        ALM_CallLab_M(LAB_ERR, ("Get col data fail."));

        /* 获取目标内存段 */
        iRet = ALM_TblColsGetColIdx(pstCols,
                                    pstPartCols->pstCols[uiIdx].acColName,
                                    &iColIdx);
        ALM_CallLab_M(LAB_ERR,
                      ("Can not find col: %s.", pstPartCols->pstCols[uiIdx].acColName));

        iRet = ALM_ERR_INVALID_PARA;
        ALM_ChkLab_M(iColIdx >= 0 && (pstCols->pstCols[iColIdx].eDataType
                                      == pstPartCols->pstCols[uiIdx].eDataType),
                     LAB_ERR, ("Col is mismatch."));

        iRet = ALM_TblRowGetColData(pstRow,
                                    pstCols,
                                    iColIdx,
                                    &uiSize,
                                    &pDataDest);
        ALM_CallLab_M(LAB_ERR, ("Get col data fail."));

        /* 更新数据 */
        (ALM_VOID)ALM_memcpy_s(pDataDest, uiSize, pDataSrc, uiSize);
    }

    return ALM_OK;

LAB_ERR:

    ALM_StClsTblRow(pstRow);
    return iRet;
}

/* 数据行中获取指定列对应的内存起始位及大小 */
ALM_INT32 ALM_TblRowGetColData(CONST ALM_TBL_ROW_STRU* pstRow,
                               CONST ALM_TBL_COLS_STRU* pstCols,
                               ALM_UINT32 uiColIdx,
                               OUT ALM_UINT32* puiSize,
                               OUT ALM_VOID** ppData)
{
    ALM_INT32 iRet;
    ALM_UINT32 uiSize;
    ALM_VOID* pDataStart;

    ALM_ChkRet_M(pstRow != ALM_NULL && pstCols != ALM_NULL && puiSize != ALM_NULL
                 && ppData != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_ChkRet_M(uiColIdx < pstCols->uiColNum, ALM_ERR_INVALID_PARA, (ALM_NULL));

    *puiSize = 0;
    *ppData = ALM_NULL;

    /* 得到数据存储起始位 */
    iRet = ALMTblRowGetColsTotalSize(pstCols->pstCols, uiColIdx, &uiSize);
    ALM_CallRet_M(iRet, ("Get cols total size fail."));

    pDataStart = (ALM_VOID*)((ALM_CHAR*)pstRow->pData + uiSize);

    /* 得到数据大小 */
    iRet = ALMTblRowGetColsTotalSize(pstCols->pstCols + uiColIdx,
                                     1,
                                     &uiSize);
    ALM_CallRet_M(iRet, ("Get col size fail."));
    ALM_ChkRet_M(uiSize > 0, ALM_ERR_INVALID_PARA, ("col size is zero."));

    *puiSize = uiSize;
    *ppData = pDataStart;

    return ALM_OK;
}

/* 存储获取多个列所需数据大小 */
static ALM_INT32 ALMTblRowGetColsTotalSize(CONST ALM_TBL_COL_STRU* pstCols,
        ALM_UINT32 uiColNum,
        ALM_UINT32* puiTotalSize)
{
    ALM_UINT32 uiSize;
    ALM_UINT32 uiIdx;
    ALM_INT32 iRet;

    ALM_ChkRet_M(pstCols != ALM_NULL && puiTotalSize != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    *puiTotalSize = 0;

    for (uiIdx = 0; uiIdx < uiColNum; ++uiIdx)
    {
        iRet = ALM_TblDataSize(pstCols[uiIdx].eDataType, &uiSize);
        ALM_CallRet_M(iRet, ("Get col data size fail."));

        (*puiTotalSize) += uiSize;
    }

    return ALM_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */
