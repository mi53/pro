/*******************************************************************************
Description    : 使用多叉树实现存储。
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_STORE_TREE_H__
#define __ALM_STORE_TREE_H__

#include "alm_tool_base.h"

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

/************************************************************************/
/*                      存储器定义相关数据结构                               */
/************************************************************************/

/* 比较指定数据是否符合要求，并得到需要继续迭代的端口 */
typedef struct
{
    ALM_UINT16* ausNexts;
    ALM_UINT32 uiCap;
    ALM_UINT32 uiNum;
    ALM_BOOL bThisIsEqual;
} ALM_STORE_TREE_CMP_RSLT_STRU;

/* 初始化存储数据元 */
typedef ALM_INT32 (*ALM_STORE_TREE_INIT_DATA_FUNC)(OUT ALM_VOID* pData,
        CONST ALM_VOID* pEx);

/* 清理存储数据元 */
typedef ALM_VOID (*ALM_STORE_TREE_CLS_DATA_FUNC)(INOUT ALM_VOID* pData,
        CONST ALM_VOID* pEx);

/* 拷贝存储数据元 */
typedef ALM_INT32 (*ALM_STORE_TREE_CPY_DATA_FUNC)(OUT ALM_VOID* pDestData,
        CONST ALM_VOID* pSrcData,
        CONST ALM_VOID* pEx);

/*
* 描述: 得到数据比较结果及下层遍历端口。得到的端口号必须小于端口总数目。
* pCmpL - 比较左数，一般为mark或数据。
* bFindByMark - 比较左数为mark还是数据。
* pDataR - 比较右数。
* pstRslt - 得到的比较结果。
* pEx - 初始化存储对象时指定的扩展成员。
*/
typedef ALM_INT32 (*ALM_STORE_TREE_GET_PORT_FUNC)(CONST ALM_VOID* pCmpL,
        ALM_BOOL bFindByMark,
        CONST ALM_VOID* pDataR,
        INOUT ALM_STORE_TREE_CMP_RSLT_STRU* pstRslt,
        CONST ALM_VOID* pEx);

/* 存储器自定义之基础部分 */
typedef struct
{
    /* 初始化数据对象，若没有指定使用仅做内存清零处理 */
    ALM_STORE_TREE_INIT_DATA_FUNC fnDataInit;

    /* 清理数据对象，若没有指定使用仅做内存清零处理 */
    ALM_STORE_TREE_CLS_DATA_FUNC fnDataCls;

    /* 拷贝数据对象，若没有指定使用memcpy */
    ALM_STORE_TREE_CPY_DATA_FUNC fnDataCpy;

    /* 得到数据比较结果及下层遍历端口，不能为NULL */
    ALM_STORE_TREE_GET_PORT_FUNC fnGetPort;

} ALM_STORE_TREE_ADAPTER_STRU;

/**
* 描述：存储操作信息结构体。
* 成员:
* uiUnitSize - 数据单元大小。
* usPortNum - 指定端口数目，必须大于0。
* bIncSame - 是否允许添加重复数据。
* stAdapterFuncs - 数据单元初始化、清理等函数。
* pEx - 自定义附加信息，数据元操作排序等可能需要使用该扩展内容。
*/
typedef struct
{
    ALM_INT32 uiUnitSize;
    ALM_UINT16 usPortNum;
    ALM_BOOL bIncSame;
    ALM_STORE_TREE_ADAPTER_STRU stAdapterFuncs;
    CONST ALM_VOID* pEx;
} ALM_STORE_TREE_CFG_STRU;

/************************************************************************/
/*                      存储器实现及操作                                   */
/************************************************************************/

/* 用于定义多叉树的工具宏 */
#define ALM_STORE_TREE_STRU_T(type) ALM_STORE_TREE_STRU

/* 定义数结点 */
typedef struct TAG_ALM_STORE_TREE_NODE_STRU
{
    ALM_VOID* pData;

    /*apstPorts为数组，存储子节点地址*/
    struct TAG_ALM_STORE_TREE_NODE_STRU** apstPorts;

    /* 下层子及结点本身总数目 */
    ALM_UINT32 uiNodeNum;

    struct TAG_ALM_STORE_TREE_NODE_STRU* pstLast;
    ALM_UINT16 usLastPort;
} ALM_STORE_TREE_NODE_STRU;

/* 定义多叉树存储 */
typedef struct
{
    ALM_STORE_TREE_CFG_STRU stStoreCfg;
    ALM_STORE_TREE_NODE_STRU* pstRoot;
} ALM_STORE_TREE_STRU;

