/*******************************************************************************
Description    : 实现alm_table_obj.h。
FileName       : alm_table_obj.c
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_table_obj.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


/* 初始化一个表格 */
ALM_INT32 ALM_TblObjInit(OUT ALM_TBL_OBJ_STRU* pstTbl,
                         CONST ALM_CHAR* pcName,
                         ALM_UINT32 uiColNum,
                         CONST ALM_TBL_COL_DIM_STRU* pstColsDim)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_UINT32 uiNameLen = ALM_ToolStrLen(pcName, ALM_CONST_NAME_MAX_LEN + 1);

    /* 参数校验 */
    ALM_ChkRet_M(pstTbl != ALM_NULL && pstColsDim != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_ChkRet_M(uiColNum > 0, ALM_ERR_INVALID_PARA,
                 ("Table needs at least one column."));

    ALM_ChkRet_M(uiNameLen < ALM_CONST_NAME_MAX_LEN + 1,
                 ALM_ERR_INVALID_PARA,
                 ("The table col name length is more then max value."));

    /* 初始化输出 */
    ALM_OsMemset0_M(pstTbl, ALM_TBL_OBJ_STRU, 1);

    /* 初始化表格名 */
    if (pcName != ALM_NULL)
    {
        (ALM_VOID)ALM_strncpy_s(pstTbl->acTblName,
                                sizeof(pstTbl->acTblName) - 1,
                                pcName,
                                uiNameLen);
        pstTbl->acTblName[sizeof(pstTbl->acTblName) - 1] = '\0';
    }

    /* 初始化表格列 */
    iRet = ALM_TblColsInit(&pstTbl->stCols);
    ALM_CallRet_M(iRet, ("Init table cols fail."));

    iRet = ALM_TblColsAddCol(&pstTbl->stCols,
                             uiColNum,
                             pstColsDim);
    ALM_CallLab_M(LAB_END, ("Add table cols fail."));

    /* 初始化表格行 */
    iRet = ALM_TblRowsInit(&pstTbl->stRows, &pstTbl->stCols);
    ALM_CallLab_M(LAB_END, ("Init table rows fail."));


LAB_END:

    if (iRet != ALM_OK)
    {
        ALM_StClsTblObj(pstTbl);
        return iRet;
    }

    return ALM_OK;
}

/* 清理表格，清理后应重新初始化才能使用 */
ALM_VOID ALM_StClsTblObj(INOUT ALM_TBL_OBJ_STRU* pstTbl)
{
    if (ALM_NULL == pstTbl)
    {
        return;
    }

    ALMStClsTblCols(&pstTbl->stCols);
    ALM_StClsTblRows(&pstTbl->stRows);

    ALM_OsMemset0_M(pstTbl, ALM_TBL_OBJ_STRU, 1);
}

/**
* 描述: 向表格中插入数据。对以未指定的列采用默认值，若不存在默认值使用0填充。
* 参数:
* 1. pstTbl - 待插入数据的表格。
* 2. pstRows - 指定新插入数据行中数据列及列对应的值。
*/
ALM_INT32 ALM_TblObjInsert(INOUT ALM_TBL_OBJ_STRU* pstTbl,
                           CONST ALM_TBL_SROW_STRU* pstNewRow)
{
    ALM_INT32 iRet = ALM_OK;

    ALM_ChkRet_M(pstTbl != ALM_NULL && pstNewRow != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_ChkRet_M(pstNewRow->uiMapsNum > 0 && pstNewRow->pstMaps != ALM_NULL,
                 ALM_ERR_INVALID_PARA,
                 ("table insert empty."));

    iRet = ALM_TblRowsInsert(&pstTbl->stRows, pstNewRow);
    ALM_CallRet_M(iRet, ("table obj insert fail."));

    return iRet;
}

/**
* 描述: 删除表格中符合要求的数据行。
* 参数:
* 1. pstTbl - 待删除数据的表格。
* 2. pstCdt - 待删除的数据行应符合的条件，若传入NULL删除全部数据。
*/
ALM_INT32 ALM_TblObjDel(INOUT ALM_TBL_OBJ_STRU* pstTbl,
                        CONST ALM_TBL_CDT_STRU* pstCdt)
{
    ALM_INT32 iRet = ALM_OK;

    ALM_ChkRet_M(pstTbl != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    iRet = ALM_TblRowsDel(&pstTbl->stRows, pstCdt);
    ALM_CallRet_M(iRet, ("table obj delete data fail."));

    return iRet;
}

/**
* 描述: 更新表格中数据行。
* 参数:
* 1. pstTbl - 待更新数据的表格。
* 2. pstCdt - 需要更新数据的数据行应满足的条件，若传入NULL更新全部数据。
* 3. pstNewVal - 指定待更新的列及对应的新值。
*/
ALM_INT32 ALM_TblObjUpdate(INOUT ALM_TBL_OBJ_STRU* pstTbl,
                           CONST ALM_TBL_CDT_STRU* pstCdt,
                           CONST ALM_TBL_SROW_STRU* pstNewVal)
{
    ALM_INT32 iRet = ALM_OK;

    ALM_ChkRet_M(pstTbl != ALM_NULL && pstNewVal != ALM_NULL,
                 ALM_ERR_NULL_PTR,
                 (ALM_NULL));

    ALM_ChkRet_M(pstNewVal->uiMapsNum > 0 && pstNewVal->pstMaps != ALM_NULL,
                 ALM_ERR_INVALID_PARA, (ALM_NULL));

    iRet = ALM_TblRowsUpdate(&pstTbl->stRows, pstCdt, pstNewVal);
    ALM_CallRet_M(iRet, ("table obj update data fail."));

    return iRet;
}


/**
* 描述: 查询表格中数据行。调用该函数后应外部清理查询结果。
* 参数:
* 1. pstTbl - 待更新数据的表格。
* 2. pstCdt - 所得数据行应满足的条件。
* 3. pstRslt - 查询得到的全部数据行，会得到指定列的对应的数据值，若没指定列得到全部列的值。
*/
ALM_INT32 ALM_TblObjQuery(CONST ALM_TBL_OBJ_STRU* pstTbl,
                          CONST ALM_TBL_CDT_STRU* pstCdt,
                          INOUT ALM_TBL_RSLT_STRU* pstRslt)
{
    ALM_INT32 iRet = ALM_OK;

    ALM_ChkRet_M(pstTbl != ALM_NULL && pstRslt != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    iRet = ALM_TblRowsQuery(&pstTbl->stRows, pstCdt, pstRslt);
    ALM_CallRet_M(iRet, ("table obj query data fail."));

    return ALM_OK;
}

/* 得到表格中总数据行数。*/
ALM_INT32 ALM_TblObjTotal(CONST ALM_TBL_OBJ_STRU* pstTbl, ALM_UINT32* puiRowTotal)
{
    ALM_INT32 iRet = ALM_OK;

    ALM_ChkRet_M(pstTbl != ALM_NULL && puiRowTotal != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    iRet = ALM_TblRowsTotal(&pstTbl->stRows, puiRowTotal);
    ALM_CallRet_M(iRet, ("get table rows count fail."));

    return ALM_OK;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
