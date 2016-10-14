/****************************************************************************************
********
********   文件名:   VLMain.c 
********   功能描述: 车标识别主体函数实现
********   版本:     V1.0
********   作者:     flybird
********   日期:      2016-9-7 19:38
********  
*****************************************************************************************/
#include "stdafx.h"
#include <math.h>
#include <malloc.h>
#include "string.h"
#include "VLMain.h"

#define  WINSIZEX    64   //图像X方向尺寸
#define  WINSIZEY    32    //图像Y方向尺寸
#define  ORIENDIM    9     //方向维度 
#define  ORIVALUE    20    //180/ORIENDIM
#define  BLOCKSIZE   4     //每个块含有细胞个数
#define  BLOCKFeaNum 36    //每个块特征相量数 ORIENDIM*BLOCKSIZE

#define  BLOCKXSIZE  16     //每个块X方向像素点个数
#define  BLOCKYSIZE  16     //每个块Y方向像素点个数

#define  MOVESTEPX  8      //X方向移动步长
#define  MOVESTEPY  8      //Y方向移动步长

#define  CELLXSIZE  8      //每个细胞X方向像素点个数
#define  CELLYSIZE  8      //每个细胞Y方向像素点个数


//////////////////////////////////内部函数////////////////////////////////////////
//提取HOG特征
//OSAL_INT32 VL_GetImageHogFeature(OSAL_INT32 *pFeature,OSAL_UCHAR *pImage,OSAL_UINT32 iW,OSAL_UINT32 iH,OSAL_INT32 *pnMeasure,OSAL_INT32 *pnTheta,OSAL_INT32 *pIntegHist);

OSAL_INT32 VL_ImageGamma(OSAL_PUCHAR pImage,OSAL_INT32 iW,OSAL_INT32 iH);

//计算梯度值
OSAL_INT32 VL_CalImageGradients(OSAL_INT32 *pnMeasure, OSAL_INT32 *pnTheta,OSAL_PUCHAR pImage,OSAL_INT32 iW,OSAL_INT32 iH);

//计算梯度直方图
OSAL_INT32 VL_CalIntegHist(OSAL_INT32 *pIntegHist,OSAL_INT32 *pnMeasure, OSAL_INT32 *pnTheta);

//计算HOG特征	
OSAL_INT32 VL_CalHOG(OSAL_INT32 *pFeature,OSAL_INT32 *pIntegHist,OSAL_INT32 *pnMeasure);

SDKErrCode VL_TaskCreate(OSAL_HANDLE *pHandle, OSAL_INT32 iMaxW, OSAL_INT32 iMaxH)
{
	return SDK_ERR_NONE;
}


SDKErrCode VL_TaskFree(OSAL_HANDLE *pHandle)
{
	return SDK_ERR_NONE;
}


SDKErrCode VL_TaskProcess(OSAL_HANDLE handle, OSAL_UCHAR* pImage, OSAL_INT32 iW, OSAL_INT32 iH,ImageDataType eType,PTVLResult* ppVFResult)
{



	return SDK_ERR_NONE;
}



//////////////////////////////////内部函数////////////////////////////////////////


//提取HOG特征
OSAL_INT32 VL_GetImageHogFeature(OSAL_INT32 *pFeature,OSAL_UCHAR *pImage,OSAL_UINT32 iW,OSAL_UINT32 iH,OSAL_INT32 *pnMeasure,OSAL_INT32 *pnTheta,OSAL_INT32 *pIntegHist)
{
	//对图像进行Gamma矫正
	VL_ImageGamma(pImage,iW,iH);
	//图像平滑

	//1计算梯度值
	VL_CalImageGradients(pnMeasure,pnTheta,pImage,iW,iH);

	//2计算积分直方图
	VL_CalIntegHist(pIntegHist,pnMeasure,pnTheta);

	//3计算HOG特征	
	VL_CalHOG(pFeature,pIntegHist,pnMeasure);

	return 0;
}

OSAL_INT32 VL_ImageGamma(OSAL_PUCHAR pImage,OSAL_INT32 iW,OSAL_INT32 iH)
{
	OSAL_INT32 i,j;
	OSAL_PUCHAR lpSrc=OSAL_NULL, lpSrcTemp=OSAL_NULL;

	lpSrc = pImage;

	for (i=0;i<iH;i++)
	{
		lpSrcTemp = lpSrc;
		for (j=0;j<iW;j++)
		{
			*lpSrcTemp =(OSAL_UCHAR) sqrt((double)(*lpSrcTemp));
			lpSrcTemp++;
		}
		lpSrc += iW;
	}
	return 0;
}