/* 清理多叉树对象 */
ALM_VOID ALM_StClsStoreTree(INOUT ALM_STORE_TREE_STRU* pstTree);

/* 用于保存查询结果 */
typedef struct
{
    CONST ALM_STORE_TREE_STRU* pstTree;
    ALM_VOID* pDatas;
    ALM_UINT32 uiNum;
    ALM_UINT32 uiCap;
    ALM_UINT32 uiDataSize;
} ALM_STORE_TREE_RSLT_STRU;

/* 清理ALM_STORE_TREE_RSLT_STRU对象 */
ALM_VOID ALM_StClsStoreTreeRslt(INOUT ALM_STORE_TREE_RSLT_STRU*);

/* 初始化多叉树 */
ALM_INT32 ALM_StoreTreeInit(OUT ALM_STORE_TREE_STRU* pstTree,
                            CONST ALM_STORE_TREE_CFG_STRU* pstStoreInfo);

/* 得到数据总量 */
ALM_INT32 ALM_StoreTreeGetTotal(CONST ALM_STORE_TREE_STRU* pstTree,
                                ALM_UINT32* puiCnt);

/* 删除指定数据 */
ALM_INT32 ALM_StoreTreeDel(INOUT ALM_STORE_TREE_STRU* pstTree,
                           CONST ALM_VOID* pMark);

/* 清理全部数据，较ALM_StoreTreeDel效率更高，无论是否出错均强制清空 */
ALM_VOID ALM_StoreTreeDelAll(INOUT ALM_STORE_TREE_STRU* pstTree);

/* 添加数据 */
ALM_INT32 ALM_StoreTreeAdd(INOUT ALM_STORE_TREE_STRU* pstTree,
                           CONST ALM_VOID* pData);

/* 查找数据，得到数据的拷贝 */
ALM_INT32 ALM_StoreTreeGet(CONST ALM_STORE_TREE_STRU* pstTree,
                           CONST ALM_VOID* pMark,
                           OUT ALM_STORE_TREE_RSLT_STRU* pstRslt);

/************************************************************************
* 该部分函数能用于提高性能，由于直接操作NODE，须谨慎使用。
************************************************************************/

/* 保存待处理结点 */
typedef struct
{
    ALM_STORE_TREE_NODE_STRU** pstArray;
    ALM_UINT32 uiCap;
    ALM_UINT32 uiNum;
} ALM_STORE_TREE_NODES_STRU;

/* 查找数据并得到数据所在结点，谨慎修改所得数据，否则会导致树结构紊乱（仅在不影响索引时修改） */
ALM_INT32 ALM_StoreTreeGetNodes(CONST ALM_STORE_TREE_STRU* pstTree,
                                CONST ALM_VOID* pMark,
                                OUT ALM_STORE_TREE_NODES_STRU* pstRslt);

/* 更新指定数据 */
ALM_INT32 ALM_StoreTreeUpdateNode(INOUT ALM_STORE_TREE_STRU* pstTree,
                                  IN ALM_STORE_TREE_NODE_STRU* pstOld,
                                  CONST ALM_VOID* pNewData);

/************************************************************************
* 常用工具。
************************************************************************/

/* 比较结果，为了便于得到端口的反向端口此处数据不能随意 */
typedef enum
{
    ALM_STORE_TREE_CMP_EQUAL = 0,
    ALM_STORE_TREE_CMP_LESS = 1,
    ALM_STORE_TREE_CMP_MORE = 2,
    ALM_STORE_TREE_CMP_IGR = 3
} ALM_STORE_TREE_CMP_RSLT_ENUM;

/* 单个索引项值比较的最大结果数目 */
#define ALM_STORE_TREE_RSLT_MAX_NUM 3

/* 每一个列在端口索引中占位数 */
#define ALM_STORE_TREE_PORT_BITS 2

/* 端口占位掩码 */
#define ALM_STORE_TREE_PORT_MASK 3

/* 等值端口、或称反向等向端口 */
#define ALM_STORE_TREE_EQUAL_PORT 0

/* 端口号转变为端口码，返回端口码 */
ALM_UINT16 ALM_TreePortIdx2Code(ALM_UINT16 usIdx);

/* 端口码转变为端口号，返回端口号  */
ALM_UINT16 ALM_TreePortCode2Idx(ALM_UINT16 usCode);

/* 得到端口的反向端口，返回反向端口  */
ALM_UINT16 ALM_TreePortReverse(ALM_UINT16 usPort);

#if (defined(ALM_PACK_4) || defined(ALM_PACK_8))
#pragma pack ()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /*ifdef __cplusplus */
#endif /*if __cplusplus*/

#endif /* __ALM_STORE_TREE_H__ */
