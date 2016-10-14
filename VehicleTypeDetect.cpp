// TypeSdk.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include "string.h"
#include "math.h"

#include "VehicleTypeDetect.h"
#include "Bp_sdk.h"
#include"VLMain.h"
#include"SDKCommon.h"


OSAL_INT32 data_image[756];
int nMeasure[2048],nTheta[2048],nHist[2048];

              //add other bp here;
int ret;

//Bp* pBP = OSAL_NULL;

SDKErrCode VehicleType_TaskCreate(OSAL_HANDLE *pHandle)
{

	Bp* pBP = NULL;

 	pBP = (Bp*)malloc(sizeof(Bp));


	if(pBP == OSAL_NULL)
	{
		return SDK_ERR_ALLOC;
	}

 	Bpinit(pBP,756,160,3, 0) ;


	memset(nHist,     0,sizeof(int)*2048);
	memset(nTheta,    0,sizeof(int)*2048);
	memset(nMeasure,  0,sizeof(int)*2048);
	memset(data_image,0,sizeof(int)*756);

	*pHandle = (OSAL_HANDLE)pBP;

	return SDK_ERR_NONE;
}

SDKErrCode VehicleType_SetParam(OSAL_HANDLE Handle)/*,PlateType eType*/
{
//	type =  eType;
	return SDK_ERR_NONE;
}

static SDKErrCode  resize_64(OSAL_UCHAR* src,OSAL_UCHAR* dst,OSAL_INT32 iW,OSAL_INT32 iH,OSAL_INT32 step_x, OSAL_INT32 step_y)
{
	int i=0,j=0;

	for(i=0;i<32;i++)
	{
		for(j=0;j<64;j++)
		{
			dst[i*64+j]=src[i*step_y*iW+j*step_x];
		}
	}


   return SDK_ERR_NONE;

}

static OSAL_UCHAR s_temp_image[64*32]={0};
SDKErrCode VehicleType_TaskProcess(OSAL_HANDLE handle,OSAL_UCHAR* pImage,OSAL_INT32 iW,OSAL_INT32 iH,
	ImageDataType eType,TYPEResult* ppELAResult)
{
	int size_step_x=iW/64,size_step_y=iH/32;

    int proc_frame_inval=0;


	if (NULL==handle||NULL==pImage)
	{
	 	return SDK_ERR_PARAM;
	}

	if(size_step_x==0||size_step_y==0)
	{
	 	return SDK_ERR_PARAM;
	}

	resize_64(pImage,s_temp_image,iW,iH,size_step_x,size_step_y);


	VL_GetImageHogFeature(data_image,s_temp_image, 64, 32,nMeasure,nTheta,nHist);


	int ret = BPpredict((Bp*)(handle),data_image);

	//int ret = BPpredict(pBP,data_image);

	*ppELAResult = Type_other;

	if (ret==0)
	{
		*ppELAResult = Type_Truck;
	}
	if (ret==1)
	{
		*ppELAResult = Type_Car;
	}
	if (ret==2)
	{
		*ppELAResult = Type_MiniBus;
	}

	return SDK_ERR_NONE;
}

SDKErrCode   VehicleType_GetParam(OSAL_HANDLE Handle,OSAL_VOID *pParam)
{
	return SDK_ERR_NONE;
}

SDKErrCode VehicleType_TaskFree(OSAL_HANDLE *pHandle)
{

	Bprelease((Bp*)pHandle);

	free(*pHandle);
	*pHandle = OSAL_NULL;
	return SDK_ERR_NONE;
}
