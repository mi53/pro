/*******************************************************************************
Description    : 使用多叉树实现存储。
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#include "alm_store_tree.inc"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /*ifdef __cplusplus */
#endif /*if __cplusplus*/


/* 清理ALM_STORE_TREE_RSLT_STRU对象 */
ALM_VOID ALM_StClsStoreTreeRslt(INOUT ALM_STORE_TREE_RSLT_STRU* pstRslt)
{
    ALM_STORE_TREE_CLS_DATA_FUNC fnDataCls;
    ALM_UINT32 uiIdx;

    if (ALM_NULL != pstRslt && pstRslt->pstTree != ALM_NULL)
    {
        fnDataCls = pstRslt->pstTree->stStoreCfg.stAdapterFuncs.fnDataCls;

        if (fnDataCls != ALM_NULL)
        {
            for (uiIdx = 0; uiIdx < pstRslt->uiNum; ++uiIdx)
            {
                fnDataCls(
                    (ALM_CHAR*)pstRslt->pDatas + pstRslt->uiDataSize * uiIdx,
                    pstRslt->pstTree->stStoreCfg.pEx);
            }
        }
        else
        {
            if (pstRslt->uiNum > 0)
            {
                (ALM_VOID)ALM_memset_s(pstRslt->pDatas,
                                       pstRslt->uiDataSize * pstRslt->uiNum,
                                       0,
                                       pstRslt->uiDataSize * pstRslt->uiNum);
            }
        }

        ALM_AdapterBaseMemFree(pstRslt->pDatas);
        ALM_OsMemset0_M(pstRslt, ALM_STORE_TREE_RSLT_STRU, 1);
    }
}

/* 初始化多叉树 */
ALM_INT32 ALM_StoreTreeInit(OUT ALM_STORE_TREE_STRU* pstTree,
                            CONST ALM_STORE_TREE_CFG_STRU* pstStoreInfo)
{
    ALM_ChkRet_M(pstTree != ALM_NULL && pstStoreInfo != ALM_NULL, ALM_ERR_NULL_PTR,
                 (ALM_NULL));
    ALM_ChkRet_M(ALMStoreTreeCfgCheck(pstStoreInfo), ALM_ERR_INVALID_PARA,
                 (ALM_NULL));

    ALM_OsMemset0_M(pstTree, ALM_STORE_TREE_STRU, 1);

    ALM_OsMemcpy_M(&pstTree->stStoreCfg, 1, pstStoreInfo, 1,
                   ALM_STORE_TREE_CFG_STRU);

    return ALM_OK;
}

/* 清除多叉树*/
ALM_VOID ALM_StClsStoreTree(INOUT ALM_STORE_TREE_STRU* pstTree)
{
    if (pstTree != ALM_NULL)
    {
        ALM_StoreTreeDelAll(pstTree);
        ALM_OsMemset0_M(pstTree, ALM_STORE_TREE_STRU, 1);
    }
}