#if 0
//计算梯度值
OSAL_INT32 VL_CalImageGradients(OSAL_INT32 *pnMeasure, OSAL_INT32 *pnTheta,OSAL_PUCHAR pImage,OSAL_INT32 iW,OSAL_INT32 iH)
{
	OSAL_INT32 i,j,iSrcTmp1,iSrcTmp2,iSrcNextTmp1,iSrcNextTmp2,nPerLine,GX,GY;
	OSAL_PINT32 pDstMea = OSAL_NULL,pDstMeaTemp = OSAL_NULL,pDstThe = OSAL_NULL,pDstTheTemp = OSAL_NULL;
	OSAL_PUCHAR lpSrc1=OSAL_NULL, lpSrcTemp1=OSAL_NULL,lpSrc2=OSAL_NULL, lpSrcTemp2=OSAL_NULL;

	pDstMea = pnMeasure;
	pDstThe = pnTheta;
	//边界行列赋0
	memset(pDstMea,0,iW*sizeof(OSAL_INT32));
	memset(pDstThe,0,iW*sizeof(OSAL_INT32));
	for (i = 0; i < iH; i++)
	{
		*pDstMea = 0;
		*pDstThe = 0;
		pDstMea += iW;
		pDstThe += iW;
	}
	pDstMea = pnMeasure+iW-1;
	pDstThe = pnTheta+iW-1;
	for (i = 0; i < iH; i++)
	{
		*pDstMea = 0;
		*pDstThe = 0;
		pDstMea += iW;
		pDstThe += iW;
	}
	pDstMea = pnMeasure+iW*(iH-1);
	pDstThe = pnTheta+iW*(iH-1);
	memset(pDstMea,0,iW*sizeof(OSAL_INT32));
	memset(pDstThe,0,iW*sizeof(OSAL_INT32));

	//计算其他像素点的x,y方向梯度值
	nPerLine = (iW<<1);
	pDstMea = pnMeasure+iW+1;
	pDstThe = pnTheta+iW+1;
	lpSrc1 = pImage+iW;
	lpSrc2 = pImage+1;
	for (i = 1; i < iH-1; i++)
	{
		pDstMeaTemp = pDstMea;
		pDstTheTemp = pDstThe;
		lpSrcTemp1 = lpSrc1;
		lpSrcTemp2 = lpSrc2;
		for (j = 1; j < iW-1; j++)
		{
			//X方向值
			iSrcTmp1 = *lpSrcTemp1;
			iSrcNextTmp1 = *(lpSrcTemp1+2);
			//Y方向值
			iSrcTmp2 = *lpSrcTemp2;
			iSrcNextTmp2 = *(lpSrcTemp2+nPerLine);

			//计算梯度
			GX = iSrcNextTmp1-iSrcTmp1;
			GY = iSrcNextTmp2-iSrcTmp2;
			//计算梯度幅值和方向
			(*pDstMeaTemp) = (OSAL_INT32)(sqrt((double)(GX)*(GX) + (GY)*(GY))+0.5);
			(*pDstTheTemp) = (OSAL_INT32)(atan2((double)(GY), (double)(GX))*57.3);

			//将角度转换到直方图区间
			(*pDstTheTemp) = ((*pDstTheTemp)<0?((*pDstTheTemp)+180)/ORIVALUE:(*pDstTheTemp)/ORIVALUE);
			if((*pDstTheTemp) >= ORIENDIM || (*pDstTheTemp) < 0)
			{//越界处理
				(*pDstTheTemp) = ORIENDIM-1;
			}
			pDstMeaTemp++;
			pDstTheTemp++;
			lpSrcTemp1++;
			lpSrcTemp2++;
		}
		pDstMea += iW;
		pDstThe += iW;
		lpSrc1 += iW;
		lpSrc2 += iW;
	}
	return 0;
}

#endif


#if 0
//计算梯度值
OSAL_INT32 VL_CalImageGradients(OSAL_INT32 *pnMeasure, OSAL_INT32 *pnTheta,OSAL_PUCHAR pImage,OSAL_INT32 iW,OSAL_INT32 iH)
{
	OSAL_INT32 i,j,iSrcTmp1,iSrcTmp2,iSrcNextTmp1,iSrcNextTmp2,nPerLine,iTemp;
	OSAL_PINT32 pDst1 = OSAL_NULL,pDstTemp1 = OSAL_NULL,pDst2 = OSAL_NULL,pDstTemp2 = OSAL_NULL;
	OSAL_PINT32 pSrc1 = OSAL_NULL,pSrcTemp1 = OSAL_NULL,pSrc2 = OSAL_NULL,pSrcTemp2 = OSAL_NULL;
	OSAL_PUCHAR lpSrc1=OSAL_NULL, lpSrcTemp1=OSAL_NULL,lpSrc2=OSAL_NULL, lpSrcTemp2=OSAL_NULL;
	OSAL_DBL64 fTheta = 0.0,mag = 0.0,fTemp = 0.0,angle;
	int * xsub,*ysub;
	xsub=(int*)malloc(iW*iH*sizeof(int));
	ysub=(int*)malloc(iW*iH*sizeof(int));

	pDst1 = xsub+1;
	lpSrc1 = pImage;
	//计算X方向梯度
	for (i=0;i<iH;i++)
	{
		pDstTemp1 = pDst1;
		lpSrcTemp1 = lpSrc1;
		for (j=1;j<(iW-1);j++)
		{
			*pDstTemp1 = (*(lpSrcTemp1+2))-(*lpSrcTemp1);

			pDstTemp1++;
			lpSrcTemp1++;
		}
		pDst1 += iW;
		lpSrc1 += iW;
	}
	pDst1 = xsub;
	for (i=0;i<iH;i++)
	{
		*pDst1 = *(pDst1+1);
		*(pDst1+iW-1) = *(pDst1+iW-2);
		pDst1 += iW;
	}

	pDst1 = ysub+iW;
	lpSrc1 = pImage;
	//计算Y方向梯度
	for (i=1;i<iH-1;i++)
	{
		pDstTemp1 = pDst1;
		lpSrcTemp1 = lpSrc1;
		for (j=0;j<iW;j++)
		{
			*pDstTemp1 = (*(lpSrcTemp1+2*iW))-(*lpSrcTemp1);

			pDstTemp1++;
			lpSrcTemp1++;
		}
		pDst1 += iW;
		lpSrc1 += iW;
	}

	pDst1 = ysub;
	for (j=0;j<iW;j++)
	{
		*pDst1 = *(pDst1+iW);
		*(pDst1+iW*(iH-1)) = *(pDst1+iW*(iH-2));
		pDst1++;
	}

	pDst1 = pnTheta;
	pDst2 = pnMeasure;
	pSrc1 = xsub;  
	pSrc2 = ysub;

	//计算角度/模值
	for (i=0;i<iH;i++)
	{
		pDstTemp1 = pDst1;
		pDstTemp2 = pDst2;
		pSrcTemp1 = pSrc1;
		pSrcTemp2 = pSrc2;
		for (j=0;j<iW;j++)
		{
			//计算梯度幅值和方向
			fTheta = (OSAL_INT32)(atan2((double)(*pSrcTemp2), (double)(*pSrcTemp1))*57.3);	
			mag = (OSAL_INT32)(sqrt((double)(*pSrcTemp1)*(*pSrcTemp1) + (*pSrcTemp2)*(*pSrcTemp2))+0.5);

			fTheta = (fTheta<0?(fTheta+180):fTheta);
			fTheta = (fTheta>180?(fTheta-180):fTheta);

			fTemp = fTheta/ORIVALUE;
			iTemp = (fTemp+0.5);
			if (fTemp>=iTemp)
			{//第二区间为左侧区间
				(*pDstTemp1) = iTemp;
				angle = 1.0-(((*pDstTemp1)+0.5)-fTemp);
				pDstTemp1++;
				(*pDstTemp1) = ((iTemp<=0)?(ORIENDIM-1):iTemp-1);
				pDstTemp1++;
			}
			else 
			{//第二区间为右侧区间
				(*pDstTemp1) = (OSAL_INT32)fTemp;
				angle = 1.0-(fTemp-((*pDstTemp1)+0.5));
				pDstTemp1++;
				(*pDstTemp1) = ((iTemp>ORIENDIM-1)?0:iTemp+1);
				pDstTemp1++;
				
			}
			(*pDstTemp2) = mag*angle;
			pDstTemp2++;
			(*pDstTemp2) = mag*(1.f-angle);
			pDstTemp2++;
			//将角度转换到直方图区间
			//pDstTemp1++;
			//pDstTemp2++;
			pSrcTemp1++;
			pSrcTemp2++;
		}
		
		pDst1 += iW*2;
		pDst2 += iW*2;
		pSrc1 += iW;
		pSrc2 += iW;
	}

	free(xsub);
	free(ysub);

	return 0;
}
#endif 

//计算梯度值
OSAL_INT32 VL_CalImageGradients(OSAL_INT32 *pnMeasure, OSAL_INT32 *pnTheta,OSAL_PUCHAR pImage,OSAL_INT32 iW,OSAL_INT32 iH)
{
	OSAL_INT32 i,j,iSrcTmp1,iSrcTmp2,iSrcNextTmp1,iSrcNextTmp2,nPerLine,GX,GY,iTemp;
	OSAL_PINT32 pDst1 = OSAL_NULL,pDstTemp1 = OSAL_NULL,pDst2 = OSAL_NULL,pDstTemp2 = OSAL_NULL;
	OSAL_PINT32 pSrc1 = OSAL_NULL,pSrcTemp1 = OSAL_NULL,pSrc2 = OSAL_NULL,pSrcTemp2 = OSAL_NULL;
	OSAL_PUCHAR lpSrc1=OSAL_NULL, lpSrcTemp1=OSAL_NULL,lpSrc2=OSAL_NULL, lpSrcTemp2=OSAL_NULL;
	int * xsub,*ysub;
	xsub=(int*)malloc(iW*iH*sizeof(int));
	ysub=(int*)malloc(iW*iH*sizeof(int));

	pDst1 = xsub+1;
	lpSrc1 = pImage;
	//计算X方向梯度
	for (i=0;i<iH;i++)
	{
		pDstTemp1 = pDst1;
		lpSrcTemp1 = lpSrc1;
		for (j=1;j<(iW-1);j++)
		{
			*pDstTemp1 = (*(lpSrcTemp1+2))-(*lpSrcTemp1);

			pDstTemp1++;
			lpSrcTemp1++;
		}
		pDst1 += iW;
		lpSrc1 += iW;
	}
	pDst1 = xsub;
	for (i=0;i<iH;i++)
	{
		*pDst1 = *(pDst1+1);
		*(pDst1+iW-1) = *(pDst1+iW-2);
		pDst1 += iW;
	}

	pDst1 = ysub+iW;
	lpSrc1 = pImage;
	//计算Y方向梯度
	for (i=1;i<iH-1;i++)
	{
		pDstTemp1 = pDst1;
		lpSrcTemp1 = lpSrc1;
		for (j=0;j<iW;j++)
		{
			*pDstTemp1 = (*(lpSrcTemp1+2*iW))-(*lpSrcTemp1);

			pDstTemp1++;
			lpSrcTemp1++;
		}
		pDst1 += iW;
		lpSrc1 += iW;
	}

	pDst1 = ysub;
	for (j=0;j<iW;j++)
	{
		*pDst1 = *(pDst1+iW);
		*(pDst1+iW*(iH-1)) = *(pDst1+iW*(iH-2));
		pDst1++;
	}

	pDst1 = pnTheta;
	pDst2 = pnMeasure;
	pSrc1 = xsub;  
	pSrc2 = ysub;

	//计算角度/模值
	for (i=0;i<iH;i++)
	{
		pDstTemp1 = pDst1;
		pDstTemp2 = pDst2;
		pSrcTemp1 = pSrc1;
		pSrcTemp2 = pSrc2;
		for (j=0;j<iW;j++)
		{
			//计算梯度幅值和方向
			(*pDstTemp1) = (OSAL_INT32)(atan2((double)(*pSrcTemp2), (double)(*pSrcTemp1))*57.3);	
			(*pDstTemp2) = (OSAL_INT32)(sqrt((double)(*pSrcTemp1)*(*pSrcTemp1) + (*pSrcTemp2)*(*pSrcTemp2))+0.5);
			iTemp = (*pDstTemp1);
			//将角度转换到直方图区间
			(*pDstTemp1) = ((*pDstTemp1)<0?((*pDstTemp1)+180)/ORIVALUE:(*pDstTemp1)/ORIVALUE);
			//if((*pDstTemp1) >= ORIENDIM || (*pDstTemp1) < 0)
			//{//越界处理
			//	(*pDstTemp1) = ORIENDIM-1;
			//}
			(*pDstTemp1) =((*pDstTemp1) >= ORIENDIM?(*pDstTemp1-1):(*pDstTemp1));
			pDstTemp1++;
			pDstTemp2++;
			pSrcTemp1++;
			pSrcTemp2++;
		}

		pDst1 += iW;
		pDst2 += iW;
		pSrc1 += iW;
		pSrc2 += iW;
	}

	free(xsub);
	free(ysub);

	return 0;
}

