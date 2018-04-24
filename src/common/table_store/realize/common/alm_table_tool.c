/*******************************************************************************
Description    : 扩展并完善工具对象。
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_table_tool.inc"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

/************************************************************************/
/*         ALM_TBL_DATA_STRU: 注意正确处理未字节对齐的整形数据。               */
/************************************************************************/

/* 确定数据类型的有效性 */
ALM_BOOL ALM_TblDataTypeCheck(ALM_ENUM(ALM_TBL_DATA_TYPE_ENUM) eType)
{
    switch (eType)
    {
        case ALM_TBL_DATA_TYPE_UINT8:
        case ALM_TBL_DATA_TYPE_UINT32:
        case ALM_TBL_DATA_TYPE_UINT64:
        case ALM_TBL_DATA_TYPE_CHAR18:
        case ALM_TBL_DATA_TYPE_CHAR32:
        case ALM_TBL_DATA_TYPE_CHAR64:
        case ALM_TBL_DATA_TYPE_CHAR128:
        case ALM_TBL_DATA_TYPE_CHAR256:
        case ALM_TBL_DATA_TYPE_CHAR512:
            return ALM_TRUE;

        default:
            return ALM_FALSE;
    }
}

/* 获取指定数据类型大小 */
ALM_INT32 ALM_TblDataSize(ALM_ENUM(ALM_TBL_DATA_TYPE_ENUM) eType,
                          OUT ALM_UINT32* puiSize)
{
    ALM_ChkRet_M(puiSize != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    switch (eType)
    {
        case ALM_TBL_DATA_TYPE_UINT8:
            *puiSize = ALM_TBL_DATA_TYPE_UINT8_LEN;
            break;

        case ALM_TBL_DATA_TYPE_UINT32:
            *puiSize = ALM_TBL_DATA_TYPE_UINT32_LEN;
            break;

        case ALM_TBL_DATA_TYPE_UINT64:
            *puiSize = ALM_TBL_DATA_TYPE_UINT64_LEN;
            break;

        case ALM_TBL_DATA_TYPE_CHAR18:
            *puiSize = ALM_TBL_DATA_TYPE_CHAR18_LEN;
            break;

        case ALM_TBL_DATA_TYPE_CHAR32:
            *puiSize = ALM_TBL_DATA_TYPE_CHAR32_LEN;
            break;

        case ALM_TBL_DATA_TYPE_CHAR64:
            *puiSize = ALM_TBL_DATA_TYPE_CHAR64_LEN;
            break;

        case ALM_TBL_DATA_TYPE_CHAR128:
            *puiSize = ALM_TBL_DATA_TYPE_CHAR128_LEN;
            break;

        case ALM_TBL_DATA_TYPE_CHAR256:
            *puiSize = ALM_TBL_DATA_TYPE_CHAR256_LEN;
            break;

        case ALM_TBL_DATA_TYPE_CHAR512:
            *puiSize = ALM_TBL_DATA_TYPE_CHAR512_LEN;
            break;

        default:
            *puiSize = 0;
            return ALM_ERR_INVALID_PARA;
    }

    return ALM_OK;
}

/* 初始化数据项 */
ALM_INT32 ALM_TblDataInit(OUT ALM_TBL_DATA_STRU* pstData,
                          ALM_ENUM(ALM_TBL_DATA_TYPE_ENUM) eType)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiSize;

    ALM_ChkRet_M(pstData != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    iRet = ALM_TblDataSize(eType, &uiSize);
    ALM_CallRet_M(iRet, ("Get data size fail."));

    pstData->eType = eType;
    pstData->uiDataSize = uiSize;
    pstData->pData = ALM_OsMemAlloc_M(ALM_TRUE, ALM_CHAR, uiSize);

    ALM_ChkRet_M(pstData->pData != ALM_NULL, ALM_ERR_OS_MALLOC_FAILED, (ALM_NULL));

    return iRet;
}

/* 依据字符串及指定数据类型，得到对应的数据对象 */
ALM_INT32 ALM_TblDataInitByStrVal(OUT ALM_TBL_DATA_STRU* pstData,
                                  ALM_ENUM(ALM_TBL_DATA_TYPE_ENUM) eType,
                                  CONST ALM_CHAR* pcVal)
{
    ALM_INT32 iRet;

    ALM_ChkRet_M(pstData != ALM_NULL && pcVal != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    iRet = ALM_TblDataInit(pstData, eType);
    ALM_CallRet_M(iRet, (ALM_NULL));

    iRet = ALMTblDataWrite(pstData, pcVal);
    ALM_CallLab_M(LAB_END, (ALM_NULL));

LAB_END:

    if (iRet != ALM_OK)
    {
        ALM_StClsTblData(pstData);
    }

    return iRet;
}

/* 写入数据，不会重新分配空间，非字符串类型数据大小必须一致，字符串类型容量必须足够 */
ALM_INT32 ALM_TblDataWriteByPtr(INOUT ALM_TBL_DATA_STRU* pstData,
                                ALM_ENUM(ALM_TBL_DATA_TYPE_ENUM) eType,
                                CONST ALM_VOID* pData,
                                ALM_UINT32 uiDataSize)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiStrLen;

    ALM_ChkRet_M(pData != ALM_NULL && uiDataSize > 0, ALM_ERR_NULL_PTR, (ALM_NULL));
    ALM_ChkRet_M(ALM_TblDataTypeCheck(eType), ALM_ERR_INVALID_PARA, (ALM_NULL));

    if (ALMTblDataCheckStr(eType))
    {
        uiStrLen = ALM_ToolStrLen((CONST ALM_CHAR*)pData, uiDataSize);

        ALM_ChkLab_M(!(uiStrLen >= pstData->uiDataSize
                       && (iRet = ALM_ERR_INVALID_PARA) != ALM_OK),
                     LAB_END, ("string is too long to the data."));

        (ALM_VOID)ALM_memset_s(pstData->pData, pstData->uiDataSize, 0,
                               pstData->uiDataSize);

        (ALM_VOID)ALM_strncpy_s((ALM_CHAR*)pstData->pData,
                                pstData->uiDataSize,
                                (CONST ALM_CHAR*)pData,
                                uiStrLen);
    }
    else
    {
        ALM_ChkLab_M(!(uiDataSize != pstData->uiDataSize
                       && (iRet = ALM_ERR_INVALID_PARA) != ALM_OK),
                     LAB_END, (ALM_NULL));

        (ALM_VOID)ALM_memcpy_s(pstData->pData, pstData->uiDataSize, pData,
                               uiDataSize);
    }

LAB_END:

    if (iRet != ALM_OK)
    {
        ALM_StClsTblData(pstData);
    }

    return iRet;
}

/* 清理data对象 */
ALM_VOID ALM_StClsTblData(INOUT ALM_TBL_DATA_STRU* pstData)
{
    if (pstData != ALM_NULL)
    {
        ALM_AdapterBaseMemFree(pstData->pData);
        ALM_OsMemset0_M(pstData, ALM_TBL_DATA_STRU, 1);
    }
}


/* 改变数据字节序*/
ALM_INT32 ALM_TblDataChangeEndin(INOUT ALM_TBL_DATA_STRU* pstData)
{
    ALM_ChkRet_M(pstData != ALM_NULL && pstData->pData != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    if (!ALMTblDataCheckStr(pstData->eType))
    {
        ALM_ToolChangeEndin(pstData->pData, pstData->uiDataSize);
    }

    return ALM_OK;
}

/* 判断是否为字符串类型 */
static ALM_BOOL ALMTblDataCheckStr(ALM_ENUM(ALM_TBL_DATA_TYPE_ENUM) eType)
{
    switch (eType)
    {
        case ALM_TBL_DATA_TYPE_UINT8:
        case ALM_TBL_DATA_TYPE_UINT32:
        case ALM_TBL_DATA_TYPE_UINT64:
            return ALM_FALSE;

        default:
            return ALM_TRUE;
    }
}

/* 将字符串转换为data对应的数据类型，并写入数据对象 */
static ALM_INT32 ALMTblDataWrite(INOUT ALM_TBL_DATA_STRU* pstData,
                                 CONST ALM_CHAR* pcVal)
{
    ALM_INT32 iRet;
    ALM_UINT32 uiValLen;
    ALM_UINT64 ulValTmp;
    ALM_UINT32 uiValTmp;
    ALM_UCHAR ucValTmp;
    ALM_VOID* pNew;

    ALM_ChkRet_M(pstData != ALM_NULL && pcVal != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));
    ALM_ChkRet_M(ALM_TblDataTypeCheck(pstData->eType), ALM_ERR_INVALID_PARA,
                 (ALM_NULL));

    if (!ALMTblDataCheckStr(pstData->eType))
    {
        iRet = ALM_ToolAtoul(pcVal, 0, &ulValTmp);
        ALM_CallRet_M(iRet, (ALM_NULL));

        if (ALM_TBL_DATA_TYPE_UINT32 == pstData->eType)
        {
            uiValTmp = (ALM_UINT32)ulValTmp;
            pNew = &uiValTmp;
            uiValLen = sizeof(uiValTmp);
        }
        else if (ALM_TBL_DATA_TYPE_UINT8 == pstData->eType)
        {
            ucValTmp = (ALM_UCHAR)ulValTmp;
            pNew = &ucValTmp;
            uiValLen = sizeof(ucValTmp);
        }
        else
        {
            pNew = &ulValTmp;
            uiValLen = sizeof(ulValTmp);
        }

        /* 考虑数据可能未字节对齐，使用mmemcpy赋值 */
        (ALM_VOID)ALM_memcpy_s(pstData->pData, pstData->uiDataSize, pNew,
                               uiValLen);
    }
    else
    {
        uiValLen = ALM_ToolStrLen(pcVal, ALM_TBL_DATA_TYPE_MAX_LEN);
        ALM_ChkRet_M(uiValLen < ALM_TBL_DATA_TYPE_MAX_LEN,
                     ALM_ERR_INVALID_PARA, (ALM_NULL));

        ALM_ChkRet_M(uiValLen < pstData->uiDataSize,
                     ALM_ERR_INVALID_PARA,
                     ("string is too ALM_INT64 to the data."));

        (ALM_VOID)ALM_strncpy_s((ALM_CHAR*)pstData->pData, pstData->uiDataSize,
                                pcVal, uiValLen);
        ((ALM_CHAR*)pstData->pData)[pstData->uiDataSize - 1] = '\0';
    }

    return ALM_OK;
}


/* 比较数据大小，数据类型必须一致 */
ALM_INT32 ALM_TblDataCmp(CONST ALM_TBL_DATA_STRU* pstLeft,
                         CONST ALM_TBL_DATA_STRU* pstRight,
                         ALM_INT32* piRlst)
{
    ALM_UINT32 uiLen;

    ALM_ChkRet_M(pstLeft != ALM_NULL && pstRight != ALM_NULL && piRlst != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_ChkRet_M(pstLeft->pData != ALM_NULL && pstRight->pData != ALM_NULL
                 && pstLeft->eType == pstRight->eType
                 && ALM_TblDataTypeCheck(pstLeft->eType),
                 ALM_ERR_INVALID_PARA, (ALM_NULL));

    if (ALMTblDataCheckStr(pstLeft->eType))
    {
        uiLen = pstLeft->uiDataSize > pstRight->uiDataSize
                ? pstLeft->uiDataSize : pstRight->uiDataSize;
        *piRlst = strncmp((CONST ALM_CHAR*)pstLeft->pData,
                          (CONST ALM_CHAR*)pstRight->pData,
                          uiLen);
    }
    else
    {
        ALM_ChkRet_M(pstLeft->uiDataSize == pstRight->uiDataSize
                     && pstLeft->uiDataSize > 0,
                     ALM_ERR_INVALID_PARA, (ALM_NULL));

        *piRlst = memcmp(pstLeft->pData, pstRight->pData, pstLeft->uiDataSize);

        if (ALM_ToolIsBigEndin())
        {
            *piRlst *= -1;
        }
    }

    return ALM_OK;
}


/* 获取数据内容，数值数据类型，buf长度必须与数据类型大小保持一致，string类型只需能存储便可 */
ALM_INT32 ALM_TblDataGet(CONST ALM_TBL_DATA_STRU* pstData,
                         ALM_ENUM(ALM_TBL_DATA_TYPE_ENUM) eDataType,
                         OUT ALM_VOID* pBuf,
                         ALM_UINT32 uiBufLen)
{
    ALM_INT32 iRet;

    ALM_ChkRet_M(pstData != ALM_NULL && pBuf != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));
    ALM_ChkRet_M(ALM_TblDataTypeCheck(pstData->eType) && pstData->pData != ALM_NULL
                 && pstData->eType == eDataType, ALM_ERR_INVALID_PARA, (ALM_NULL));

    if (ALMTblDataCheckStr(eDataType))
    {
        iRet = ALM_strncpy_s((ALM_CHAR*)pBuf, uiBufLen,
                             (ALM_CHAR*)pstData->pData,
                             pstData->uiDataSize);
        ALM_ChkRet_M(iRet == EOK, ALM_ERR_INVALID_PARA,
                     ("Copy col string value fail, maybe buffer is not enough."));
    }
    else
    {
        ALM_ChkRet_M(pstData->uiDataSize == uiBufLen, ALM_ERR_INVALID_PARA,
                     ("Get result col value with different size."));
        (ALM_VOID)ALM_memcpy_s(pBuf, uiBufLen, pstData->pData,
                               pstData->uiDataSize);
    }

    return ALM_OK;
}

/************************************************************************/
/*              ALM_TBL_CDT_STRU                                    */
/************************************************************************/

/* 清理已初始化的查询条件 */
ALM_VOID ALM_StClsTblCdt(INOUT ALM_TBL_CDT_STRU* pstCdt)
{
    if (pstCdt != ALM_NULL)
    {
        ALM_StClsTblSRow(&pstCdt->stCols);
    }
}

/* 增加列条件 */
ALM_INT32 ALM_TblCdtAddColCdts(INOUT ALM_TBL_CDT_STRU* pstCdt,
                               CONST ALM_TBL_SROW_STRU* pstColCdts)
{
    ALM_UINT32 uiIdx;
    ALM_INT32 iRet = ALM_OK;

    ALM_ChkRet_M(pstCdt != ALM_NULL && pstColCdts != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));
    ALM_ChkRet_M(pstColCdts->uiMapsNum > 0 && pstColCdts->pstMaps != ALM_NULL,
                 ALM_ERR_INVALID_PARA, (ALM_NULL));

    /* 扩展存储空间 */
    iRet = ALMTblSRowsExpand(&pstCdt->stCols, pstColCdts->uiMapsNum);
    ALM_CallRet_M(iRet, (ALM_NULL));

    /* 添加新的col条件 */
    for (uiIdx = 0; uiIdx < pstColCdts->uiMapsNum; ++uiIdx)
    {
        iRet = ALM_TblSRowsAdd(&pstCdt->stCols,
                               pstColCdts->pstMaps[uiIdx].acColName,
                               &(pstColCdts->pstMaps[uiIdx].stVal),
                               ALM_NULL);
        ALM_CallRet_M(iRet, (ALM_NULL));
    }

    return ALM_OK;
}