/* 得到数据总量 */
ALM_INT32 ALM_StoreTreeGetTotal(CONST ALM_STORE_TREE_STRU* pstTree,
                                ALM_UINT32* puiCnt)
{
    ALM_ChkRet_M(pstTree != ALM_NULL && puiCnt != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    if (ALM_NULL == pstTree->pstRoot)
    {
        *puiCnt = 0;
    }
    else
    {
        *puiCnt = pstTree->pstRoot->uiNodeNum;
    }

    return ALM_OK;
}

/*
* 描述:删除指定数据
* pstTree - 树
* pMark - 待删除数据
*/
ALM_INT32 ALM_StoreTreeDel(INOUT ALM_STORE_TREE_STRU* pstTree,
                           CONST ALM_VOID* pMark)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_STORE_TREE_WORK_STRU stWork;
    ALM_STORE_TREE_FIND_OPT_STRU stOpt;
    ALM_STORE_TREE_NODES_STRU stNextBuf;
    ALM_BOOL bNeedContinue = ALM_TRUE;
    ALM_BOOL bSearchLeaf = ALM_TRUE;

    ALM_ChkRet_M(pstTree != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_OsMemset0_M(&stWork, ALM_STORE_TREE_WORK_STRU, 1);
    ALM_OsMemset0_M(&stOpt, ALM_STORE_TREE_FIND_OPT_STRU, 1);
    ALM_OsMemset0_M(&stNextBuf, ALM_STORE_TREE_NODES_STRU, 1);
    /* 查找结点,先删叶子节点，再删非叶子节点 */
    /* 删除的过程中，先找到符合条件的叶子节点删除，若找不到，再找非叶子节点删除 */
    stOpt.bFindByMark = ALM_TRUE;
    stOpt.bOnlyFindOnce = ALM_TRUE;
    stOpt.bOnlyGetLeaf = ALM_TRUE;

    /* 先扩展部分空间，解决扫描NULL指针问题 */
    ALM_ArrayObjExpandRet_M(stNextBuf, ALM_STORE_TREE_NODE_STRU*, 1);

    while (bNeedContinue)
    {
        iRet = ALMStoreTreeFind(pstTree, pMark, &stOpt, &stWork);
        ALM_CallLab_M(LAB_END, (ALM_NULL));

        if (0 == stWork.stEquals.uiNum)
        {
            if (bSearchLeaf)
            {
                stOpt.bOnlyGetLeaf = ALM_FALSE;
                bSearchLeaf = ALM_FALSE;
            }
            else
            {
                bNeedContinue = ALM_FALSE;
            }
        }
        else
        {
            ALM_Chk_M(1 == stWork.stEquals.uiNum, (ALM_NULL));

            iRet = ALMStoreTreeDelNode(pstTree,
                                       stWork.stEquals.pstArray[0],
                                       &stWork,
                                       &stNextBuf);
            ALM_CallLab_M(LAB_END, (ALM_NULL));

            ALMStoreTreeNodeFree(stWork.stEquals.pstArray[0], pstTree);
        }
    }

LAB_END:
    ALMStClsStoreTreeWork(&stWork);
    ALM_ArrayObjCls_M(stNextBuf);
    ALMChkTree_M(pstTree);
    return iRet;
}


/* 清理全部数据，较ALM_StoreTreeDel效率更高，无论是否出错均强制清空 */
ALM_VOID ALM_StoreTreeDelAll(INOUT ALM_STORE_TREE_STRU* pstTree)
{
    ALM_INT32 iRet;
    ALM_STORE_TREE_NODE_STRU** apstWaitDels = ALM_NULL;
    ALM_UINT32 uiNum = 0;
    ALM_STORE_TREE_NODE_STRU* pstDel;
    ALM_UINT32 uiIdx;

    ALM_ChkVoid_M(pstTree != ALM_NULL, (ALM_NULL));

    /* 清理全部结点 */
    pstDel = pstTree->pstRoot;

    while (pstDel != ALM_NULL)
    {
        /* 添加下层NODE到待删除链 */
        ALM_ArrayExpandLab_M(apstWaitDels,
                             ALM_STORE_TREE_NODE_STRU*,
                             uiNum,
                             ALM_NULL,
                             pstTree->stStoreCfg.usPortNum,
                             LAB_END);

        if (ALM_NULL == apstWaitDels)
        {
            return;
        }

        for (uiIdx = 0; uiIdx < pstTree->stStoreCfg.usPortNum; ++uiIdx)
        {
            if (pstDel->apstPorts[uiIdx] != ALM_NULL)
            {
                apstWaitDels[uiNum] = pstDel->apstPorts[uiIdx];
                ++uiNum;
            }
        }

        /* 删除结点 */
        ALMStoreTreeNodeFree(pstDel, pstTree);

        /* 迭代下一个待删除项 */
        if (uiNum > 0)
        {
            pstDel = apstWaitDels[uiNum - 1];
            --uiNum;
        }
        else
        {
            pstDel = ALM_NULL;
        }
    }

LAB_END:
    ALM_AdapterBaseMemFree(apstWaitDels);

    /* 树结构已经紊乱，清理失败亦强制清空 */
    pstTree->pstRoot = ALM_NULL;
}

/* 添加数据 */
ALM_INT32 ALM_StoreTreeAdd(INOUT ALM_STORE_TREE_STRU* pstTree,
                           CONST ALM_VOID* pData)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_STORE_TREE_WORK_STRU stWork;
    ALM_STORE_TREE_NODES_STRU stNextBuf;
    ALM_STORE_TREE_NODE_STRU* pstNew = ALM_NULL;

    ALM_ChkRet_M(pstTree != ALM_NULL && pData != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_OsMemset0_M(&stWork, ALM_STORE_TREE_WORK_STRU, 1);
    ALM_OsMemset0_M(&stNextBuf, ALM_STORE_TREE_NODES_STRU, 1);
    /* 创建并添加结点 */
    iRet = ALMStoreTreeNodeNew(&pstNew, pstTree, pData);
    ALM_CallLab_M(LAB_END, (ALM_NULL));

    iRet = ALMStoreTreeAddNode(pstTree, pstNew, &stWork);
    ALM_CallLab_M(LAB_END, (ALM_NULL));

    /* 均衡化处理 */
    iRet = ALMStoreTreeDoBlanch(pstTree, pstNew, &stWork, &stNextBuf);
    ALM_CallLab_M(LAB_END, (ALM_NULL));

LAB_END:
    ALMStClsStoreTreeWork(&stWork);
    ALM_ArrayObjCls_M(stNextBuf);
    ALMChkTree_M(pstTree);
    return iRet;
}


/* 查找数据，得到数据的拷贝 */
ALM_INT32 ALM_StoreTreeGet(CONST ALM_STORE_TREE_STRU* pstTree,
                           CONST ALM_VOID* pMark,
                           OUT ALM_STORE_TREE_RSLT_STRU* pstRslt)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_STORE_TREE_NODES_STRU stNodes;
    ALM_UINT32 uiIdx;

    ALM_ChkRet_M(pstTree != ALM_NULL && pstRslt != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_OsMemset0_M(&stNodes, ALM_STORE_TREE_NODES_STRU, 1);
    ALM_OsMemset0_M(pstRslt, ALM_STORE_TREE_RSLT_STRU, 1);

    pstRslt->pstTree = pstTree;

    if (ALM_NULL == pstTree->pstRoot)
    {
        return ALM_OK;
    }

    /* 查找结点 */
    iRet = ALM_StoreTreeGetNodes(pstTree, pMark, &stNodes);
    ALM_CallLab_M(LAB_END, (ALM_NULL));

    /* 结果转换 */
    if (stNodes.uiNum > 0)
    {
        iRet = ALM_ToolArrayExpand((ALM_VOID**)&pstRslt->pDatas,
                                   pstTree->stStoreCfg.uiUnitSize,
                                   pstRslt->uiNum,
                                   stNodes.uiNum);
        ALM_CallLab_M(LAB_END, (ALM_NULL));

        pstRslt->uiCap = stNodes.uiNum;
        pstRslt->uiDataSize = pstTree->stStoreCfg.uiUnitSize;

        for (uiIdx = 0; uiIdx < stNodes.uiNum; ++uiIdx)
        {
            if (ALM_NULL != pstTree->stStoreCfg.stAdapterFuncs.fnDataInit)
            {
                iRet = pstTree->stStoreCfg.stAdapterFuncs.fnDataInit(
                           (ALM_CHAR*)pstRslt->pDatas + (uiIdx * pstRslt->uiDataSize),
                           pstTree->stStoreCfg.pEx);
                ALM_CallLab_M(LAB_END, (ALM_NULL));
            }

            if (ALM_NULL != pstTree->stStoreCfg.stAdapterFuncs.fnDataCpy)
            {
                iRet = pstTree->stStoreCfg.stAdapterFuncs.fnDataCpy(
                           (ALM_CHAR*)pstRslt->pDatas + (uiIdx * pstRslt->uiDataSize),
                           stNodes.pstArray[uiIdx]->pData,
                           pstTree->stStoreCfg.pEx);
                ALM_CallLab_M(LAB_END, (ALM_NULL));
            }
            else
            {
                (ALM_VOID)ALM_memcpy_s(
                    (ALM_CHAR*)pstRslt->pDatas + (uiIdx * pstRslt->uiDataSize),
                    pstRslt->uiDataSize,
                    stNodes.pstArray[uiIdx]->pData,
                    pstTree->stStoreCfg.uiUnitSize);
            }
        }

        pstRslt->uiNum = stNodes.uiNum;
    }

LAB_END:
    ALM_ArrayObjCls_M(stNodes);

    if (iRet != ALM_OK)
    {
        ALM_StClsStoreTreeRslt(pstRslt);
    }

    return iRet;
}


/* 查找数据并得到数据所在结点，谨慎修改所得数据，否则会导致树结构紊乱（仅在不影响索引时修改） */
ALM_INT32 ALM_StoreTreeGetNodes(CONST ALM_STORE_TREE_STRU* pstTree,
                                CONST ALM_VOID* pMark,
                                OUT ALM_STORE_TREE_NODES_STRU* pstRslt)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_STORE_TREE_WORK_STRU stWork;
    ALM_STORE_TREE_FIND_OPT_STRU stOpt;

    ALM_ChkRet_M(pstTree != ALM_NULL && pstRslt != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_OsMemset0_M(pstRslt, ALM_STORE_TREE_NODES_STRU, 1);
    ALM_OsMemset0_M(&stWork, ALM_STORE_TREE_WORK_STRU, 1);
    ALM_OsMemset0_M(&stOpt, ALM_STORE_TREE_FIND_OPT_STRU, 1);

    if (ALM_NULL == pstTree->pstRoot)
    {
        return ALM_OK;
    }

    /* 查找结点 */
    stOpt.bFindByMark = ALM_TRUE;
    iRet = ALMStoreTreeFind((ALM_STORE_TREE_STRU*)pstTree, pMark, &stOpt,
                            &stWork);
    ALM_CallLab_M(LAB_END, (ALM_NULL));


    ALM_OsMemcpy_M(pstRslt, 1, &stWork.stEquals, 1, ALM_STORE_TREE_NODES_STRU);

    ALM_OsMemset0_M(&stWork.stEquals, ALM_STORE_TREE_NODES_STRU, 1);

LAB_END:
    ALMStClsStoreTreeWork(&stWork);
    return iRet;
}

/*
* 描述: 更新指定数据
* pstTree - 树
* pstOld - old数据
* pNewData - new数据
*/
ALM_INT32 ALM_StoreTreeUpdateNode(INOUT ALM_STORE_TREE_STRU* pstTree,
                                  IN ALM_STORE_TREE_NODE_STRU* pstOld,
                                  CONST ALM_VOID* pNewData)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_STORE_TREE_WORK_STRU stWork;
    ALM_STORE_TREE_NODES_STRU stNextBuf;
    ALM_BOOL bEqual;

    ALM_Chk_M(pstTree != ALM_NULL && pstOld != ALM_NULL && pNewData != ALM_NULL, (ALM_NULL));

    ALM_OsMemset0_M(&stWork, ALM_STORE_TREE_WORK_STRU, 1);
    ALM_OsMemset0_M(&stNextBuf, ALM_STORE_TREE_NODES_STRU, 1);

    /* 初始化比较对象 */
    ALM_ArrayExpandLab_M(stWork.stCmp.ausNexts,
                         ALM_UINT16,
                         stWork.stCmp.uiNum,
                         &stWork.stCmp.uiCap,
                         pstTree->stStoreCfg.usPortNum,
                         LAB_END);

    /* 比较数据，若为等值直接拷贝 */
    iRet = pstTree->stStoreCfg.stAdapterFuncs.fnGetPort(pstOld->pData,
            ALM_FALSE,
            pNewData,
            &stWork.stCmp,
            pstTree->stStoreCfg.pEx);
    ALM_CallLab_M(LAB_END, (ALM_NULL));

    if (stWork.stCmp.bThisIsEqual)
    {
        goto LAB_END;
    }

    if (ALM_STORE_TREE_EQUAL_PORT == stWork.stCmp.ausNexts[0])
    {
        bEqual = ALM_TRUE;
    }
    else
    {
        bEqual = ALM_FALSE;
    }

    /* 不相等，需要更新索引位置，删除 */
    if (!bEqual)
    {
        iRet = ALMStoreTreeDelNode(pstTree, pstOld, &stWork, &stNextBuf);
        ALM_CallLab_M(LAB_END, (ALM_NULL));
    }

    /* 更新数据 */
    iRet = pstTree->stStoreCfg.stAdapterFuncs.fnDataCpy(
               pstOld->pData,
               pNewData,
               pstTree->stStoreCfg.pEx);
    ALM_CallLab_M(LAB_END, (ALM_NULL));

    /* 不相等，需要更新索引位置，重新添加 */
    if (!bEqual)
    {
        iRet = ALMStoreTreeAddNode(pstTree, pstOld, &stWork);
        ALM_CallLab_M(LAB_END, (ALM_NULL));
    }

LAB_END:
    ALMStClsStoreTreeWork(&stWork);
    ALM_AdapterBaseMemFree(stNextBuf.pstArray);
    return iRet;
}


/* 创建节点 */
static ALM_INT32 ALMStoreTreeNodeNew(OUT ALM_STORE_TREE_NODE_STRU** ppstNewNode,
                                     CONST ALM_STORE_TREE_STRU* pstTree,
                                     CONST ALM_VOID* pData)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_STORE_TREE_NODE_STRU* pstNew = ALM_NULL;

    ALM_ChkRet_M(ppstNewNode != ALM_NULL && pstTree != ALM_NULL, ALM_ERR_NULL_PTR,
                 (ALM_NULL));

    *ppstNewNode = ALM_NULL;

    /* 结点本身 */
    pstNew = ALM_OsMemAlloc_M(ALM_TRUE, ALM_STORE_TREE_NODE_STRU, 1);
    ALM_ChkRet_M(pstNew != ALM_NULL, ALM_ERR_OS_MALLOC_FAILED, (ALM_NULL));

    pstNew->uiNodeNum = 1;

    /* 索引 */
    pstNew->apstPorts = ALM_OsMemAlloc_M(ALM_TRUE, ALM_STORE_TREE_NODE_STRU*,
                                         pstTree->stStoreCfg.usPortNum);
    ALM_ChkLab_M(!(ALM_NULL == pstNew->apstPorts
                   && (iRet = ALM_ERR_OS_MALLOC_FAILED) != ALM_OK),
                 LAB_END, (ALM_NULL));

    /* 数据 */
    pstNew->pData = ALM_OsMemAlloc_M(ALM_FALSE, ALM_CHAR,
                                     pstTree->stStoreCfg.uiUnitSize);
    ALM_ChkLab_M(!(ALM_NULL == pstNew->pData
                   && (iRet = ALM_ERR_OS_MALLOC_FAILED) != ALM_OK),
                 LAB_END, (ALM_NULL));

    if (pstTree->stStoreCfg.stAdapterFuncs.fnDataInit != ALM_NULL)
    {
        iRet = pstTree->stStoreCfg.stAdapterFuncs.fnDataInit(pstNew->pData,
                pstTree->stStoreCfg.pEx);
        ALM_CallLab_M(LAB_END, (ALM_NULL));
    }

    if (pstTree->stStoreCfg.stAdapterFuncs.fnDataCpy)
    {
        iRet = pstTree->stStoreCfg.stAdapterFuncs.fnDataCpy(pstNew->pData,
                pData,
                pstTree->stStoreCfg.pEx);
        ALM_CallLab_M(LAB_END, (ALM_NULL));
    }
    else
    {
        (ALM_VOID)ALM_memcpy_s(pstNew->pData, pstTree->stStoreCfg.uiUnitSize,
                               pData, pstTree->stStoreCfg.uiUnitSize);
    }

