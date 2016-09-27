/****************************************************************************************
********
********   文件名:   OSAL_type.h
********   功能描述: 基本数据类型定义
********   版本:     V1.0
********   作者:     flybird
********   日期:     2014-12-16 9:35
********  
*****************************************************************************************/

#ifndef  OSALTYPE_H
#define  OSALTYPE_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#pragma once 

	typedef  char               OSAL_CHAR;
	typedef  unsigned char      OSAL_UCHAR;
	typedef  char               OSAL_INT8;
	typedef  short              OSAL_INT16;
	typedef  int                OSAL_INT32;
	typedef  long long          OSAL_INT64;
	typedef  unsigned char      OSAL_UINT8;
	typedef  unsigned short     OSAL_UINT16;
	typedef  unsigned int       OSAL_UINT32;
	typedef  unsigned long long OSAL_UINT64;
	typedef  unsigned int       OSAL_BOOL;
	typedef  double             OSAL_DBL64;
	typedef  void               OSAL_VOID;

	typedef  OSAL_CHAR *        OSAL_PCHAR;
	typedef  OSAL_UCHAR *       OSAL_PUCHAR;
	typedef  OSAL_INT8 *        OSAL_PINT8;
	typedef  OSAL_INT16 *       OSAL_PINT16;
	typedef  OSAL_INT32 *       OSAL_PINT32;
	typedef  OSAL_INT64 *       OSAL_PINT64;
	typedef  OSAL_UINT16 *      OSAL_PUINT16;
	typedef  OSAL_UINT32 *      OSAL_PUINT32;
	typedef  OSAL_UINT64 *      OSAL_PUINT64;
	typedef  OSAL_BOOL *        OSAL_PBOOL;
	typedef  OSAL_DBL64 *       OSAL_PDBL64;

	typedef OSAL_VOID *         OSAL_HANDLE;

	/* 布尔值声明*/
#define  OSAL_TRUE	  1
#define  OSAL_FALSE	  0

#define  OSAL_OK      0
#define  OSAL_FAIL    -1

	/* 指针空值 */
#define  OSAL_NULL    0

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

#ifndef REST
#define REST 
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OSALTYPE_H */

