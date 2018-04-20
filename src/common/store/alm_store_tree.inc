/*******************************************************************************
Description    : 定义实现多叉树用到的辅助函数及结构。
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_STORE_TREE_INC__
#define __ALM_STORE_TREE_INC__

#include "alm_store_tree.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /*ifdef __cplusplus */
#endif /*if __cplusplus*/

#if defined(ALM_PACK_4)
#pragma pack (4)
#elif defined(ALM_PACK_8)
#pragma pack (8)
#endif

/* 判断是否超出平衡（2/3衡量) */
#define ALM_STORE_TREE_CHECK_BLANCH(uiLastNum, uiThisNum) \
    ((uiLastNum - uiThisNum) >= uiThisNum / 2)

/* 查找选项 */
typedef struct
{
    /* 用于查找的身份对象是一个标记还是完整的数据 */
    ALM_BOOL bFindByMark;

    /* 是否仅需用于获取插入的上层结点 */
    ALM_BOOL bOnlyGetLast;

    /* 找到一个符合要求的数据后是否立即返回 */
    ALM_BOOL bOnlyFindOnce;

    /* 是否只获取叶子节点 */
    ALM_BOOL bOnlyGetLeaf;

} ALM_STORE_TREE_FIND_OPT_STRU;

/* 保存待处理结点 */
typedef struct
{
    ALM_STORE_TREE_CMP_RSLT_STRU stCmp;
    ALM_STORE_TREE_NODES_STRU stEquals;
    ALM_STORE_TREE_NODES_STRU stNexts;
    ALM_STORE_TREE_NODE_STRU* pstInsertLast;
    ALM_UINT16 usInsertPort;
} ALM_STORE_TREE_WORK_STRU;

/* 用于清理ALM_STORE_TREE_WORK_STRU */
static ALM_VOID ALMStClsStoreTreeWork(INOUT ALM_STORE_TREE_WORK_STRU* pstWork);

/* 创建节点 */
static ALM_INT32 ALMStoreTreeNodeNew(OUT ALM_STORE_TREE_NODE_STRU** ppstNewNode,
                                     CONST ALM_STORE_TREE_STRU* pstTree,
                                     CONST ALM_VOID* pData);

/* 释放节点，仅释放结点本身 */
static ALM_VOID ALMStoreTreeNodeFree(INOUT ALM_STORE_TREE_NODE_STRU* pstDelNode,
                                     CONST ALM_STORE_TREE_STRU* pstTree);

/* 校验store info的有效性 */
static ALM_BOOL ALMStoreTreeCfgCheck(CONST ALM_STORE_TREE_CFG_STRU* pstStoreInfo);

/* 查询数据项结点及路径 */
static ALM_INT32 ALMStoreTreeFind(INOUT ALM_STORE_TREE_STRU* pstTree,
                                  CONST ALM_VOID* pMark,
                                  CONST ALM_STORE_TREE_FIND_OPT_STRU* pstOpt,
                                  INOUT ALM_STORE_TREE_WORK_STRU* pstWork);

/* 移除树的顶层元素，并将最大子树作为根结点；同时更新原有根结点计数及子树，但不会释放结点 */
/* 会更新原有根结点的子成员及计数 */
/* bForce - ALM_TRUE:在顶层元素满足均衡性时依旧移除 */
static ALM_INT32 ALMStoreTreeRmRoot(INOUT ALM_STORE_TREE_STRU* pstTree,
                                    ALM_BOOL bForce,
                                    IN ALM_STORE_TREE_WORK_STRU* pstWork,
                                    IN ALM_STORE_TREE_NODES_STRU* pstNextBuf);

/* 添加结点，不删除新增结点的后续成员（在使用该函数时，只有等值链才应该保留后续成员） */
static ALM_INT32 ALMStoreTreeAddNode(INOUT ALM_STORE_TREE_STRU* pstTree,
                                     IN ALM_STORE_TREE_NODE_STRU* pstAdd,
                                     IN ALM_STORE_TREE_WORK_STRU* pstWork);

/* 添加结点及子结点 */
static ALM_INT32 ALMStoreTreeAddNodeAndSub(INOUT ALM_STORE_TREE_STRU* pstTree,
        IN ALM_STORE_TREE_NODE_STRU* pstAdd,
        IN ALM_STORE_TREE_WORK_STRU* pstWork,
        IN ALM_STORE_TREE_NODES_STRU* pstNextBuf);

/* 删除树中指定的节点，处理平衡性，不释放指定结点，会更新待删除结点的子成员及计数 */
static ALM_INT32 ALMStoreTreeDelNode(INOUT ALM_STORE_TREE_STRU* pstTree,
                                     IN ALM_STORE_TREE_NODE_STRU* pstDel,
                                     IN ALM_STORE_TREE_WORK_STRU* pstWork,
                                     IN ALM_STORE_TREE_NODES_STRU* pstNextBuf);

/* 优化指定结点作为根结点的子树的平衡性 */
static ALM_INT32 ALMStoreTreeDoBlanch(INOUT ALM_STORE_TREE_STRU* pstTree,
                                      IN ALM_STORE_TREE_NODE_STRU* pstNode,
                                      IN ALM_STORE_TREE_WORK_STRU* pstWork,
                                      IN ALM_STORE_TREE_NODES_STRU* pstNextBuf);

/* 校验树的有效性 */
#ifdef ALM_UT
static ALM_INT32 ALMTreeCheck(CONST ALM_STORE_TREE_STRU* pstTree);
#endif

/* 用于校验树有效性，在UT测试排查问题时使用，使用该宏会极大影响性能 */
#ifdef ALM_UT
#define ALMChkTree_M(pstTree)  \
    ALM_Chk_M((ALMTreeCheck(pstTree) == ALM_OK) \
              && (ALM_NULL == pstTree->pstRoot || pstTree->pstRoot->pstLast == ALM_NULL),\
              (ALM_NULL))
#else
#define ALMChkTree_M(pstTree)
#endif

#if (defined(ALM_PACK_4) || defined(ALM_PACK_8))
#pragma pack ()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /*ifdef __cplusplus */
#endif /*if __cplusplus*/

#endif /* __ALM_STORE_TREE_INC__ */