LAB_END:

    if (iRet != ALM_OK)
    {
        ALMStoreTreeNodeFree(pstNew, pstTree);
    }
    else
    {
        *ppstNewNode = pstNew;
    }

    return iRet;
}

/* 释放节点，仅释放结点本身 */
static ALM_VOID ALMStoreTreeNodeFree(INOUT ALM_STORE_TREE_NODE_STRU* pstDelNode,
                                     CONST  ALM_STORE_TREE_STRU* pstTree)
{
    if (ALM_NULL != pstTree && ALM_NULL != pstDelNode)
    {
        if (ALM_NULL != pstDelNode->pData)
        {
            if (ALM_NULL != pstTree->stStoreCfg.stAdapterFuncs.fnDataCls)
            {
                pstTree->stStoreCfg.stAdapterFuncs.fnDataCls(pstDelNode->pData,
                        pstTree->stStoreCfg.pEx);
            }

            ALM_AdapterBaseMemFree(pstDelNode->pData);
        }

        ALM_AdapterBaseMemFree(pstDelNode->apstPorts);
        ALM_AdapterBaseMemFree(pstDelNode);
    }
}


/* 校验store info的有效性 */
static ALM_BOOL ALMStoreTreeCfgCheck(CONST ALM_STORE_TREE_CFG_STRU* pstStoreInfo)
{
    ALM_Chk_M(pstStoreInfo != ALM_NULL, (ALM_NULL));

    ALM_ChkRet_M(pstStoreInfo->uiUnitSize > 0, ALM_FALSE, (ALM_NULL));
    ALM_ChkRet_M(pstStoreInfo->usPortNum > 0, ALM_FALSE, (ALM_NULL));
    ALM_ChkRet_M(pstStoreInfo->stAdapterFuncs.fnGetPort != ALM_NULL, ALM_FALSE, (ALM_NULL));

    return ALM_TRUE;
}

/*
*描述:查询数据项结点及路径
* pstTree - 树
* pMark - 标记是查询条件还是数据，条件为TRUE
* pstOpt - 查找选项
* pstWork - 查找后得到的数据项节点工作集
*/
static ALM_INT32 ALMStoreTreeFind(INOUT ALM_STORE_TREE_STRU* pstTree,
                                  CONST ALM_VOID* pMark,
                                  CONST ALM_STORE_TREE_FIND_OPT_STRU* pstOpt,
                                  INOUT ALM_STORE_TREE_WORK_STRU* pstWork)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_STORE_TREE_NODE_STRU* pstItr;
    ALM_UINT32 uiIdx;
    ALM_STORE_TREE_NODE_STRU* pstNext;
    ALM_UINT16 usNextPort;

    ALM_ChkRet_M(pstTree != ALM_NULL && pstOpt != ALM_NULL && pstWork != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_ChkRet_M(!(pstOpt->bOnlyFindOnce) || !(pstOpt->bOnlyGetLast),
                 ALM_ERR_INVALID_PARA, (ALM_NULL));

    /* 初始化work对象 */
    pstWork->pstInsertLast = ALM_NULL;
    pstWork->stEquals.uiNum = 0;
    pstWork->stNexts.uiNum = 0;
    pstWork->stCmp.uiNum = 0;

    if (pstWork->stCmp.uiCap < pstTree->stStoreCfg.usPortNum)
    {
        ALM_ArrayExpandRet_M(pstWork->stCmp.ausNexts,
                             ALM_UINT16,
                             pstWork->stCmp.uiNum,
                             &pstWork->stCmp.uiCap,
                             pstTree->stStoreCfg.usPortNum);
    }

    /* 查找数据 */
    pstItr = pstTree->pstRoot;

    while (pstItr != ALM_NULL)
    {
        iRet = pstTree->stStoreCfg.stAdapterFuncs.fnGetPort(pMark,
                pstOpt->bFindByMark,
                pstItr->pData,
                &pstWork->stCmp,
                pstTree->stStoreCfg.pEx);
        ALM_CallRet_M(iRet, (ALM_NULL));

        if (pstWork->stCmp.bThisIsEqual && !pstOpt->bOnlyGetLast && !pstOpt->bOnlyGetLeaf)
        {
            ALM_ArrayObjAddRet_M(pstWork->stEquals,
                                 ALM_STORE_TREE_NODE_STRU*,
                                 &pstItr,
                                 1);

            if (pstOpt->bOnlyFindOnce || !pstTree->stStoreCfg.bIncSame)
            {
                return ALM_OK;
            }
        }
        else if (pstWork->stCmp.bThisIsEqual && pstOpt->bOnlyGetLeaf)
        {
            /* 判断是否为叶子 */
            if (pstItr->uiNodeNum <= 1 ||
                (pstItr->apstPorts[ALM_STORE_TREE_EQUAL_PORT] != ALM_NULL &&
                 pstItr->uiNodeNum ==
                 pstItr->apstPorts[ALM_STORE_TREE_EQUAL_PORT]->uiNodeNum + 1))
            {
                ALM_ArrayObjAddRet_M(pstWork->stEquals,
                                     ALM_STORE_TREE_NODE_STRU*,
                                     &pstItr,
                                     1);

                if (pstOpt->bOnlyFindOnce)
                {
                    return ALM_OK;
                }
            }
        }

        /* 添加待迭代项 */
        for (uiIdx = 0; uiIdx < pstWork->stCmp.uiNum; ++uiIdx)
        {
            usNextPort = pstWork->stCmp.ausNexts[uiIdx];
            pstNext = pstItr->apstPorts[usNextPort];

            if (pstNext != ALM_NULL)
            {
                ALM_ArrayObjAddRet_M(pstWork->stNexts,
                                     ALM_STORE_TREE_NODE_STRU*,
                                     &pstNext,
                                     1);

                /* 优化单链表或单链子树的插入性能 */
                if (pstOpt->bOnlyGetLast
                    && ALM_STORE_TREE_EQUAL_PORT == usNextPort
                    && pstItr->uiNodeNum == pstNext->uiNodeNum + 1)
                {
                    pstWork->pstInsertLast = pstItr;
                    pstWork->usInsertPort = usNextPort;

                    return ALM_OK;
                }
            }
            else
            {
                /* 尽量在上层添加新成员 */
                if (ALM_NULL == pstWork->pstInsertLast)
                {
                    pstWork->pstInsertLast = pstItr;
                    pstWork->usInsertPort = usNextPort;

                    if (pstOpt->bOnlyGetLast)
                    {
                        return ALM_OK;
                    }
                }
            }
        }

        /* 向下迭代 */
        if (pstWork->stNexts.uiNum > 0)
        {
            pstItr = pstWork->stNexts.pstArray[--pstWork->stNexts.uiNum];
        }
        else
        {
            pstItr = ALM_NULL;
        }
    }

    return iRet;
}


/*
* 描述:移除树的顶层元素，并将最大子树作为根结点；同时更新原有根结点计数及子树，但不会释放结点
* 会更新原有根结点的子成员及计数
* pstTree - 树
* bForce - ALM_TRUE:在顶层元素满足均衡性时依旧移除
* pstWork - 数据项节点工作集
* pstNextBuf - 存储需要迭代的节点
*/
static ALM_INT32 ALMStoreTreeRmRoot(INOUT ALM_STORE_TREE_STRU* pstTree,
                                    ALM_BOOL bForce,
                                    IN ALM_STORE_TREE_WORK_STRU* pstWork,
                                    IN ALM_STORE_TREE_NODES_STRU* pstNextBuf)
{
    ALM_INT32 iRet;
    ALM_UINT16 usIdx;
    ALM_UINT16 usMaxPort = 0;
    ALM_STORE_TREE_NODE_STRU* pstMaxSub;
    ALM_STORE_TREE_NODE_STRU* pstTmp;

    ALM_Chk_M(pstTree != ALM_NULL && pstWork != ALM_NULL, (ALM_NULL));

    if (ALM_NULL == pstTree->pstRoot)
    {
        return ALM_OK;
    }

    /* 将最大子树上移 */
    pstMaxSub = ALM_NULL;

    for (usIdx = 0; usIdx < pstTree->stStoreCfg.usPortNum; ++usIdx)
    {
        pstTmp = pstTree->pstRoot->apstPorts[usIdx];

        if (pstTmp != ALM_NULL)
        {
            if (ALM_NULL == pstMaxSub || pstTmp->uiNodeNum > pstMaxSub->uiNodeNum)
            {
                pstMaxSub = pstTmp;
                usMaxPort = usIdx;
            }
        }
    }

    if (ALM_NULL == pstMaxSub) /* 不存在子树 */
    {
        pstTree->pstRoot = ALM_NULL;
        return ALM_OK;
    }

    /* 均衡性校验 */
    if (!bForce && pstMaxSub->usLastPort != ALM_STORE_TREE_EQUAL_PORT
        && ALM_STORE_TREE_CHECK_BLANCH(pstTree->pstRoot->uiNodeNum,
                                       pstMaxSub->uiNodeNum))
    {
        return ALM_OK;
    }

    /* 移除顶层元素，并重组为新的树 */
    pstTmp = pstTree->pstRoot;
    pstTmp->apstPorts[usMaxPort] = ALM_NULL;

    pstTree->pstRoot = pstMaxSub;
    pstTree->pstRoot->pstLast = ALM_NULL;

    /* 添加子树 */
    for (usIdx = 0; usIdx < pstTree->stStoreCfg.usPortNum; ++usIdx)
    {
        if (pstTmp->apstPorts[usIdx] != ALM_NULL)
        {
            iRet = ALMStoreTreeAddNodeAndSub(pstTree,
                                             pstTmp->apstPorts[usIdx],
                                             pstWork,
                                             pstNextBuf);
            ALM_CallRet_M(iRet, (ALM_NULL));

            pstTmp->apstPorts[usIdx] = ALM_NULL;
        }
    }

    pstTmp->uiNodeNum = 1;

    return ALM_OK;
}


/*
*描述: 添加结点，不删除新增结点的后续成员（在使用该函数时，只有等值链才应该保留后续成员
* pstTree - 树
* pstAdd - 待添加节点
* pstWork - 数据项节点工作集
*/
static ALM_INT32 ALMStoreTreeAddNode(INOUT ALM_STORE_TREE_STRU* pstTree,
                                     IN ALM_STORE_TREE_NODE_STRU* pstAdd,
                                     IN ALM_STORE_TREE_WORK_STRU* pstWork)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_STORE_TREE_FIND_OPT_STRU stOpt;
    ALM_STORE_TREE_NODE_STRU* pstItr;
    ALM_STORE_TREE_NODE_STRU* pstTmp;
    ALM_UINT32 uiAddNum;

    ALM_ChkRet_M(pstTree != ALM_NULL && pstAdd != ALM_NULL && pstWork != ALM_NULL,
                 ALM_ERR_NULL_PTR, (ALM_NULL));

    ALM_Chk_M(pstAdd->pstLast == ALM_NULL, (ALM_NULL));

    ALM_OsMemset0_M(&stOpt, ALM_STORE_TREE_FIND_OPT_STRU, 1);

    /* 若同时插入多个数据，只能插入等值链 */
    uiAddNum = pstAdd->uiNodeNum;
    pstTmp = pstAdd->apstPorts[ALM_STORE_TREE_EQUAL_PORT];

    ALM_Chk_M(1 == uiAddNum
              || (pstTmp != ALM_NULL && (pstTmp->uiNodeNum + 1) == uiAddNum), (ALM_NULL));

    /* 得到插入位 */
    stOpt.bOnlyGetLast = ALM_TRUE;
    iRet = ALMStoreTreeFind(pstTree, pstAdd->pData, &stOpt, pstWork);
    ALM_CallRet_M(iRet, (ALM_NULL));

    /* 添加数据 */
    if (pstWork->pstInsertLast != ALM_NULL)
    {
        pstTmp = pstWork->pstInsertLast->apstPorts[pstWork->usInsertPort];

        pstWork->pstInsertLast->apstPorts[pstWork->usInsertPort] = pstAdd;
        pstAdd->pstLast = pstWork->pstInsertLast;
        pstAdd->usLastPort = pstWork->usInsertPort;

        /* 单链或单链子树等值位插入 */
        if (pstTmp != ALM_NULL)
        {
            pstItr = pstAdd;

            /* 得到等值链尾部 */
            while (pstItr->apstPorts[ALM_STORE_TREE_EQUAL_PORT] != ALM_NULL)
            {
                pstItr->uiNodeNum += pstTmp->uiNodeNum;
                pstItr = pstItr->apstPorts[ALM_STORE_TREE_EQUAL_PORT];
            }

            /* 插入等值链 */
            pstItr->apstPorts[ALM_STORE_TREE_EQUAL_PORT] = pstTmp;
            pstItr->uiNodeNum += pstTmp->uiNodeNum;

            pstTmp->pstLast = pstItr;
            pstTmp->usLastPort = ALM_STORE_TREE_EQUAL_PORT;
        }
    }
    else
    {
        pstTree->pstRoot = pstAdd;
        pstAdd->pstLast = ALM_NULL;
        pstAdd->usLastPort = 0;
    }

    /* 更新计数 */
    pstItr = pstAdd->pstLast;

    while (pstItr != ALM_NULL)
    {
        pstItr->uiNodeNum += uiAddNum;
        pstItr = pstItr->pstLast;
    }

    return iRet;
}

/*
* 描述: 添加结点及子结点
* pstTree - 待更新的树
* pstAdd - 添加节点
* pstWork - 数据项节点工作集
* pstNextBuf - 需要迭代的节点
*/
static ALM_INT32 ALMStoreTreeAddNodeAndSub(INOUT ALM_STORE_TREE_STRU* pstTree,
        IN ALM_STORE_TREE_NODE_STRU* pstAdd,
        IN ALM_STORE_TREE_WORK_STRU* pstWork,
        IN ALM_STORE_TREE_NODES_STRU* pstNextBuf)
{
    ALM_INT32 iRet;
    ALM_UINT16 usIdx;
    ALM_STORE_TREE_NODE_STRU* pstItr;

    ALM_Chk_M(pstTree != ALM_NULL && pstAdd != ALM_NULL && pstWork != ALM_NULL, (ALM_NULL));

    pstAdd->pstLast = ALM_NULL;
    pstWork->stNexts.uiNum = 0, pstItr = pstAdd, pstNextBuf->uiNum = 0;

    while (pstItr != ALM_NULL)
    {
        /* 添加下层成员到Next，单独处理等值链 */
        for (usIdx = 0; usIdx < pstTree->stStoreCfg.usPortNum; ++usIdx)
        {
            if (ALM_NULL != pstItr->apstPorts[usIdx]
                && usIdx != ALM_STORE_TREE_EQUAL_PORT)
            {
                ALM_ArrayObjAddRet_M(*pstNextBuf,
                                     ALM_STORE_TREE_NODE_STRU*,
                                     &pstItr->apstPorts[usIdx],
                                     1);

                pstItr->uiNodeNum -= pstItr->apstPorts[usIdx]->uiNodeNum;
                pstItr->apstPorts[usIdx] = ALM_NULL;
            }
        }

        pstItr->pstLast = ALM_NULL;

        /* 添加结点本身及等值链 */
        iRet = ALMStoreTreeAddNode(pstTree, pstItr, pstWork);
        ALM_CallRet_M(iRet, (ALM_NULL));

        /* 迭代下一项 */
        if (pstNextBuf->uiNum > 0)
        {
            --pstNextBuf->uiNum;
            pstItr = pstNextBuf->pstArray[pstNextBuf->uiNum];
        }
        else
        {
            pstItr = ALM_NULL;
        }
    }


    return ALM_OK;
}

/* 删除树中指定的节点，处理平衡性，不释放指定结点，会更新待删除结点的子成员及计数 */
static ALM_INT32 ALMStoreTreeDelNode(INOUT ALM_STORE_TREE_STRU* pstTree,
                                     IN ALM_STORE_TREE_NODE_STRU* pstDel,
                                     IN ALM_STORE_TREE_WORK_STRU* pstWork,
                                     IN ALM_STORE_TREE_NODES_STRU* pstNextBuf)
{
    ALM_INT32 iRet = ALM_OK;
    ALM_STORE_TREE_NODE_STRU* pstLast;
    ALM_STORE_TREE_NODE_STRU* pstRootBk;
    ALM_UINT16 usLastPort;

    ALM_Chk_M(pstTree != ALM_NULL && pstDel != ALM_NULL && pstWork != ALM_NULL
              && pstNextBuf != ALM_NULL, (ALM_NULL));

    /* 移除结点后剩余的子节点重构成子多叉树 */
    pstRootBk = pstTree->pstRoot;

    pstLast = pstDel->pstLast;
    usLastPort = pstDel->usLastPort;

    pstDel->pstLast = ALM_NULL;
    pstTree->pstRoot = pstDel;
    iRet = ALMStoreTreeRmRoot(pstTree, ALM_TRUE, pstWork, pstNextBuf);
    ALM_CallRet_M(iRet, (ALM_NULL));

    /* 挂载新树并移除结点 */
    if (pstLast != ALM_NULL)
    {
        pstLast->apstPorts[usLastPort] = pstTree->pstRoot;

        if (pstTree->pstRoot != ALM_NULL)
        {
            pstTree->pstRoot->pstLast = pstLast;
            pstTree->pstRoot->usLastPort = usLastPort;
        }

        /* 更新计数 */
        while (pstLast != ALM_NULL)
        {
            --pstLast->uiNodeNum;
            pstLast = pstLast->pstLast;
        }

        pstTree->pstRoot = pstRootBk;

        /* 均衡化处理 */
        if (pstTree->pstRoot != ALM_NULL)
        {
            iRet = ALMStoreTreeDoBlanch(pstTree,
                                        pstTree->pstRoot,
                                        pstWork,
                                        pstNextBuf);
            ALM_CallRet_M(iRet, (ALM_NULL));
        }
    }

    return ALM_OK;
}

/*
* 描述:优化指定结点作为根结点的子树的平衡性
* pstTree - 树
* pstNode - 指定节点做为平衡起始节点
* pstWork - 数据项节点工作集
* pstNextBuf - 需要迭代的节点
*/
static ALM_INT32 ALMStoreTreeDoBlanch(INOUT ALM_STORE_TREE_STRU* pstTree,
                                      IN ALM_STORE_TREE_NODE_STRU* pstNode,
                                      IN ALM_STORE_TREE_WORK_STRU* pstWork,
                                      IN ALM_STORE_TREE_NODES_STRU* pstNextBuf)
{
    ALM_INT32 iRet;
    ALM_STORE_TREE_NODE_STRU* pstItr;
    ALM_STORE_TREE_NODE_STRU* pstLast;
    ALM_STORE_TREE_STRU stTreeTmp;
    ALM_UINT16 usLastPort;

    ALM_Chk_M(pstTree != ALM_NULL && pstNode != ALM_NULL, (ALM_NULL));

    ALM_OsMemcpy_M(&stTreeTmp, 1, pstTree, 1, ALM_STORE_TREE_STRU);

    pstItr = pstNode;

    while (pstItr != ALM_NULL)
    {
        /* 对子树做平衡性处理: 超出平衡移除子树根结点，再重新插入 */
        pstLast = pstItr->pstLast, usLastPort = pstItr->usLastPort;
        stTreeTmp.pstRoot = pstItr, stTreeTmp.pstRoot->pstLast = ALM_NULL;

        iRet = ALMStoreTreeRmRoot(&stTreeTmp, ALM_FALSE, pstWork, pstNextBuf);
        ALM_CallRet_M(iRet, (ALM_NULL));

        if (stTreeTmp.pstRoot != pstItr)
        {
            iRet = ALMStoreTreeAddNode(&stTreeTmp, pstItr, pstWork);
            ALM_CallRet_M(iRet, (ALM_NULL));

            pstItr = stTreeTmp.pstRoot;
        }

        ALM_Chk_M(pstItr != ALM_NULL, (ALM_NULL));

        /* 重新挂载子树 */
        if (pstLast != ALM_NULL)
        {
            pstLast->apstPorts[usLastPort] = pstItr;
        }
        else
        {
            pstTree->pstRoot = pstItr;
        }

        pstItr->pstLast = pstLast;
        pstItr->usLastPort = usLastPort;

        pstItr = pstLast;
    }

    return ALM_OK;
}

/* 用于清理ALM_STORE_TREE_WORK_STRU */
static ALM_VOID ALMStClsStoreTreeWork(INOUT ALM_STORE_TREE_WORK_STRU* pstWork)
{
    if (ALM_NULL != pstWork)
    {
        ALM_AdapterBaseMemFree(pstWork->stCmp.ausNexts);
        ALM_ArrayObjCls_M(pstWork->stEquals);
        ALM_ArrayObjCls_M(pstWork->stNexts);

        ALM_OsMemset0_M(pstWork, ALM_STORE_TREE_WORK_STRU, 1);
    }
}

/************************************************************************
* 常用工具。
************************************************************************/

/* 端口号转变为端口码，返回端口码 */
ALM_UINT16 ALM_TreePortIdx2Code(ALM_UINT16 usPort)
{
    ALM_UINT16 usCode = 0;
    ALM_UINT16 usTmp;
    ALM_UINT16 usIdx = 0;

#ifdef LIC_DEBUG
    ALM_UINT16 usPortBk = usPort;
#endif

    while (usPort > 0)
    {
        usTmp = usPort % ALM_STORE_TREE_RSLT_MAX_NUM;
        usTmp <<= (ALM_STORE_TREE_PORT_BITS * usIdx);
        usCode |= usTmp;

        usPort /= ALM_STORE_TREE_RSLT_MAX_NUM;
        ++usIdx;
    }

#ifdef LIC_DEBUG
    {
        usTmp = ALM_TreePortCode2Idx(usCode);
        ALM_Chk_M(usTmp == usPortBk, (ALM_NULL));
    }
#endif

    return usCode;
}


/* 端口码转变为端口号，返回端口号  */
ALM_UINT16 ALM_TreePortCode2Idx(ALM_UINT16 usCode)
{
    ALM_UINT16 usPort = 0;
    ALM_UINT16 usTmp;
    ALM_UINT16 usIdx = 1;

    while (usCode > 0)
    {
        usTmp = (usCode & ALM_STORE_TREE_PORT_MASK) * usIdx;
        usPort += usTmp;

        usCode >>= ALM_STORE_TREE_PORT_BITS;
        usIdx *= ALM_STORE_TREE_RSLT_MAX_NUM;
    }

    return usPort;
}

/* 得到端口的反向端口，返回反向端口  */
ALM_UINT16 ALM_TreePortReverse(ALM_UINT16 usPort)
{
    ALM_UINT16 usRevPort = 0;

    /* 得到反向端口 */
#define ALM_STORE_TREE_REVERSE(uiCode) \
    ((((uiCode ^ (uiCode << 1)) & 0xaaaaaaaa) \
      | ((uiCode ^ (uiCode >> 1)) & 0x55555555)) ^ uiCode)

    if (usPort == ALM_STORE_TREE_EQUAL_PORT)
    {
        usRevPort = usPort;
    }
    else
    {
        usRevPort = ALM_TreePortIdx2Code(usPort);
        usRevPort = ALM_STORE_TREE_REVERSE(usRevPort);
        usRevPort = ALM_TreePortCode2Idx(usRevPort);
    }

#ifdef LIC_DEBUG /* 反向端口的反向端口应等于其本身 */
    {
        ALM_UINT16 usTmp = ALM_TreePortIdx2Code(usRevPort);
        usTmp = ALM_STORE_TREE_REVERSE(usTmp);
        usTmp = ALM_TreePortCode2Idx(usTmp);
        ALM_Chk_M(usTmp == usPort, (ALM_NULL));
    }
#endif

    return usRevPort;
}

/* 校验树的有效性 */

#ifdef ALM_UT
static ALM_INT32 ALMTreeCheck(CONST ALM_STORE_TREE_STRU* pstTree)
{
    ALM_INT32 iRet;
    ALM_STORE_TREE_WORK_STRU stNodes = {{0}, {0}, {0}, 0, 0};
    ALM_STORE_TREE_FIND_OPT_STRU stOpt;
    ALM_UINT32 uiIdx;
    ALM_UINT32 uiIdxSub;
    ALM_UINT16 usIdx;
    ALM_UINT32 uiTmp;
    ALM_STORE_TREE_WORK_STRU stSubNodes = {{0}, {0}, {0}, 0, 0};
    ALM_STORE_TREE_NODE_STRU* pstBk;
    ALM_STORE_TREE_NODE_STRU* pstItr;

    ALM_ChkRet_M(pstTree != ALM_NULL, ALM_ERR_NULL_PTR, (ALM_NULL));

    /* 得到所有结点 */
    stOpt.bFindByMark = ALM_TRUE;
    stOpt.bOnlyFindOnce = ALM_FALSE, stOpt.bOnlyGetLast = ALM_FALSE, stOpt.bOnlyGetLeaf = ALM_FALSE;
    iRet = ALMStoreTreeFind((ALM_STORE_TREE_STRU*)pstTree, ALM_NULL, &stOpt,
                            &stNodes);
    ALM_CallRet_M(iRet, (ALM_NULL));

    /* 校验 */
    ALM_Chk_M(ALM_NULL == pstTree->pstRoot
              || pstTree->pstRoot->uiNodeNum == stNodes.stEquals.uiNum, (ALM_NULL));
    ALM_Chk_M(ALM_NULL == pstTree->pstRoot || ALM_NULL == pstTree->pstRoot->pstLast,
              (ALM_NULL));

    for (uiIdx = 0; uiIdx < stNodes.stEquals.uiNum; ++uiIdx)
    {
        pstItr = stNodes.stEquals.pstArray[uiIdx];

        ALM_Chk_M(pstItr != ALM_NULL, ("The find rslt include null."));

        if (ALM_NULL == pstItr->pstLast)
        {
            ALM_Chk_M(pstItr == pstTree->pstRoot,
                      ("No last, so must be root."));
        }
        else
        {
            ALM_Chk_M(pstItr->pstLast->apstPorts[pstItr->usLastPort] ==
                      pstItr, ("Node not in the port of last."));
        }

        /* 校验当前结点 */
        uiTmp = 1;

        for (usIdx = 0; usIdx < pstTree->stStoreCfg.usPortNum; ++usIdx)
        {
            if (ALM_NULL != pstItr->apstPorts[usIdx])
            {
                uiTmp += pstItr->apstPorts[usIdx]->uiNodeNum;

                /* 校验结点及下层结点方向有效性 */
                pstBk = ((ALM_STORE_TREE_STRU*)pstTree)->pstRoot;
                ((ALM_STORE_TREE_STRU*)pstTree)->pstRoot =
                    pstItr->apstPorts[usIdx];
                ALMStClsStoreTreeWork(&stSubNodes);
                iRet = ALMStoreTreeFind((ALM_STORE_TREE_STRU*)pstTree, ALM_NULL,
                                        &stOpt, &stSubNodes);

                ((ALM_STORE_TREE_STRU*)pstTree)->pstRoot = pstBk;
                ALM_CallLab_M(LAB_END, (ALM_NULL));

                for (uiIdxSub = 0; uiIdxSub < stSubNodes.stEquals.uiNum; ++uiIdxSub)
                {
                    stNodes.stCmp.uiNum = 0;
                    iRet = pstTree->stStoreCfg.stAdapterFuncs.fnGetPort(
                               stSubNodes.stEquals.pstArray[uiIdxSub]->pData,
                               ALM_FALSE,
                               pstItr->pData,
                               &stNodes.stCmp,
                               pstTree->stStoreCfg.pEx);
                    ALM_CallLab_M(LAB_END, (ALM_NULL));

                    ALM_Chk_M(1 == stNodes.stCmp.uiNum,
                              ("Data compare, must exist one port."));

                    ALM_Chk_M(stNodes.stCmp.ausNexts[0] == usIdx,
                              ("Node is local with invalid port."));

                    ALM_Chk_M(stSubNodes.stEquals.pstArray[uiIdxSub] != pstItr,
                              ("Find same nodes."));
                }
            }
        }

        /* 结点计数应为下层结点总数目加一 */
        if (pstTree->stStoreCfg.usPortNum > 1)
        {
            ALM_Chk_M(uiTmp == pstItr->uiNodeNum,
                      ("Node count is not equal with all subs."));
        }
    }

LAB_END:
    ALMStClsStoreTreeWork(&stNodes);
    ALMStClsStoreTreeWork(&stSubNodes);
    (ALM_VOID)uiTmp;
    return iRet;
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /*ifdef __cplusplus */
#endif /*if __cplusplus*/
