/*******************************************************************************
Description    : table store实现。
FileName       : alm_table_store.c
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_table_store.h"
#include <string.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

/* 用于保存全部表格信息 */
typedef struct
{
    ALM_UINT32 uiTblNum;
    ALM_UINT32 uiTblCap;
    ALM_TBL_OBJ_STRU** ppstTables;
} ALM_TBLS_STRU;

/* 用于存储全部表格的全局变量 */
static ALM_TBLS_STRU s_stTables = {0, 0, ALM_NULL};

/* 保存临时表格对象，必须指定名称 */
static ALM_INT32 ALMTblStoreSave(ALM_TBL_OBJ_STRU* pstTbl);

/* 得到已创建表格索引，若不存在得到负值。 */
static ALM_INT32 ALMTblStoreGet(CONST ALM_CHAR* pcTblName, ALM_INT32* piIdx);

/* 得到已创建表格 */
static ALM_INT32 ALMTblStoreDel(CONST ALM_CHAR* pcTblName);

/* 校验条件有效性 */
static ALM_BOOL ALMTblCheckCdt(CONST ALM_TBL_OBJ_STRU* pstTblObj,
                               CONST ALM_TBL_CDT_STRU* pstCdt);

/* 得到已创建的表格 */
ALM_TBL_OBJ_STRU** ALM_TblStoreGetAll(OUT ALM_UINT32* puiNum)
{
    ALM_Chk_M(puiNum != ALM_NULL, (ALM_NULL));

    *puiNum = s_stTables.uiTblNum;

    return s_stTables.ppstTables;
}