/* 增加列条件 */
ALM_INT32 ALM_TblCdtAdd(INOUT ALM_TBL_CDT_STRU* pstCdt,
                        CONST ALM_CHAR* pcColName,
                        CONST ALM_TBL_DATA_STRU* pstVal)
{
    ALM_INT32 iRet = ALM_OK;

    ALM_ChkRet_M(pstCdt != ALM_NULL && pcColName != ALM_NULL
                 && pstVal != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    /* 扩展存储空间 */
    iRet = ALMTblSRowsExpand(&pstCdt->stCols, 1);
    ALM_CallRet_M(iRet, (ALM_NULL));

    iRet = ALM_TblSRowsAdd(&pstCdt->stCols, pcColName, pstVal, ALM_NULL);
    ALM_CallRet_M(iRet, (ALM_NULL));

    return ALM_OK;
}

/************************************************************************/
/*               ALM_TBL_COL_MAP_STRU                               */
/************************************************************************/


/* 初始化单个列条件，不再使用应使用函数ALMTblQueryColCdtCls清理 */
static ALM_INT32 ALMStInitTblSRowsCol(OUT ALM_TBL_COL_MAP_STRU* pstColMap,
                                      CONST ALM_CHAR* pcColName,
                                      CONST ALM_TBL_DATA_STRU* pstVal)
{
    ALM_UINT32 uiNameLen;
    ALM_INT32 iRet;

    ALM_ChkRet_M(pstColMap != ALM_NULL && pcColName != ALM_NULL
                 && pstVal != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    uiNameLen = ALM_ToolStrLen(pcColName, sizeof(pstColMap->acColName));
    ALM_ChkRet_M(uiNameLen > 0 && uiNameLen < sizeof(pstColMap->acColName),
                 ALM_ERR_INVALID_PARA, ("Col name is more than max value."));

    (ALM_VOID)ALM_strncpy_s(pstColMap->acColName,
                            sizeof(pstColMap->acColName),
                            pcColName,
                            uiNameLen);

    iRet = ALM_TblDataInit(&pstColMap->stVal, pstVal->eType);
    ALM_CallRet_M(iRet, (ALM_NULL));

    iRet = ALM_TblDataWriteByPtr(&pstColMap->stVal,
                                 pstVal->eType,
                                 pstVal->pData,
                                 pstVal->uiDataSize);
    ALM_CallRet_M(iRet, (ALM_NULL));


    return ALM_OK;
}

/* 清理已初始的列，清理后应重新初始化才能使用 */
static ALM_VOID ALMStClsTblSRowsCol(INOUT ALM_TBL_COL_MAP_STRU* pstColMap)
{
    if (pstColMap != ALM_NULL)
    {
        ALM_StClsTblData(&pstColMap->stVal);
        ALM_OsMemset0_M(pstColMap, ALM_TBL_COL_MAP_STRU, 1);
    }
}

/* 更新一个数据行，只更新指定列 */
ALM_INT32 ALM_TblSRowsToUpdateRow(CONST ALM_TBL_SROW_STRU* pstSRows,
                                  CONST ALM_TBL_COLS_STRU* pstCols,
                                  INOUT ALM_TBL_ROW_STRU* pstRow)
{
    ALM_UINT32 uiIdx;
    ALM_INT32 iRet;

    ALM_ChkRet_M(pstSRows != ALM_NULL && pstCols != ALM_NULL
                 && pstRow != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    for (uiIdx = 0; uiIdx < pstSRows->uiMapsNum; ++uiIdx)
    {
        iRet = ALM_TblRowUpdate(pstRow,
                                pstCols,
                                pstSRows->pstMaps[uiIdx].acColName,
                                &(pstSRows->pstMaps[uiIdx].stVal));
        ALM_CallRet_M(iRet, (ALM_NULL));
    }

    return ALM_OK;
}



/* 初始化单个列条件，不再使用应使用函数ALM_StClsTblSRow清理 */
ALM_INT32 ALM_StInitTblSRows(OUT ALM_TBL_SROW_STRU* pstSRows,
                             ALM_UINT32 uiInitCap)
{
    ALM_INT32 iRet;

    ALM_ChkRet_M(pstSRows != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    pstSRows->uiMapsCap = 0;
    pstSRows->uiMapsNum = 0;
    pstSRows->pstMaps = ALM_NULL;

    iRet = ALMTblSRowsExpand(pstSRows, uiInitCap);
    ALM_CallRet_M(iRet, (ALM_NULL));

    return ALM_OK;
}

/* 清理已初始的列，清理后应重新初始化才能使用 */
ALM_VOID ALM_StClsTblSRow(INOUT ALM_TBL_SROW_STRU* pstSRows)
{
    ALM_UINT32 uiIdx;

    if (pstSRows != ALM_NULL)
    {
        if (pstSRows->uiMapsNum > 0 && pstSRows->pstMaps != ALM_NULL)
        {
            for (uiIdx = 0; uiIdx < pstSRows->uiMapsNum; ++uiIdx)
            {
                ALMStClsTblSRowsCol(pstSRows->pstMaps + uiIdx);
            }
        }

        ALM_AdapterBaseMemFree(pstSRows->pstMaps);
        ALM_OsMemset0_M(pstSRows, ALM_TBL_SROW_STRU, 1);
    }
}

/* 扩展COL MAPS存储空间，保证总容量与当前数目的差值大于等于指定的扩展数目 */
static ALM_INT32 ALMTblSRowsExpand(INOUT ALM_TBL_SROW_STRU* pstSRows,
                                   ALM_UINT32 uiExpand)
{
    ALM_INT32 iRet;

    ALM_ChkRet_M(pstSRows != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_ArrayExpandRet_M(pstSRows->pstMaps,
                         ALM_TBL_COL_MAP_STRU,
                         pstSRows->uiMapsNum,
                         &pstSRows->uiMapsCap,
                         uiExpand);

    return ALM_OK;
}

/* 增加列-值对，并得到添加后所在索引 */
/* piIdx - 若为NULL，不输出索引 */
ALM_INT32 ALM_TblSRowsAdd(INOUT ALM_TBL_SROW_STRU* pstSRows,
                          CONST ALM_CHAR* pcColName,
                          CONST ALM_TBL_DATA_STRU* pstVal,
                          OUT ALM_INT32* piIdx)
{
    ALM_INT32 iRet;
    ALM_INT32 idx;

    ALM_ChkRet_M(pstSRows != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    iRet = ALM_TblSRowsGetIdx(pstSRows, pcColName, &idx);
    ALM_CallRet_M(iRet, (ALM_NULL));
    ALM_ChkRet_M(idx < 0, ALM_ERR_INVALID_PARA, ("Col have exist."));

    iRet = ALMTblSRowsExpand(pstSRows, 1);
    ALM_CallRet_M(iRet, (ALM_NULL));

    iRet = ALMStInitTblSRowsCol(pstSRows->pstMaps + pstSRows->uiMapsNum,
                                pcColName,
                                pstVal);
    ALM_CallRet_M(iRet, (ALM_NULL));

    if (ALM_NULL != piIdx)
    {
        *piIdx = (ALM_INT32)pstSRows->uiMapsNum;
    }

    ++pstSRows->uiMapsNum;

    return ALM_OK;
}

/* 得到对应列的值 */
ALM_INT32 ALM_TblSRowsGet(IN CONST ALM_TBL_SROW_STRU* pstSRows,
                          CONST ALM_CHAR* pcColName,
                          OUT CONST ALM_TBL_DATA_STRU** ppstVal)
{
    ALM_INT32 iRet;
    ALM_INT32 idx;

    ALM_ChkRet_M(pstSRows != ALM_NULL && pcColName != ALM_NULL && ppstVal != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    iRet = ALM_TblSRowsGetIdx(pstSRows, pcColName, &idx);
    ALM_CallRet_M(iRet, (ALM_NULL));
    ALM_ChkRet_M(idx >= 0, ALM_ERR_INVALID_PARA, ("Col do not exist."));

    *ppstVal = &(pstSRows->pstMaps[idx].stVal);

    return ALM_OK;
}

/* 得到某一列的索引，若不存在指定列得到的索引为负值 */
ALM_INT32 ALM_TblSRowsGetIdx(CONST ALM_TBL_SROW_STRU* pstSRows,
                             CONST ALM_CHAR* pcColName,
                             OUT ALM_INT32* piIdx)
{
    ALM_INT32 iCmp;
    ALM_UINT32 uiIdx;

    ALM_ChkRet_M(pstSRows != ALM_NULL && pcColName != ALM_NULL
                 && piIdx != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    *piIdx = -1;

    for (uiIdx = 0; uiIdx < pstSRows->uiMapsNum; ++uiIdx)
    {
        iCmp = strcmp(pcColName, pstSRows->pstMaps[uiIdx].acColName);

        if (0 == iCmp)
        {
            *piIdx = (ALM_INT32)uiIdx;
            break;
        }
    }

    return ALM_OK;
}


/************************************************************************/
/*                         ALM_TBL_RSLT_STRU                            */
/************************************************************************/

/* 清理已初始化的查询包。 */
ALM_VOID ALM_StClsTblRslt(INOUT ALM_TBL_RSLT_STRU* pstRslt)
{
    ALM_UINT32 uiIdx;

    if (pstRslt != ALM_NULL)
    {
        for (uiIdx = 0; uiIdx < pstRslt->uiColNum; ++uiIdx)
        {
            ALM_AdapterBaseMemFree(pstRslt->pstCols[uiIdx].pcColName);
        }

        ALM_AdapterBaseMemFree(pstRslt->pstCols);
        ALM_AdapterBaseMemFree(pstRslt->pDatas);

        ALM_OsMemset0_M(pstRslt, ALM_TBL_RSLT_STRU, 1);

    }
}

/* 添加待查询列 */
ALM_INT32 ALM_TblRsltAddCol(INOUT ALM_TBL_RSLT_STRU* pstRslt,
                            ALM_UINT32 eIdx,
                            CONST ALM_CHAR* pstCol,
                            ALM_ENUM(ALM_TBL_DATA_TYPE_ENUM) eDataType)
{
    ALM_INT32 iRet = ALM_OK;

    ALM_ChkRet_M(pstRslt != ALM_NULL && pstCol != ALM_NULL, ALM_ERR_INVALID_PARA,
                 (ALM_NULL));

    /* 扩展空间 */
    ALM_ArrayExpandRet_M(pstRslt->pstCols,
                         ALM_TBL_RSLT_COL_UNIT_STRU,
                         pstRslt->uiColNum,
                         &pstRslt->uiColCap,
                         1);

    /* 得到数据大小 */
    iRet = ALM_TblDataSize(eDataType,
                           &pstRslt->pstCols[pstRslt->uiColNum].uiDataSize);
    ALM_CallRet_M(iRet, (ALM_NULL));

    /* 得到数据类型 */
    pstRslt->pstCols[pstRslt->uiColNum].eDataType = eDataType;

    /* 得到数据偏移 */
    if (pstRslt->uiColNum > 0)
    {
        pstRslt->pstCols[pstRslt->uiColNum].uiOffSet =
            pstRslt->pstCols[pstRslt->uiColNum - 1].uiDataSize
            + pstRslt->pstCols[pstRslt->uiColNum - 1].uiOffSet;
    }
    else
    {
        pstRslt->pstCols[pstRslt->uiColNum].uiOffSet = 0;
    }

    /*列索引*/
    pstRslt->pstCols[pstRslt->uiColNum].eIdx = eIdx;
    /* 得到列名 */
    pstRslt->pstCols[pstRslt->uiColNum].pcColName =
        ALM_OsMemAlloc_M(ALM_FALSE, ALM_CHAR, ALM_CONST_NAME_MAX_LEN + 1);

    ALM_ChkRet_M(pstRslt->pstCols[pstRslt->uiColNum].pcColName != ALM_NULL,
                 ALM_ERR_OS_MALLOC_FAILED, (ALM_NULL));

    (ALM_VOID)ALM_strcpy_s(pstRslt->pstCols[pstRslt->uiColNum].pcColName,
                           ALM_CONST_NAME_MAX_LEN + 1,
                           pstCol);

    /* 更新整体信息 */
    pstRslt->uiNodeDataLen += pstRslt->pstCols[pstRslt->uiColNum].uiDataSize;
    ++pstRslt->uiColNum;


    return iRet;
}


/* 获取列在结果集中的索引，若不存在得到负数 */
ALM_INT32 ALM_TblRsltGetColIdx(CONST ALM_TBL_RSLT_STRU* pstRslt,
                               CONST ALM_CHAR* pcColName,
                               OUT ALM_INT32* piColIdx)
{
    ALM_UINT32 uiIdx;
    ALM_INT32 iCmp;

    ALM_ChkRet_M(pstRslt != ALM_NULL && piColIdx != ALM_NULL && pcColName != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    *piColIdx = -1;

    for (uiIdx = 0; uiIdx < pstRslt->uiColNum; ++uiIdx)
    {
        iCmp = strcmp(pstRslt->pstCols[uiIdx].pcColName, pcColName);

        if (0 == iCmp)
        {
            *piColIdx = (ALM_INT32)uiIdx;
            break;
        }
    }

    return ALM_OK;
}

/* 依据列索引得到列的值 */
ALM_INT32 ALM_TblRsltGetColByIdx(CONST ALM_TBL_RSLT_STRU* pstRslt,
                                 CONST ALM_VOID* pRsltRow,
                                 ALM_INT32 iColIdx,
                                 OUT ALM_VOID* pBuf,
                                 ALM_UINT32 uiBufLen)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_TBL_DATA_STRU stTmp;

    ALM_ChkRet_M(pBuf != ALM_NULL && pstRslt != ALM_NULL && pRsltRow != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_ChkRet_M(iColIdx >= 0 && ((ALM_UINT32)iColIdx) < pstRslt->uiColNum,
                 ALM_ERR_INVALID_PARA, (ALM_NULL));

    stTmp.uiDataSize = pstRslt->pstCols[iColIdx].uiDataSize;
    stTmp.eType = pstRslt->pstCols[iColIdx].eDataType;
    stTmp.pData = ALM_TOOL_ADDR_OFFSET(pRsltRow,
                                       pstRslt->pstCols[iColIdx].uiOffSet);

    iRet = ALM_TblDataGet(&stTmp, stTmp.eType, pBuf, uiBufLen);
    ALM_CallRet_M(iRet, (ALM_NULL));

    return ALM_OK;
}

/* 获取列在结果集中的指定列的值 */
ALM_INT32 ALM_TblRsltGetCol(CONST ALM_TBL_RSLT_STRU* pstRslt,
                            CONST ALM_VOID* pRsltRow,
                            CONST ALM_CHAR* pcColName,
                            OUT ALM_VOID* pBuf,
                            ALM_UINT32 uiBufLen)
{
    ALM_INT32 iRet;
    ALM_INT32 iColIdx;

    ALM_ChkRet_M(pstRslt != ALM_NULL && pRsltRow != ALM_NULL && pcColName != ALM_NULL
                 && pBuf != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    iRet = ALM_TblRsltGetColIdx(pstRslt, pcColName, &iColIdx);
    ALM_CallRet_M(iRet, (ALM_NULL));
    ALM_ChkRet_M(iColIdx >= 0, ALM_ERR_INVALID_PARA,
                 ("Can not find col:\"%s\".", pcColName));

    iRet = ALM_TblRsltGetColByIdx(pstRslt, pRsltRow, iColIdx, pBuf, uiBufLen);
    ALM_CallRet_M(iRet, (ALM_NULL));

    return iRet;
}

/**
* 描述: 初始化查询包，不再使用应使用对应的函数清理。
* 参数:
* pstRslt - 待初始化对象。
* pstCols - 指定要查询的列。
* uiNum - 数据条目的数量。
*/
ALM_INT32 ALM_TblRsltInit(OUT ALM_TBL_RSLT_STRU* pstRslt,
                          CONST ALM_TBL_COLS_STRU* pstCols)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiIdx;

    ALM_ChkRet_M(pstRslt != ALM_NULL && pstCols != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_ChkRet_M(pstCols->uiColNum > 0 && pstCols->pstCols != ALM_NULL,
                 ALM_ERR_INVALID_PARA, (ALM_NULL));

    ALM_OsMemset0_M(pstRslt, ALM_TBL_RSLT_STRU, 1);

    for (uiIdx = 0; uiIdx < pstCols->uiColNum; ++uiIdx)
    {
        iRet = ALM_TblRsltAddCol(pstRslt,
                                 pstCols->pstCols[uiIdx].eIdx,
                                 pstCols->pstCols[uiIdx].acColName,
                                 pstCols->pstCols[uiIdx].eDataType);
        ALM_CallLab_M(LAB_END, (ALM_NULL));
    }

LAB_END:

    if (iRet != ALM_OK)
    {
        ALM_StClsTblRslt(pstRslt);
    }

    return iRet;
}


/* 将列按照列名由小到大排序，并同时排序列内容，若需要会转换字节序 */
ALM_INT32 ALM_TblRsltSort(INOUT ALM_TBL_RSLT_STRU* pstRslt,
                          ALM_BOOL bChangeEndin)
{
    ALM_UINT32 uiIdxRow;
    ALM_UINT32 uiIdxCol;
    ALM_UINT32 uiIdxCol2;
    ALM_TBL_RSLT_COL_UNIT_STRU stColTmp;
    ALM_INT32 iCmp;
    ALM_BOOL bChange = ALM_FALSE;
    ALM_UINT32 uiOffset;
    ALM_CHAR* pRowBk;

    ALM_ChkRet_M(pstRslt != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_OsMemset0_M(&stColTmp, ALM_TBL_RSLT_COL_UNIT_STRU, 1);

    /* 排序列，按名称从小到大排列 */
    for (uiIdxCol = 0; uiIdxCol < pstRslt->uiColNum; ++uiIdxCol)
    {
        for (uiIdxCol2 = 0; uiIdxCol2 < uiIdxCol; ++uiIdxCol2)
        {
            iCmp = strcmp(pstRslt->pstCols[uiIdxCol].pcColName,
                          pstRslt->pstCols[uiIdxCol2].pcColName);

            if (iCmp < 0)
            {
                ALM_OsMemcpy_M(&stColTmp, 1, &pstRslt->pstCols[uiIdxCol2], 1,
                               ALM_TBL_RSLT_COL_UNIT_STRU);

                ALM_OsMemcpy_M(&pstRslt->pstCols[uiIdxCol2], 1,
                               &pstRslt->pstCols[uiIdxCol], 1,
                               ALM_TBL_RSLT_COL_UNIT_STRU);

                ALM_OsMemcpy_M(&pstRslt->pstCols[uiIdxCol], 1,  &stColTmp, 1,
                               ALM_TBL_RSLT_COL_UNIT_STRU);

                bChange = ALM_TRUE;
            }
        }
    }

    /* 重新整理列内容 */
    if ((bChange || bChangeEndin) && pstRslt->uiNum > 0)
    {
        ALM_TBL_RSLT_COL_UNIT_STRU* pstCol;

        pRowBk = ALM_OsMemAlloc_M(ALM_FALSE, ALM_CHAR, pstRslt->uiNodeDataLen);
        ALM_ChkRet_M(pRowBk != ALM_NULL, ALM_ERR_OS_MALLOC_FAILED, (ALM_NULL));

        for (uiIdxRow = 0; uiIdxRow < pstRslt->uiNum; ++uiIdxRow)
        {
            (ALM_VOID)ALM_memcpy_s(pRowBk, pstRslt->uiNodeDataLen,
                                   ALM_TBL_RSLT_IDX(pstRslt, uiIdxRow), pstRslt->uiNodeDataLen);

            uiOffset = 0;

            for (uiIdxCol = 0; uiIdxCol < pstRslt->uiColNum; ++uiIdxCol)
            {
                pstCol = &pstRslt->pstCols[uiIdxCol];

                (ALM_VOID)ALM_memcpy_s(
                    ALM_TBL_RSLT_IDX_OFFSET(pstRslt, uiIdxRow, uiOffset),
                    pstCol->uiDataSize,
                    ALM_TOOL_ADDR_OFFSET(pRowBk, pstCol->uiOffSet),
                    pstCol->uiDataSize);

                if (bChangeEndin && !ALMTblDataCheckStr(pstCol->eDataType))
                {
                    ALM_ToolChangeEndin(
                        ALM_TBL_RSLT_IDX_OFFSET(pstRslt, uiIdxRow, uiOffset),
                        pstCol->uiDataSize);
                }

                uiOffset += pstCol->uiDataSize;
            }
        }

        ALM_AdapterBaseMemFree(pRowBk);
    }

    /* 刷新列offset */
    for (uiIdxCol = 0; uiIdxCol < pstRslt->uiColNum; ++uiIdxCol)
    {
        if (uiIdxCol != 0)
        {
            pstRslt->pstCols[uiIdxCol].uiOffSet =
                pstRslt->pstCols[uiIdxCol - 1].uiOffSet
                + pstRslt->pstCols[uiIdxCol - 1].uiDataSize;
        }
        else
        {
            pstRslt->pstCols[uiIdxCol].uiOffSet = 0;
        }
    }


    return ALM_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */
