/****************************************************************************************
********
********   文件名:   TypeSdk.h
********   功能描述: 车型对外接口函数声明
********   版本:     V1.0
********   作者:     xiaoshuang
********   日期:     2016-9-7 20:20
********  
*****************************************************************************************/
#ifndef   VEHICLETYPESDK_H
#define   VEHICLETYPESDK_H


#define  TYPEAPI_EXPORTS

#ifdef TYPEAPI_EXPORTS
#define TYPEAPI_API __declspec(dllexport)
#else
#define TYPEAPI_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "SDKCommon.h"
#include "OSAL_type.h"

	typedef enum
	{
		Type_Blue                      =  0,  /*蓝色*/
		Type_Yellow                    =  1,  /*黄色*/
		Type_White                     =  2,  /*白色*/

		Type_PARAM_OTHER                = -1   /*未知格式*/
	} PlateType;

	typedef enum
	{
		Type_Truck                  = 0,//da车；
		Type_Car                   = 1,//xiao车；
		
		Type_MiniBus                 = 2,//mianbao

		Type_Bus                       = 3,//客车；

		Type_other                     = 4

	} TYPEResult;
	TYPEAPI_API SDKErrCode VehicleType_TaskCreate(OSAL_HANDLE *pHandle);              // 创建一个算法处理过程
	TYPEAPI_API SDKErrCode VehicleType_SetParam(OSAL_HANDLE Handle);    // 设置参数
 	TYPEAPI_API SDKErrCode VehicleType_TaskProcess(OSAL_HANDLE handle,OSAL_UCHAR* pImage,OSAL_INT32 iW,OSAL_INT32 iH,ImageDataType eType,TYPEResult* ppELAResult);
 	SDKErrCode VehicleType_TaskFree(OSAL_HANDLE *pHandle);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TYPESDK_H */


