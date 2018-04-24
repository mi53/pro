/*******************************************************************************
Description    : rows中存储row需要指定的一些函数。
FileName       : alm_talbe_row_store.c
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_table_row_store.h"
#include "alm_table_tool.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

/* 比较条件及数据行某一具体列 */
static ALM_INT32 ALMTblRowStoreCmp(CONST ALM_VOID* pCmpL,
                                   ALM_BOOL bFindByMark,
                                   CONST ALM_TBL_ROW_STRU* pstRow,
                                   CONST ALM_TBL_COLS_STRU* pstCols,
                                   ALM_UINT32 uiColIdx,
                                   OUT ALM_ENUM(ALM_STORE_TREE_CMP_RSLT_ENUM)* peRslt);


/* 初始化ROW对象 */
ALM_INT32 ALM_TblRowStoreInit(OUT ALM_TBL_ROW_STRU* pstRow,
                              CONST ALM_TBL_COLS_STRU* pstCols)
{
    ALM_INT32 iRet;

    iRet = ALM_TblRowInit(pstRow, pstCols);
    ALM_CallRet_M(iRet, (ALM_NULL));

    return ALM_OK;
}

/* 清理ROW对象 */
ALM_VOID ALM_TblRowStoreCls(INOUT ALM_TBL_ROW_STRU* pstRow,
                            CONST ALM_TBL_COLS_STRU* pstCols)
{
    (ALM_VOID)pstCols;

    ALM_StClsTblRow(pstRow);
}

/* Row拷贝 */
ALM_INT32 ALM_TblRowStoreCpy(OUT ALM_TBL_ROW_STRU* pstDestRow,
                             CONST ALM_TBL_ROW_STRU* pstSrcRow,
                             CONST ALM_TBL_COLS_STRU* pstCols)
{
    ALM_ChkRet_M(pstDestRow != ALM_NULL && pstSrcRow != ALM_NULL,  ALM_ERR_NULL_PTR, (ALM_NULL));
    ALM_ChkRet_M(pstDestRow->pData != ALM_NULL && pstSrcRow->pData != ALM_NULL
                 && pstDestRow->uiSize == pstSrcRow->uiSize, ALM_ERR_NULL_PTR,  (ALM_NULL));

    (ALM_VOID)pstCols;

    (ALM_VOID)ALM_memcpy_s(pstDestRow->pData, pstDestRow->uiSize,
                           pstSrcRow->pData, pstSrcRow->uiSize);

    return ALM_OK;
}

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
                                 CONST ALM_TBL_COLS_STRU* pstCols)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT16 usCdtCode = 0;
    ALM_UINT16 usIgrMask = 0;
    ALM_UINT16 usTmp;
    ALM_ENUM(ALM_STORE_TREE_CMP_RSLT_ENUM) eCmpRslt;
    ALM_UINT32 uiIdx;
    ALM_UINT8 ubIndexColCount = 0;
    ALM_BOOL bEqual = ALM_TRUE;

    ALM_Chk_M(pstRow != ALM_NULL && pstRslt != ALM_NULL && pstCols != ALM_NULL, (ALM_NULL));
    ALM_Chk_M(bFindByMark || ALM_NULL != pCmpL, ("data compare but left is null"));
    ALM_Chk_M(pCmpL != pstRow, (ALM_NULL));

    /* 判断当前项并得到得到端口条件码及忽略码。 */
    for (uiIdx = 0; uiIdx < pstCols->uiColNum; ++uiIdx)
    {
        if (bEqual || pstCols->pstCols[uiIdx].bNeedIndex)
        {
            iRet = ALMTblRowStoreCmp(pCmpL, bFindByMark, pstRow, pstCols,
                                     uiIdx, &eCmpRslt);
            ALM_CallRet_M(iRet, (ALM_NULL));
        }
        else
        {
            continue;
        }

        if (pstCols->pstCols[uiIdx].bNeedIndex)
        {
            ++ubIndexColCount;

            if (eCmpRslt != ALM_STORE_TREE_CMP_IGR)
            {
                /* 得到列端口码 */
                usTmp = (ALM_UINT16)(eCmpRslt
                                     << (pstCols->pstCols[uiIdx].uiIndexId
                                         * ALM_STORE_TREE_PORT_BITS));
                usCdtCode |= usTmp;

                /* 得到不忽略校验位的掩码 */
                usTmp = (ALM_UINT16)(ALM_STORE_TREE_PORT_MASK
                                     << (pstCols->pstCols[uiIdx].uiIndexId
                                         * ALM_STORE_TREE_PORT_BITS));
                usIgrMask |= usTmp;
            }
        }

        if (bEqual
            && eCmpRslt != ALM_STORE_TREE_CMP_IGR
            && eCmpRslt != ALM_STORE_TREE_CMP_EQUAL)
        {
            bEqual = ALM_FALSE;
        }

        if (!bEqual && ubIndexColCount >= pstCols->ubIndexColNum)
        {
            break;
        }
    }

    pstRslt->bThisIsEqual = bEqual;

    /* 依据条件码及端口码得到Next。 */
    pstRslt->uiNum = 0;

    for (uiIdx = 0; uiIdx < pstCols->usPortNum; ++uiIdx)
    {
        usTmp = (pstCols->pusPortCode[uiIdx] ^ usCdtCode) & usIgrMask;

        if (usTmp != 0)
        {
            continue;
        }

        ALM_Chk_M(pstRslt->uiNum < pstRslt->uiCap, (ALM_NULL));
        pstRslt->ausNexts[pstRslt->uiNum] = (ALM_UINT16)uiIdx;
        ++pstRslt->uiNum;
    }

    return ALM_OK;
}


/* 比较条件及数据行某一具体列 */
static ALM_INT32 ALMTblRowStoreCmp(CONST ALM_VOID* pCmpL,
                                   ALM_BOOL bFindByMark,
                                   CONST ALM_TBL_ROW_STRU* pstRow,
                                   CONST ALM_TBL_COLS_STRU* pstCols,
                                   ALM_UINT32 uiColIdx,
                                   OUT ALM_ENUM(ALM_STORE_TREE_CMP_RSLT_ENUM)* peRslt)
{
    ALM_INT32 iRet = ALM_OK;
    CONST ALM_TBL_CDT_STRU* pstCdt = (CONST ALM_TBL_CDT_STRU*)pCmpL;
    CONST ALM_TBL_ROW_STRU* pstCdtRow = (CONST ALM_TBL_ROW_STRU*)pCmpL;
    ALM_INT32 iCmp;
    ALM_TBL_DATA_STRU stDataTmp; /* 仅作一个数据包使用 */
    ALM_TBL_DATA_STRU stCdtData; /* 仅作一个数据包使用 */
    ALM_TBL_DATA_STRU* pstCdtData = ALM_NULL;
    ALM_INT32 iColIdx;

    ALM_Chk_M(pstRow != ALM_NULL && pstCols != ALM_NULL && peRslt != ALM_NULL, (ALM_NULL));

    if (bFindByMark && ALM_NULL != pstCdt)
    {
        /* 得到cdt中对应列 */
        iRet = ALM_TblSRowsGetIdx(&pstCdt->stCols,
                                  pstCols->pstCols[uiColIdx].acColName,
                                  &iColIdx);
        ALM_CallRet_M(iRet, (ALM_NULL));

        /* 未指定列，默认符合条件 */
        if (iColIdx >= 0)
        {
            pstCdtData = &(pstCdt->stCols.pstMaps[iColIdx].stVal);
        }
    }
    else if (!bFindByMark)
    {
        ALM_Chk_M(pstCdtRow != ALM_NULL, (ALM_NULL));

        stCdtData.eType = pstCols->pstCols[uiColIdx].eDataType;
        iRet = ALM_TblRowGetColData(pstCdtRow,
                                    pstCols,
                                    uiColIdx,
                                    &stCdtData.uiDataSize,
                                    &stCdtData.pData);
        ALM_CallRet_M(iRet, (ALM_NULL));

        pstCdtData = &stCdtData;
    }

    if (ALM_NULL == pstCdtData)
    {
        *peRslt = ALM_STORE_TREE_CMP_IGR;
        return ALM_OK;
    }

    /* 得到row中对应列的数据 */
    stDataTmp.eType = pstCols->pstCols[uiColIdx].eDataType;
    iRet = ALM_TblRowGetColData(pstRow,
                                pstCols,
                                uiColIdx,
                                &stDataTmp.uiDataSize,
                                &stDataTmp.pData);
    ALM_CallRet_M(iRet, (ALM_NULL));

    /* 比较并转换比较结果 */
    iRet = ALM_TblDataCmp(pstCdtData, &stDataTmp, &iCmp);
    ALM_CallRet_M(iRet, (ALM_NULL));

    if (0 == iCmp)
    {
        *peRslt = ALM_STORE_TREE_CMP_EQUAL;
    }
    else if (iCmp > 0)
    {
        *peRslt = ALM_STORE_TREE_CMP_MORE;
    }
    else
    {
        *peRslt = ALM_STORE_TREE_CMP_LESS;
    }

    return ALM_OK;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */
