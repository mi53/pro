/*******************************************************************************
Description    : 扩展并完善工具对象。
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLE_TOOL_INC__
#define __ALM_TBLE_TOOL_INC__

#include "alm_table_tool.h"
#include <string.h>

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
/*         ALM_TBL_DATA_STRU: 注意正确处理未字节对齐的整形数据。              */
/************************************************************************/

/* 将字符串转换为data对应的数据类型，并写入数据对象 */
static ALM_INT32 ALMTblDataWrite(INOUT ALM_TBL_DATA_STRU* pstData,
                                 CONST ALM_CHAR* pcVal);

/* 判断是否为字符串类型 */
static ALM_BOOL ALMTblDataCheckStr(ALM_ENUM(ALM_TBL_DATA_TYPE_ENUM) eType);

/************************************************************************/
/*               ALM_TBL_SROW_STRU                                      */
/************************************************************************/

/* 扩展COL MAPS存储空间，保证总容量与当前数目的差值大于等于指定的扩展数目 */
static ALM_INT32 ALMTblSRowsExpand(INOUT ALM_TBL_SROW_STRU* pstSRows,
                                   ALM_UINT32 uiExpand);

/* 初始化单个列条件，不再使用应使用函数ALMTblQueryColCdtCls清理 */
static ALM_INT32 ALMStInitTblSRowsCol(OUT ALM_TBL_COL_MAP_STRU* pstColMap,
                                      CONST ALM_CHAR* pcColName,
                                      CONST ALM_TBL_DATA_STRU* pstVal);

/* 清理已初始的列，清理后应重新初始化才能使用 */
static ALM_VOID ALMStClsTblSRowsCol(INOUT ALM_TBL_COL_MAP_STRU* pstColMap);

/************************************************************************/
/*              ALM_TBL_CDT_STRU                                        */
/************************************************************************/


/************************************************************************/
/*                         ALM_TBL_RSLT_STRU                            */
/************************************************************************/


#if (defined(ALM_PACK_4) || defined(ALM_PACK_8))
#pragma pack ()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_TOOL_INC__ */
