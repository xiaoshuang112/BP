



#ifndef   VEHICLECOLOR_H
#define   VEHICLECOLOR_H


#define  VEHICLECOLORAPI_EXPORTS

#ifdef VEHICLECOLORAPI_EXPORTS
#define VEHICLECOLORAPI_API __declspec(dllexport)
#else
#define VEHICLECOLORAPI_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "OSAL_type.h"
#include "SDKCommon.h"

typedef  struct TagVRECT{     /*  矩阵结构体       */
	OSAL_INT16   left;        //  矩阵的左边缘
	OSAL_INT16   right;       //  矩阵的右边缘
	OSAL_INT16   top;         //  矩阵的上边缘
	OSAL_INT16   bottom;      //  矩阵的下边缘	
}VRECT,  *PVRECT;

enum  Vehicle_COLOR
{ 
	V_Black=0,
	V_White,
	V_Gray,
	V_Red,
	V_Orange,
	V_Yellow,
	V_Green,
	V_Indigo,
	V_Blue,
	V_Purple,
	V_Sliver,
};

VEHICLECOLORAPI_API SDKErrCode  vehicle_color_detect(OSAL_HANDLE handle,OSAL_UCHAR* pImage,OSAL_INT32 iW,OSAL_INT32 iH,ImageDataType eType,int* ppELAResult );


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ELAEXPORT_H */