//计算梯度直方图
OSAL_INT32 VL_CalIntegHist(OSAL_INT32 *pIntegHist,OSAL_INT32 *pnMeasure, OSAL_INT32 *pnTheta)
{
	OSAL_INT32 i,j,k,l,iCellNumX,iCellNumY;
	OSAL_PINT32 pDstHist = OSAL_NULL,pHistTemp = OSAL_NULL,pSrcMea = NULL,pMeaTemp = OSAL_NULL,pMeaTemp2 = OSAL_NULL,pSrcTheta = OSAL_NULL,pThetaTemp = OSAL_NULL,pThetaTemp2 = OSAL_NULL;

	//(假设图像尺寸为64*32 细胞大小为8*8 方向维度为9 直方图个数=(64*32/(8*8))*9)
	iCellNumX = WINSIZEX/CELLXSIZE;
	iCellNumY = WINSIZEY/CELLYSIZE;
	memset(pIntegHist,0,iCellNumX*iCellNumY*ORIENDIM*sizeof(OSAL_INT32));
	pDstHist = pIntegHist;
	pSrcMea = pnMeasure;
	pSrcTheta = pnTheta;

	for (j = 0; j < iCellNumY; j++)
	{
		pMeaTemp = pSrcMea;
		pThetaTemp = pSrcTheta;	

		for (i= 0; i < iCellNumX; i++)
		{
			pMeaTemp2 = pMeaTemp;
			pThetaTemp2 = pThetaTemp;
			//计算每个细胞的梯度直方图
			for (l = 0;l < CELLYSIZE;l++)
			{
				for (k = 0; k < CELLXSIZE;k++)
				{
					pDstHist[*(pThetaTemp2+k)] += (*(pMeaTemp2+k));
					//pDstHist[*(pThetaTemp2+2*k)] += (*(pMeaTemp2+2*k));
					//pDstHist[*(pThetaTemp2+2*k+1)] += (*(pMeaTemp2+2*k+1));
				}
				pMeaTemp2 += WINSIZEX;
				pThetaTemp2 += WINSIZEX;
			}
			
			pMeaTemp += CELLXSIZE;
			pThetaTemp += CELLXSIZE;

			pDstHist+=ORIENDIM;

		}
		pSrcMea += CELLYSIZE*WINSIZEX;
		pSrcTheta += CELLYSIZE*WINSIZEX;
	}
	return 0;
}

#if 1
//计算HOG特征	
OSAL_INT32 VL_CalHOG(OSAL_INT32 *pFeature,OSAL_INT32 *pIntegHist,OSAL_INT32 *pnMeasure)
{
	OSAL_INT32 i,j,l,k,iBlockNumX,iBlockNumY,nPerline,nHistStep[3],iCellNumX,iCellNumY,iCellRowNum,iCellColNum,CopySize;
	OSAL_DBL64 Sum;
	OSAL_PINT32 pDstFea = OSAL_NULL,pDstFeaTemp = OSAL_NULL,pSrcHist = OSAL_NULL,pSrcHistTemp = OSAL_NULL,pSrcHistTemp2 = OSAL_NULL;

	iCellNumX = WINSIZEX/CELLXSIZE;
	iCellNumY = WINSIZEY/CELLYSIZE;

	iCellRowNum = BLOCKYSIZE/CELLYSIZE;
	iCellColNum = BLOCKXSIZE/CELLXSIZE;
	CopySize = sizeof(int)*ORIENDIM*iCellColNum;

	iBlockNumX = WINSIZEX/MOVESTEPX-1;
	iBlockNumY = WINSIZEY/MOVESTEPY-1;
	
	nPerline = ORIENDIM*BLOCKXSIZE/CELLXSIZE;
	nHistStep[0] = (MOVESTEPY/CELLYSIZE)*(WINSIZEX/CELLXSIZE)*ORIENDIM;
	nHistStep[1] = MOVESTEPX/CELLXSIZE*ORIENDIM;
	nHistStep[2] =(WINSIZEX/CELLXSIZE)*ORIENDIM;

	pDstFea= pFeature;
	pSrcHist = pIntegHist;
	for(j = 0; j < iBlockNumY; j++)
	{
		pSrcHistTemp = pSrcHist;
		for (i = 0; i < iBlockNumX; i++)
		{
			pDstFeaTemp = pDstFea;
			pSrcHistTemp2 = pSrcHistTemp;
			for (l=0;l<iCellRowNum;l++)
			{	
				memcpy(pDstFea,pSrcHistTemp2,CopySize);
				pDstFea += nPerline;
				pSrcHistTemp2 += nHistStep[2];
			}	
			Sum = 0.0;
			//进行归一化处理
			for (k=0;k<iCellRowNum*iCellColNum*ORIENDIM;k++)
			{
				Sum += pDstFeaTemp[k]*pDstFeaTemp[k];
			}
			Sum++; //防止分母为0
			Sum = sqrt((double)Sum);
			for (k=0;k<iCellRowNum*iCellColNum;k++)
			{
				 pDstFeaTemp[k] = (pDstFeaTemp[k])*1000000/Sum;
			}

			pSrcHistTemp += nHistStep[1];
		}
		pSrcHist += nHistStep[0];
	}

	return 0;
}

#endif 

#if 0
#define  OSAL_SUCCESS 0
#define  OSAL_FAILED  1
#define BLOCK_HOG_FEA_LEN  ((BLOCKSIZE)*(ORIENDIM))

/************************************************************************/
/* 灰度图像尺寸变换                                                     */
/************************************************************************/
/*
*函数名		  : vrsGrayResize
*功能描述	  : 将灰度图像尺寸变换，采用双线形方法
*输入参数说明 : pu8Src---灰度图像数据
			    n32Width---原始图像宽度
			    n32Height---原始图像高度
				newWidth---目标图像宽度
			    newHeight---目标图像高度
*输出参数说明 : Dst---尺寸变换后图像
*返回值说明	  : 执行成功返回OSAL_TRUE, 否则返回OSAL_FALSE
*创建日期	  : 2012-7-2
*/
OSAL_BOOL vtrsGrayResize(OSAL_UCHAR *Src,const OSAL_INT32 n32Width,const OSAL_INT32 n32Height,
					 const OSAL_INT32 newWidth,const OSAL_INT32 newHeight,OSAL_UCHAR *Dst)
{
	OSAL_INT32 n32PixelNum=n32Width*n32Height;
	OSAL_INT32 new32PixelNum=newWidth*newHeight;
	OSAL_INT32 x,y,yDownLoc,xLeftLoc;

	OSAL_INT32 yDis=(n32Height*100-110)/(newHeight-1);
	OSAL_INT32 xDis=(n32Width*100-110)/(newWidth-1);

	OSAL_INT32 yDownDis,yUpDis,xLeftDis,xRightDis;
	OSAL_INT32 yLoc,xLoc,left,right;

	for(y=0;y<newHeight;y++)
	{
		for(x=0;x<newWidth;x++)
		{
			xLoc=x*xDis;
			yLoc=y*yDis;
			yDownLoc=yLoc/100;
			xLeftLoc=xLoc/100;
			yDownDis=yLoc-yDownLoc*100;
			yUpDis=100-yDownDis;
			xLeftDis=xLoc-xLeftLoc*100;
			xRightDis=100-xLeftDis;

			//双线性插值
			left = Src[(yDownLoc+1)*n32Width+xLeftLoc]*yDownDis+
				Src[(yDownLoc)*n32Width+xLeftLoc]*yUpDis;
			right = Src[(yDownLoc+1)*n32Width+xLeftLoc+1]*yDownDis+
				Src[(yDownLoc)*n32Width+xLeftLoc+1]*yUpDis;

			left /= 100;
			right /= 100;

			Dst[y*newWidth+x]=(OSAL_UCHAR)((left*xRightDis+right*xLeftDis)/100);
		}
	}

	return OSAL_SUCCESS;
}



/*
*函数名		  : vrsMediafilt
*功能描述	  : 图像中值滤波处理
*输入参数说明 : pu8Src---灰度图像数据
				n32Width---图像宽度
				n32Height---图像高度
*返回值说明	  : 执行成功返回OSAL_TRUE, 否则返回OSAL_FALSE
*创建日期	  : 2012-7-2
*/
OSAL_BOOL vtrsMediafilt(OSAL_UCHAR *pu8Src, const OSAL_INT32 n32Width, const OSAL_INT32 n32Height)
{
	OSAL_INT32 i, j, k, q, index;
	OSAL_UCHAR list[5];

	if (OSAL_NULL == pu8Src)
	{
		return -1;
	}

	//中值滤波
	for(i=0;i<n32Height;i++)
	{
		for(j=0;j<n32Width;j++)
		{
			for(k=0;k<5;k++)
				list[k]=0;
			if(i>=1)
				list[0]=(OSAL_UCHAR)pu8Src[(i-1)*n32Width+j];
			if(j>=1)
				list[1]=(OSAL_UCHAR)pu8Src[(i)*n32Width+j-1];
			if(i<=n32Height-2)
				list[4]=(OSAL_UCHAR)pu8Src[(i+1)*n32Width+j];
			if(j<=n32Width-2)
				list[3]=(OSAL_UCHAR)pu8Src[(i)*n32Width+j+1];
			list[2]=(OSAL_UCHAR)pu8Src[i*n32Width+j];

			//比较获取中间值
			for(k=0;k<3;k++)
			{
				index=0;
				for(q=1;q<5;q++)
					if(list[q]>list[index])
						index=q;
				if(k<2)
					list[index]=0;
			}
			pu8Src[i*n32Width+j]=list[index];
		}
	}

	return OSAL_SUCCESS;
}

/*
*函数名		  : vrsOneDimGuassfilter
*功能描述	  : 高斯滤波
*输入参数说明 : pu8Src---灰度图像
				n32Width---图像宽度
				n32Height---图像高度
				d64Sigma---滤波半径
*输出参数说明 :	pu8Dst---输出图像
*返回值说明	  : 执行成功返回OSAL_TRUE, 否则返回OSAL_FALSE
*创建日期	  : 2013-03-14
*/
OSAL_BOOL vtrsOneDimGuassfilter(OSAL_INT32 *pTmp1, OSAL_INT32 *pTmp2, const OSAL_UCHAR *pu8Src, const OSAL_INT32 n32Width, const OSAL_INT32 n32Height, OSAL_UCHAR *pu8Dst)
{
	OSAL_INT32 i, x, y, m;

	//一维高斯分布函数，用于平滑函数中生成的高斯滤波系数
	//定义滤波窗口的大小 
	OSAL_INT32 n32WindowSize = 5;   
	//定义滤波窗口中心的索引
	OSAL_INT32 n32Center = (n32WindowSize)/2;

	OSAL_INT32 sum;

	OSAL_INT32 pd64Kernel[5] = {3, 40387, 919217, 40387, 3};
	OSAL_INT32 *temp1, *temp2;

	if ((OSAL_NULL == pu8Src) || (0 >= n32Width) || (0 >= n32Height) || (OSAL_NULL == pu8Dst))
	{
		return OSAL_FALSE;
	}

	temp1 = pTmp1;
	SETVALUE(temp1, 0, OSAL_INT32, n32Width*n32Height);
	temp2 = pTmp2;
	SETVALUE(temp2, 0, OSAL_INT32, n32Width*n32Height);

	//水平方向滤波
	for( y=0;y<n32Height;y++)
	{
		for( x=0;x<n32Width;x++)
		{
			sum=0;
			for( m=-n32Center;m<=n32Center;m++)
			{
				if(m+x>=0 && m+x<n32Width)
				{
					sum=sum+pd64Kernel[m+n32Center]*pu8Src[y*n32Width+m+x];
				}
			}
			temp1[y*n32Width+x]=sum*1e-6;
		}
	}

	//垂直方向滤波
	for( x=0;x<n32Width;x++)
	{
		for( y=0;y<n32Height;y++)
		{
			sum=0;
			for( m=-n32Center;m<=n32Center;m++)
			{
				if(m+y>=0 && m+y<n32Height)
				{
					sum=sum+pd64Kernel[m+n32Center]*temp1[(y+m)*n32Width+x];
				}
			}
			temp2[y*n32Width+x]=sum*1e-6;
		}
	}

	for(i=0;i<n32Width*n32Height;i++)
	{
		pu8Dst[i]=(OSAL_UCHAR)temp2[i];
	}

	return OSAL_TRUE;
}

/************************************************************************/
/*HOG特征                                                               */
/************************************************************************/
/*
*函数名		  : HistExpand
*功能描述	  : 将图像的亮度范围扩展至0-255，线形扩展
*输入参数说明 : pu8Src---灰度图像数据
			    n32Width---图像宽度
			    n32Height---图像高度
*返回值说明	  : 执行成功返回OSAL_TRUE, 否则返回OSAL_FALSE
*创建日期	  : 2012-7-2
*/
OSAL_INT32 vtrsCalHistExpand(OSAL_UCHAR *pu8Src, const OSAL_INT32 n32Width, const OSAL_INT32 n32Height, OSAL_UCHAR *pu8Dst)
{
	OSAL_INT32 i = 0;
	OSAL_UCHAR *tempCur = OSAL_NULL;
	OSAL_UCHAR max = 0, min = 255, dis = 0;
	OSAL_UCHAR  tempValue = 0,*tempDst = OSAL_NULL;
	
	OSAL_INT32 n32PixelNum = n32Width*n32Height;
	OSAL_UCHAR *pu8SrcCur = pu8Src;

	if ((OSAL_NULL == pu8Src) || (OSAL_NULL == pu8Dst))
	{
		return -1;
	}

	//获取灰度图像灰度值最大范围
	for(i = 0; i < n32PixelNum; i++)
	{
		tempValue = (*pu8SrcCur);
		if(max < (tempValue))
		{
			max = (tempValue);
		}
		if(min > (tempValue))
		{
			min = (tempValue);
		}
		pu8SrcCur++;
	}
	dis = max - min;

	//对灰度图进行线性拉伸
	tempCur = pu8Src;
	tempDst = pu8Dst;
	for(i = 0; i< n32PixelNum; i++)
	{
		*tempDst = (((*tempCur) - min)*255/(dis+1));
		tempCur++;
		tempDst++;
	}

	return OSAL_SUCCESS;
}


OSAL_INT32 vtrsCalcGradients(OSAL_PUCHAR pImage, const OSAL_INT32 n32Width, const OSAL_INT32 n32Height, OSAL_INT32 *pnMeasure, OSAL_INT32 *pnTheta)
{
	OSAL_INT32 i = 0, j = 0;

	OSAL_INT32 d64SrcTmp1 = 0;
	OSAL_INT32 d64SrcTmp2 = 0;
	OSAL_INT32 d64SrcNextTmp1 = 0;
	OSAL_INT32 d64SrcNextTmp2 = 0;

	//初始化指针
	OSAL_UCHAR *pu8SrcCur, *pu8SrcTemp, *pu8SrcNext, *pu8SrcNextTemp;
	OSAL_INT32 pd64GradientX, pd64GradientY; 
	OSAL_INT32 *pn32MeasureCur, *pn32ThetaCur;

	if ((OSAL_NULL == pImage) || (0 >= n32Width) || (0 >= n32Height))
	{
		return -1;
	}

	pu8SrcCur = pImage;
	pu8SrcNext = pu8SrcCur + n32Width;

	pn32MeasureCur = pnMeasure;
	pn32ThetaCur = pnTheta;

	//计算x,y方向的偏导数
	for(i=0; i<n32Height; i++)   
	{   
		pu8SrcTemp = pu8SrcCur;
		pu8SrcNextTemp = pu8SrcNext;


		for(j=0; j<n32Width; j++)   
		{
			if ((j > n32Width-2) || (i > n32Height-2))
			{
				(pd64GradientX) = 0;
				(pd64GradientY) = 0;
			}
			else
			{
				d64SrcTmp1 = (*(pu8SrcTemp++));
				d64SrcTmp2 = (*pu8SrcTemp);
				d64SrcNextTmp1 = (*(pu8SrcNextTemp++));
				d64SrcNextTmp2 = (*pu8SrcNextTemp);

				//x方向梯度
				(pd64GradientX) = (d64SrcTmp2 - d64SrcTmp1 + d64SrcNextTmp2 - d64SrcNextTmp1)/2;

				//y方向梯度
				(pd64GradientY) = -(d64SrcNextTmp1 - d64SrcTmp1 + d64SrcNextTmp2 - d64SrcTmp2)/2;
			}

			//计算梯度幅值和梯度的方向
			(*pn32MeasureCur) = (sqrt((pd64GradientX)*(pd64GradientX) + (pd64GradientY)*(pd64GradientY))+0.5);
			(*pn32ThetaCur) = (atan2((pd64GradientY), (pd64GradientX))*57.3);
			// 			(*pn32MeasureCur) = (OSAL_INT32)(vtrsSqrt((pd64GradientX)*(pd64GradientX) + (pd64GradientY)*(pd64GradientY))+0.5);
			// 			(*pn32ThetaCur) = vrsAtan((pd64GradientY), (pd64GradientX));

			//将该角度转换到0  ~  360  度之间
			if((*pn32ThetaCur) < 0)
			{
				(*pn32ThetaCur) += 180;   
			}
			(*pn32ThetaCur) /= 20;
			if((*pn32ThetaCur) >= 9 || (*pn32ThetaCur) < 0)
			{
				(*pn32ThetaCur) = 8;
			}

			pn32ThetaCur++;
			pn32MeasureCur++;

		}

		pu8SrcCur += n32Width;
		pu8SrcNext += n32Width;
	}

	return OSAL_FALSE;
}


/*
*函数名		  : GetIntegHists
*功能描述	  : 计算积分直方图
*输入参数说明 : handle ---对象句柄
				n32Width---图像的宽度
				n32Height---图像的高度
*输出参数说明 : 无
*返回值说明	  : 执行成功返回OSAL_TRUE，执行失败返回OSAL_FALSE
*创建日期	  : 2013-03-14
*/

OSAL_INT32 vtrsCalcIntegHist(OSAL_INT32 *pnMeasure, OSAL_INT32 *pnTheta, OSAL_INT32 *TempHist, OSAL_INT32 *pIntegHist)
{
	OSAL_INT32 i = 0, j = 0, BinNum = 0;
	OSAL_INT32 *preLine = OSAL_NULL;
	OSAL_INT32 *pcurline;
	OSAL_INT32 *pCurrPixPos;
	OSAL_INT32 *m_pn32Measure;
	OSAL_INT32 *m_pn32Theta;
	OSAL_INT32 *pTempHist = TempHist;

	if ((OSAL_NULL == pnMeasure) || (OSAL_NULL == pnTheta) || (OSAL_NULL == pIntegHist))
	{
		return -1;
	}

	SETVALUE(pIntegHist,0,OSAL_INT32,(SIZE+1)*(SIZE+1)*ORIENDIM);	

	pcurline =pIntegHist + (SIZE+2)*ORIENDIM;
	pCurrPixPos = pcurline;
	m_pn32Measure = pnMeasure;
	m_pn32Theta = pnTheta;

	//计算积分图
	for (i = 0; i < SIZE; i++)
	{
		if (i != 0)
		{
			COPYVALUE(pcurline, preLine, OSAL_PINT32, SIZE*ORIENDIM);
		}

		pTempHist = TempHist;
		SETVALUE(pTempHist, 0, OSAL_INT32, ORIENDIM);

		for (j = 0; j < SIZE; j++)
		{
			TempHist[*(m_pn32Theta++)] += (*(m_pn32Measure++)) ;
			pTempHist = TempHist;
			for(BinNum = 0; BinNum < ORIENDIM; ++BinNum)
				(*(pCurrPixPos++)) += (*(pTempHist++)) ;
		}
		preLine = pcurline;
		pcurline += (SIZE+1)*ORIENDIM;
		pCurrPixPos += ORIENDIM;
	}

	return OSAL_SUCCESS; 
}

/*
*函数名		  : vrsCalcHOG
*功能描述	  : 统计图像的hog特征，
*输入参数说明 : handle---对象句柄
			    n32Width---图像宽度
			    n32Height---图像高度
*输出参数说明 : Dst---得到的特征值指针
*返回值说明	  : 执行成功返回OSAL_TRUE, 否则返回OSAL_FALSE
*创建日期	  : 2013-03-14
*/
OSAL_BOOL vtrsCalcHOG(OSAL_INT32 *pHist, const OSAL_INT32 n32Width,const OSAL_INT32 n32Height, OSAL_INT32 *tempHist, OSAL_INT32 *pFeature)
{
	//计算HOG特征
	OSAL_INT32 i = 0, j = 0;
	OSAL_INT32 stepX = SIZE/CELLXSIZE;
	OSAL_INT32 stepY = SIZE/CELLYSIZE;
	OSAL_INT32 *pIntegHist = pHist;
	OSAL_INT32 *pIntegHist1 = OSAL_NULL;
	OSAL_INT32 *pIntegHist2 = OSAL_NULL;
	OSAL_INT32 *pIntegHist3 = OSAL_NULL;
	OSAL_INT32 *pIntegHist4 = OSAL_NULL;
	OSAL_INT32 BinNum = 0;
	OSAL_INT32 *pDst = pFeature;
	OSAL_INT32 *pTempHist, *description = tempHist + CELLYSIZE*CELLXSIZE*ORIENDIM;
	OSAL_INT32 *pTempHist1, *pTempHist2;

	if ((OSAL_NULL == pIntegHist) || (OSAL_NULL == pFeature))
	{
		return -1;
	}

	//根据积分图计算每一块的相应特征
	pTempHist = tempHist;
	for (i = 0; i < CELLYSIZE; i ++)
	{
		for (j = 0; j < CELLXSIZE; j ++)
		{
			pIntegHist4 = pIntegHist + ((i+1) *stepY* (SIZE+1) + (j+1)*stepX) * ORIENDIM;
			pIntegHist3 = pIntegHist + ((i+1) *stepY* (SIZE+1) + j*stepX) * ORIENDIM;
			pIntegHist2 = pIntegHist + ( i*stepY* (SIZE+1) + (j+1)*stepX) * ORIENDIM;
			pIntegHist1 = pIntegHist + (i*stepY* (SIZE+1) + j*stepX) * ORIENDIM;

			for (BinNum = 0; BinNum < ORIENDIM; BinNum++)
			{
				*pTempHist++ = (*pIntegHist4++) - (*pIntegHist3++) - 
					         (*pIntegHist2++) + (*pIntegHist1++);
			}
		}
	}
	
	pTempHist1 = tempHist;
	pTempHist2 = tempHist+CELLXSIZE*ORIENDIM;

	//计算每一维的特征值
	for (j = 1; j <= Stepx*Stepy; ++j)
	{

		OSAL_INT32 *pDescription = description;
		OSAL_INT32 sum = 0 ;
		OSAL_INT32 HistBin = 0 ;

		for(HistBin=0;HistBin<2*ORIENDIM;++HistBin)
		{
			*pDescription++ =  *pTempHist1;
		//	sum += *pTempHist1++;
			pTempHist1++;
		}

		for(HistBin = 0; HistBin < 2*ORIENDIM; ++HistBin)
		{
			*pDescription++ =  *pTempHist2;
		//	sum += *pTempHist2++;
			pTempHist2++;
		}

		if(j%3 !=0)
		{
			pTempHist1 -= (1*ORIENDIM) ;
			pTempHist2 -= (1*ORIENDIM) ;
		}

		pDescription = description;
		sum = 48*255;
		for(i=0;i<BLOCK_HOG_FEA_LEN;i++)
		{
			(*(pDst++)) = ((*(pDescription++))*100)/sum;
		}
	
	}

	return OSAL_TRUE;
}


/*
*函数名		  : vrsCalcHOGFeature
*功能描述	  : 计算整型HOG特征
*输入参数说明 : handle ---对象句柄
				n32Width---图像的宽度
				n32Height---图像的高度
*输出参数说明 : Dst---特征值数据指针
*返回值说明	  : 执行成功返回OSAL_TRUE，执行失败返回OSAL_FALSE
*创建日期	  : 2013-03-14
*/
OSAL_BOOL vtrsCalcHOGFeature(OSAL_PUCHAR pLogoSrc, OSAL_INT32 nWidth, OSAL_INT32 nHeight, OSAL_PUCHAR pTmpSrc1, OSAL_PUCHAR pTmpSrc2, 
						   OSAL_INT32 nRW, OSAL_INT32 nRH, OSAL_INT32 *pnMeasure, OSAL_INT32 *pnTheta, OSAL_INT32 *pIntegHist, OSAL_INT32  *pFeature)
{
	//图像规整处理
	vtrsGrayResize(pLogoSrc, nWidth, nHeight, nRW, nRH, pTmpSrc1);

	//中值滤波
	vtrsMediafilt(pTmpSrc1, nRW, nRH);

	//高斯滤波
	vtrsOneDimGuassfilter(pnMeasure, pnTheta, pTmpSrc1, nRW, nRH, pTmpSrc2);

	//灰度拉伸
	vtrsCalHistExpand(pTmpSrc2, nRW, nRH, pTmpSrc1);

	//计算梯度值
	vtrsCalcGradients(pTmpSrc1, nRW, nRH, pnMeasure, pnTheta);

	//计算积分直方图
	vtrsCalcIntegHist(pnMeasure, pnTheta, pFeature, pIntegHist);

	//计算HOG特征
	vtrsCalcHOG(pIntegHist, nRW, nRH, pnMeasure, pFeature);

	return OSAL_SUCCESS;
}

#endif 

















