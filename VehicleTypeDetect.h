/****************************************************************************************
********
********   �ļ���:   TypeSdk.h
********   ��������: ���Ͷ���ӿں�������
********   �汾:     V1.0
********   ����:     xiaoshuang
********   ����:     2016-9-7 20:20
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
		Type_Blue                      =  0,  /*��ɫ*/
		Type_Yellow                    =  1,  /*��ɫ*/
		Type_White                     =  2,  /*��ɫ*/

		Type_PARAM_OTHER                = -1   /*δ֪��ʽ*/
	} PlateType;

	typedef enum
	{
		Type_Truck                  = 0,//da����
		Type_Car                   = 1,//xiao����
		
		Type_MiniBus                 = 2,//mianbao

		Type_Bus                       = 3,//�ͳ���

		Type_other                     = 4

	} TYPEResult;
	TYPEAPI_API SDKErrCode VehicleType_TaskCreate(OSAL_HANDLE *pHandle);              // ����һ���㷨�������
	TYPEAPI_API SDKErrCode VehicleType_SetParam(OSAL_HANDLE Handle);    // ���ò���
 	TYPEAPI_API SDKErrCode VehicleType_TaskProcess(OSAL_HANDLE handle,OSAL_UCHAR* pImage,OSAL_INT32 iW,OSAL_INT32 iH,ImageDataType eType,TYPEResult* ppELAResult);
 	SDKErrCode VehicleType_TaskFree(OSAL_HANDLE *pHandle);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TYPESDK_H */