/* 保存临时表格对象，必须指定名称 */
static ALM_INT32 ALMTblStoreSave(ALM_TBL_OBJ_STRU* pstTbl)
{
    ALM_INT32 iRet;
    ALM_INT32 iTblIdx;

    ALM_ChkRet_M(pstTbl != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    /* 查询是否已经存在 */
    iRet = ALMTblStoreGet(pstTbl->acTblName, &iTblIdx);
    ALM_CallRet_M(iRet, (ALM_NULL));
    ALM_ChkRet_M(iTblIdx < 0, ALM_ERR_INVALID_PARA, ("table exist."));

    /* 扩展存储空间 */
    if (s_stTables.uiTblNum >= s_stTables.uiTblCap)
    {
        ALM_ArrayExpandRet_M(s_stTables.ppstTables,
                             ALM_TBL_OBJ_STRU*,
                             s_stTables.uiTblNum,
                             &s_stTables.uiTblCap,
                             1);

        s_stTables.uiTblCap = (s_stTables.uiTblNum + 1);
    }

    /* 保存新的表格 */
    s_stTables.ppstTables[s_stTables.uiTblNum] = pstTbl;
    ++s_stTables.uiTblNum;

    return ALM_OK;
}

/* 得到已创建表格索引，若不存在得到负值。 */
static ALM_INT32 ALMTblStoreGet(CONST ALM_CHAR* pcTblName, ALM_INT32* piIdx)
{
    ALM_UINT32 uiTblNameLen;
    ALM_UINT32 uiIdx;
    ALM_INT32 iCmp;

    ALM_ChkRet_M(pcTblName != ALM_NULL && piIdx != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    uiTblNameLen = ALM_ToolStrLen(pcTblName, ALM_CONST_NAME_MAX_LEN + 1);
    ALM_ChkRet_M(uiTblNameLen > 0 && uiTblNameLen < ALM_CONST_NAME_MAX_LEN + 1,
                 ALM_ERR_NULL_PTR, ("Table name is invalid."));

    *piIdx = -1;

    for (uiIdx = 0; uiIdx < s_stTables.uiTblNum; ++uiIdx)
    {
        iCmp = strcmp(pcTblName, s_stTables.ppstTables[uiIdx]->acTblName);

        if (0 == iCmp)
        {
            *piIdx = (ALM_INT32)uiIdx;
            break;
        }
    }

    return ALM_OK;
}

/* 得到已创建表格 */
static ALM_INT32 ALMTblStoreDel(CONST ALM_CHAR* pcTblName)
{
    ALM_INT32 idx;
    ALM_INT32 iRet;

    iRet = ALMTblStoreGet(pcTblName, &idx);
    ALM_CallRet_M(iRet, (ALM_NULL));

    if (idx < 0)
    {
        ALM_LogInfo_M(("table is not exist, so can not delete it."));
        return ALM_OK;
    }

    /* 删除表格 */
    ALM_StClsTblObj(s_stTables.ppstTables[idx]);
    ALM_AdapterBaseMemFree(s_stTables.ppstTables[idx]);
    s_stTables.ppstTables[idx] = ALM_NULL;

    if ((ALM_UINT32)idx + 1 < s_stTables.uiTblNum)
    {
        s_stTables.ppstTables[idx] =
            s_stTables.ppstTables[s_stTables.uiTblNum - 1];
        s_stTables.ppstTables[s_stTables.uiTblNum - 1] = ALM_NULL;
    }

    --s_stTables.uiTblNum;


    return ALM_OK;
}

/* 创建表格，不能重名 */
ALM_INT32 ALM_TblNew(CONST ALM_CHAR* pcTblName,
                     ALM_UINT32 uiColNum,
                     CONST ALM_TBL_COL_DIM_STRU* pstColsDim)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_INT32 idx;
    ALM_TBL_OBJ_STRU* pstNew = ALM_NULL;

    ALM_ChkRet_M(pcTblName != ALM_NULL && pstColsDim != ALM_NULL && uiColNum > 0,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    /* 不允许重名表格 */
    iRet = ALMTblStoreGet(pcTblName, &idx);
    ALM_CallRet_M(iRet, (ALM_NULL));
    ALM_ChkRet_M(idx < 0, ALM_ERR_INVALID_PARA, ("table have exist."));

    /* 创建新的表格对象 */
    pstNew = ALM_OsMemAlloc_M(ALM_TRUE, ALM_TBL_OBJ_STRU, 1);
    ALM_ChkRet_M(pstNew != ALM_NULL, ALM_ERR_OS_MALLOC_FAILED, (ALM_NULL));

    iRet = ALM_TblObjInit(pstNew, pcTblName, uiColNum, pstColsDim);
    ALM_CallLab_M(LAB_ERR, (ALM_NULL));

    /* 添加新的表格 */
    iRet = ALMTblStoreSave(pstNew);
    ALM_CallLab_M(LAB_ERR, (ALM_NULL));

LAB_ERR:

    if (iRet != ALM_OK)
    {
        ALM_StClsTblObj(pstNew);
        ALM_AdapterBaseMemFree(pstNew);
    }

    return iRet;
}

/* 删除表格 */
ALM_VOID ALM_TblDel(CONST ALM_CHAR* pcTblName)
{
    ALM_INT32 iRet;

    iRet = ALMTblStoreDel(pcTblName);
    ALM_CallVoid_M((ALM_NULL));

    return ;
}

/**
* 描述: 向表格中插入数据。对以未指定的列采用默认值，若不存在默认值使用0填充。
* 参数:
* 1. pstTbl - 待插入数据的表格。
* 2. pstNewRow - 指定新插入数据行中数据列及列对应的值。
*/
ALM_INT32 ALM_TblInsert(CONST ALM_CHAR* pcTbl,
                        CONST ALM_TBL_SROW_STRU* pstNewRow)
{
    ALM_INT32 idx;
    ALM_INT32 iRet;
    ALM_TBL_OBJ_STRU* pstTblObj = ALM_NULL;

    ALM_ChkRet_M(pcTbl != ALM_NULL && pstNewRow != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    /* 得到表格对象 */
    iRet = ALMTblStoreGet(pcTbl, &idx);
    ALM_CallRet_M(iRet, (ALM_NULL));
    ALM_ChkRet_M(idx >= 0, ALM_ERR_UNKNOW, ("table not exist: %s.", pcTbl));

    pstTblObj = s_stTables.ppstTables[idx];

    /* 执行表格操作 */
    iRet = ALM_TblObjInsert(pstTblObj, pstNewRow);
    ALM_CallRet_M(iRet, (ALM_NULL));

    return ALM_OK;
}

/**
* 描述: 删除表格中符合要求的数据行。
* 参数:
* 1. pstTbl - 待删除数据的表格。
* 2. pstCdt - 待删除的数据行应符合的条件，若传入NULL删除全部数据。
*/
ALM_INT32 ALM_TblRmRows(CONST ALM_CHAR* pcTbl, CONST ALM_TBL_CDT_STRU* pstCdt)
{
    ALM_INT32 idx;
    ALM_INT32 iRet;
    ALM_TBL_OBJ_STRU* pstTblObj = ALM_NULL;

    ALM_ChkRet_M(pcTbl != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    /* 得到表格对象 */
    iRet = ALMTblStoreGet(pcTbl, &idx);
    ALM_CallRet_M(iRet, (ALM_NULL));
    ALM_ChkRet_M(idx >= 0, ALM_ERR_UNKNOW, ("table not exist: %s.", pcTbl));

    pstTblObj = s_stTables.ppstTables[idx];

    /* 条件校验 */
    ALM_ChkRet_M(ALMTblCheckCdt(pstTblObj, pstCdt), ALM_ERR_INVALID_PARA,
                 (ALM_NULL));

    /* 执行表格操作 */
    iRet = ALM_TblObjDel(pstTblObj, pstCdt);
    ALM_CallRet_M(iRet, (ALM_NULL));

    return ALM_OK;
}

/**
* 描述: 更新表格中数据行。
* 参数:
* 1. pstTbl - 待更新数据的表格。
* 2. pstCdt - 需要更新数据的数据行应满足的条件。
* 3. pstNewVal - 指定待更新的列及对应的新值。
*/
ALM_INT32 ALM_TblUpdate(CONST ALM_CHAR* pcTbl,
                        CONST ALM_TBL_CDT_STRU* pstCdt,
                        CONST ALM_TBL_SROW_STRU* pstNewVal)
{
    ALM_INT32 idx;
    ALM_INT32 iRet;
    ALM_TBL_OBJ_STRU* pstTblObj = ALM_NULL;

    ALM_ChkRet_M(pcTbl != ALM_NULL && pstCdt != ALM_NULL && pstNewVal != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    if (0 == pstNewVal->uiMapsNum)
    {
        return ALM_OK;
    }

    /* 得到表格对象 */
    iRet = ALMTblStoreGet(pcTbl, &idx);
    ALM_CallRet_M(iRet, (ALM_NULL));
    ALM_ChkRet_M(idx >= 0, ALM_ERR_UNKNOW, ("table not exist: %s.", pcTbl));

    pstTblObj = s_stTables.ppstTables[idx];

    /* 条件校验 */
    ALM_ChkRet_M(ALMTblCheckCdt(pstTblObj, pstCdt), ALM_ERR_INVALID_PARA,
                 (ALM_NULL));

    /* 执行表格操作 */
    iRet = ALM_TblObjUpdate(pstTblObj, pstCdt, pstNewVal);
    ALM_CallRet_M(iRet, (ALM_NULL));

    return ALM_OK;
}

/**
* 描述: 查询表格中数据行。调用该函数后应清理查询结果。
* 参数:
* 1. pstTbl - 待更新数据的表格。
* 2. pstCdt - 所得数据行应满足的条件。
* 3. pstRslt - 查询得到的全部数据行，可以指定要查询的列。
*/
ALM_INT32 ALM_TblQuery(CONST ALM_CHAR* pcTbl,
                       CONST ALM_TBL_CDT_STRU* pstCdt,
                       INOUT ALM_TBL_RSLT_STRU* pstRslt)
{
    ALM_INT32 idx;
    ALM_INT32 iRet;
    ALM_TBL_OBJ_STRU* pstTblObj = ALM_NULL;

    ALM_ChkRet_M(pcTbl != ALM_NULL && pstRslt != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    /* 得到表格对象 */
    iRet = ALMTblStoreGet(pcTbl, &idx);
    ALM_CallRet_M(iRet, (ALM_NULL));
    ALM_ChkRet_M(idx >= 0, ALM_ERR_UNKNOW, ("table not exist: %s.", pcTbl));

    pstTblObj = s_stTables.ppstTables[idx];

    /* 条件校验 */
    ALM_ChkRet_M(ALMTblCheckCdt(pstTblObj, pstCdt), ALM_ERR_INVALID_PARA,
                 (ALM_NULL));

    /* 执行表格操作 */
    iRet = ALM_TblObjQuery(pstTblObj, pstCdt, pstRslt);
    ALM_CallRet_M(iRet, (ALM_NULL));

    /*赋值列索引*/
    pstRslt->iTblIdx = idx;

    return ALM_OK;
}

/* 得到表格中数据总行数。*/
ALM_INT32 ALM_TblTotal(CONST ALM_CHAR* pcTbl, ALM_UINT32* puiRowCount)
{
    ALM_INT32 idx;
    ALM_INT32 iRet;
    ALM_TBL_OBJ_STRU* pstTblObj = ALM_NULL;

    ALM_ChkRet_M(pcTbl != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    /* 得到表格对象 */
    iRet = ALMTblStoreGet(pcTbl, &idx);
    ALM_CallRet_M(iRet, (ALM_NULL));
    ALM_ChkRet_M(idx >= 0, ALM_ERR_UNKNOW, ("table not exist: %s.", pcTbl));

    pstTblObj = s_stTables.ppstTables[idx];

    /* 执行表格操作 */
    iRet = ALM_TblObjTotal(pstTblObj, puiRowCount);
    ALM_CallRet_M(iRet, (ALM_NULL));

    return ALM_OK;
}

/* 清楚全部表格 */
ALM_VOID ALM_TblDropAll()
{
    ALM_UINT32 uiIdx;

    for (uiIdx = 0; uiIdx < s_stTables.uiTblNum; ++uiIdx)
    {
        ALM_StClsTblObj(s_stTables.ppstTables[uiIdx]);
        ALM_AdapterBaseMemFree(s_stTables.ppstTables[uiIdx]);
    }

    ALM_AdapterBaseMemFree(s_stTables.ppstTables);
    s_stTables.ppstTables = ALM_NULL;
    s_stTables.uiTblCap = 0;
    s_stTables.uiTblNum = 0;
}

/* 清除全部表格内容 */
ALM_VOID ALM_TblClsAll()
{
    ALM_UINT32 uiIdx;

    for (uiIdx = 0; uiIdx < s_stTables.uiTblNum; ++uiIdx)
    {
        (ALM_VOID)ALM_TblObjDel(s_stTables.ppstTables[uiIdx], ALM_NULL);
    }
}


/* 校验条件有效性 */
static ALM_BOOL ALMTblCheckCdt(CONST ALM_TBL_OBJ_STRU* pstTblObj,
                               CONST ALM_TBL_CDT_STRU* pstCdt)
{
    ALM_INT32 iCmp;
    ALM_UINT32 uiIdx;
    ALM_UINT32 uiIdxCol;

    ALM_ChkRet_M(pstTblObj != ALM_NULL, ALM_FALSE, ("Null pointer"));

    if (ALM_NULL == pstCdt)
    {
        return ALM_TRUE;
    }

    for (uiIdxCol = 0; uiIdxCol < pstCdt->stCols.uiMapsNum; ++uiIdxCol)
    {
        for (uiIdx = 0; uiIdx < pstTblObj->stCols.uiColNum; ++uiIdx)
        {
            iCmp = strcmp(pstTblObj->stCols.pstCols[uiIdx].acColName,
                          pstCdt->stCols.pstMaps[uiIdxCol].acColName);

            if (0 == iCmp)
            {
                break;
            }
        }

        ALM_ChkRet_M(uiIdx < pstTblObj->stCols.uiColNum, ALM_FALSE,
                     ("Do not exist col \"%s\" of table \"%s\".",
                      pstCdt->stCols.pstMaps[uiIdxCol].acColName,
                      pstTblObj->acTblName));
    }

    return ALM_TRUE;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */
