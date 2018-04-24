/*******************************************************************************
Description    : table store实现。
FileName       : alm_table_store.h
History        :
<version> <date>     <Author> <Description>
*******************************************************************************/
#ifndef __ALM_TBLE_STORE_H__
#define __ALM_TBLE_STORE_H__

#include "alm_table_store_itf.h"
#include "alm_table_obj.h"

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

/* 得到已创建的表格 */
ALM_TBL_OBJ_STRU** ALM_TblStoreGetAll(OUT ALM_UINT32* puiNum);

#if (defined(ALM_PACK_4) || defined(ALM_PACK_8))
#pragma pack ()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* if __cplusplus */
#endif /* ifdef __cplusplus */

#endif /* __ALM_TBLE_STORE_H__ */
