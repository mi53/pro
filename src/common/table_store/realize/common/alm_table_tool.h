/*******************************************************************************
Description    : 扩展并完善工具对象。
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLE_TOOL_H__
#define __ALM_TBLE_TOOL_H__

#include "alm_table_tool_itf.h"
#include "alm_table_row.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#if defined(ALM_PACK_4)
#pragma pack (4)
#elif defined(ALM_PACK_8)
#pragma pack (8)
#endif

/************************************************************************/
/*         ALM_TBL_DATA_STRU: 注意正确处理未字节对齐的整形数据。               */
/************************************************************************/
#define ALM_TBL_DATA_TYPE_UINT8_LEN     sizeof(ALM_UCHAR)
#define ALM_TBL_DATA_TYPE_UINT32_LEN    sizeof(ALM_UINT32)
#define ALM_TBL_DATA_TYPE_UINT64_LEN    sizeof(ALM_UINT64)
#define ALM_TBL_DATA_TYPE_CHAR18_LEN    18
#define ALM_TBL_DATA_TYPE_CHAR32_LEN    32
#define ALM_TBL_DATA_TYPE_CHAR64_LEN    64
#define ALM_TBL_DATA_TYPE_CHAR128_LEN   128
#define ALM_TBL_DATA_TYPE_CHAR256_LEN   256
#define ALM_TBL_DATA_TYPE_CHAR512_LEN   512
#define ALM_TBL_DATA_TYPE_MAX_LEN       1024


/* 确定数据类型的有效性 */
ALM_BOOL ALM_TblDataTypeCheck(ALM_ENUM(ALM_TBL_DATA_TYPE_ENUM) eType);

/* 清理data对象 */
ALM_VOID ALM_StClsTblData(INOUT ALM_TBL_DATA_STRU* pstData);

/* 初始化数据项 */
ALM_INT32 ALM_TblDataInit(OUT ALM_TBL_DATA_STRU* pstData,
                          ALM_ENUM(ALM_TBL_DATA_TYPE_ENUM) eType);

/* 依据字符串及指定数据类型，得到对应的数据对象 */
ALM_INT32 ALM_TblDataInitByStrVal(OUT ALM_TBL_DATA_STRU* pstData,
                                  ALM_ENUM(ALM_TBL_DATA_TYPE_ENUM) eType,
                                  CONST ALM_CHAR* pcVal);

/* 写入数据，不会重新分配空间，非字符串类型数据大小必须一致，字符串类型容量必须足够 */
ALM_INT32 ALM_TblDataWriteByPtr(INOUT ALM_TBL_DATA_STRU* pstData,
                                ALM_ENUM(ALM_TBL_DATA_TYPE_ENUM) eType,
                                CONST ALM_VOID* pData,
                                ALM_UINT32 uiDataSize);

/* 改变数据字节序*/
ALM_INT32 ALM_TblDataChangeEndin(INOUT ALM_TBL_DATA_STRU* pstData);

/* 获取数据内容，数值数据类型，buf长度必须与数据类型大小保持一致，string类型只需能存储便可 */
ALM_INT32 ALM_TblDataGet(CONST ALM_TBL_DATA_STRU* pstData,
                         ALM_ENUM(ALM_TBL_DATA_TYPE_ENUM) eDataType,
                         OUT ALM_VOID* pBuf,
                         ALM_UINT32 uiBufLen);


/************************************************************************/
/*               ALM_TBL_SROW_STRU                                      */
/************************************************************************/

/* 更新一个数据行，只更新指定列 */
ALM_INT32 ALM_TblSRowsToUpdateRow(CONST ALM_TBL_SROW_STRU* pstSRows,
                                  CONST ALM_TBL_COLS_STRU* pstCols,
                                  INOUT ALM_TBL_ROW_STRU* pstRow);

/* 得到某一列的索引，若不存在指定列得到的索引为负值 */
ALM_INT32 ALM_TblSRowsGetIdx(CONST ALM_TBL_SROW_STRU* pstSRows,
                             CONST ALM_CHAR* pcColName,
                             OUT ALM_INT32* piIdx);


/************************************************************************/
/*              ALM_TBL_CDT_STRU                                        */
/************************************************************************/

/* 增加列条件 */
ALM_INT32 ALM_TblCdtAddColCdts(INOUT ALM_TBL_CDT_STRU* pstCdt,
                               CONST ALM_TBL_SROW_STRU* pstColCdts);

/************************************************************************/
/*                         ALM_TBL_RSLT_STRU                            */
/************************************************************************/

/**
* 描述: 初始化查询包，不再使用应使用对应的函数清理。
* 参数:
* pstRslt - 待初始化对象。
* pstCols - 指定要查询的列。
*/
ALM_INT32 ALM_TblRsltInit(OUT ALM_TBL_RSLT_STRU* pstRslt,
                          CONST ALM_TBL_COLS_STRU* pstCols);

#if (defined(ALM_PACK_4) || defined(ALM_PACK_8))
#pragma pack ()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_TOOL_H__ */
