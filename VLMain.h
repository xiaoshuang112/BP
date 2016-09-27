/****************************************************************************************
********
********   文件名:   VLMain.h 
********   功能描述: 车标识别内部结构体定义
********   版本:     V1.0
********   作者:     flybird
********   日期:     2016-9-7 19:38
********  
*****************************************************************************************/

#ifndef   VLMAIN_H
#define   VLMAIN_H

#include "OSAL_type.h"
#include "SDKCommon.h"

	/*定义识别结果结构体*/
typedef struct TagVLResult
{
	OSAL_INT32	 iLogoType;					/*  车标类型  */
	OSAL_INT32   iXStart;                   /*  车标在图片中X方向的起始位置  */
	OSAL_INT32   iXEnd;                     /*  车标在图片中X方向的终止位置  */
	OSAL_INT32   iYStart;                   /*  车标在图片中Y方向的起始位置  */
	OSAL_INT32   iYEnd;                     /*  车标在图片中Y方向的终止位置  */
}TVLResult,*PTVLResult;

SDKErrCode VL_TaskCreate(OSAL_HANDLE *pHandle, OSAL_INT32 iMaxW, OSAL_INT32 iMaxH);

SDKErrCode VL_TaskFree(OSAL_HANDLE *pHandle);

SDKErrCode VL_TaskProcess(OSAL_HANDLE handle, OSAL_UCHAR* pImage, OSAL_INT32 iW, OSAL_INT32 iH,ImageDataType eType,PTVLResult* ppVFResult);

//提取HOG特征
OSAL_INT32 VL_GetImageHogFeature(OSAL_INT32 *pFeature,OSAL_UCHAR *pImage,OSAL_UINT32 iW,OSAL_UINT32 iH,OSAL_INT32 *pnMeasure,OSAL_INT32 *pnTheta,OSAL_INT32 *pIntegHist);

#endif /* VLMAIN_H */
