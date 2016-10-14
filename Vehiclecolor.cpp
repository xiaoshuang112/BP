#include "stdafx.h"
#include <windows.h>
#include "Vehiclecolor.h"
#include <stdio.h>

#define MASK 0x80
#define MAX_MASK_AREA 40

#define RGB2YUV_SHIFT 8 /*16*/
#define BY  25            /*((int)( 0.098*(1<<RGB2YUV_SHIFT)+0.5))*/
#define BV  -17           /*((int)(-0.071*(1<<RGB2YUV_SHIFT)+0.5))*/
#define BU  112           /*((int)( 0.439*(1<<RGB2YUV_SHIFT)+0.5))*/
#define GY  129           /*((int)( 0.504*(1<<RGB2YUV_SHIFT)+0.5))*/
#define GV  -93           /*((int)(-0.368*(1<<RGB2YUV_SHIFT)+0.5))*/
#define GU  -73           /*((int)(-0.291*(1<<RGB2YUV_SHIFT)+0.5))*/
#define RY  66            /*((int)( 0.257*(1<<RGB2YUV_SHIFT)+0.5))*/
#define RV  112           /*((int)( 0.439*(1<<RGB2YUV_SHIFT)+0.5))*/
#define RU  -37           /*((int)(-0.148*(1<<RGB2YUV_SHIFT)+0.5))*/
#define YM  298            /*((int)( 1.164*(1<<RGB2YUV_SHIFT)+0.5))*/
#define UB  517            /*((int)( 2.018*(1<<RGB2YUV_SHIFT)+0.5))*/
#define VG  -207           /*((int)(-0.813*(1<<RGB2YUV_SHIFT)+0.5))*/
#define UG  -99            /*((int)(-0.391*(1<<RGB2YUV_SHIFT)+0.5))*/
#define VR  409            /*((int)( 1.596*(1<<RGB2YUV_SHIFT)+0.5)) */
#define BF_ALIGNBYTE4(value) ((value+3)>>2)
#define BF_ALIGNBYTE8(value) ((value+7)>>3)
#define BF_ALIGNBYTE32(value) ((value+31)>>5)
#define IMAGEWIDTHBYTES(bits)  ((((bits) + 31) >> 5) << 2)

#define BF_TAIBIT(value,x) (((value)<<(x))>>(x))

#define BF_GRAYVALUE(B,G,R) ((R*38+G*75+B*15)>>7)
#define GETU8SCALE(Y)   ((Y) > 255 ? 255 : (Y) < 0 ? 0 : (Y))
#define MIN(x,y,z) ((x)<(y)?(x):(y))<(z)?((x)<(y)?(x):(y)):(z)
#define MAX(x,y,z) ((x)>(y)?(x):(y))>(z)?((x)>(y)?(x):(y)):(z)

#define Vc_w  20
#define Vc_h  10

void  Vc_YUV420SP2BGR(OSAL_PUCHAR pDstImg,OSAL_INT32 iW,OSAL_INT32 iH, const OSAL_PUCHAR pSrcImg, OSAL_INT32 iWidth, OSAL_INT32 iHeight,VRECT *pROIRect)
{
	OSAL_PUCHAR REST pYCur1, pYCur2, pUCur, pVCur, pCur1, pCur2, pCur1Temp2, pCur2Temp2;
	OSAL_PUCHAR REST pYCur1Temp, pYCur2Temp, pUCurTemp, pVCurTemp, pCur1Temp, pCur2Temp;
	OSAL_INT32  i, j, nWidthDiv, nWidthPerLine;
	OSAL_INT32  Y1, Y2, Y3, Y4, U, V, B, G, R, B2, G2, R2, B3, G3, R3, B4, G4, R4;
	OSAL_INT32  nUBU, nVGV, nUGU, nVRV;
	OSAL_UCHAR  y1, y2, y3, y4, u, v, b, g, r,b2, g2, r2,b3, g3, r3,b4, g4, r4;

	VRECT ROIRect = {0};
	if(pROIRect == OSAL_NULL)
	{
		ROIRect.left = 0;
		ROIRect.top = 0;
		ROIRect.right = iWidth;
		ROIRect.bottom = iHeight;
	}
	else
	{
		//ROIRect = *pROIRect;
		//感兴趣区域必须四字节对齐
		ROIRect.left = ((pROIRect->left>>2)<<2);
		ROIRect.top = ((pROIRect->top>>1)<<1);
		ROIRect.right = ((BF_ALIGNBYTE4(pROIRect->right))<<2);
		ROIRect.bottom = ((pROIRect->bottom>>1)<<1);
	}

	if( pSrcImg == OSAL_NULL || pDstImg == OSAL_NULL)
	{
		return;
	}


	//nWidthDiv   = (ROIRect.right-ROIRect.left)/2;
	nWidthDiv   = iW/2;
	nWidthPerLine = IMAGEWIDTHBYTES(24*nWidthDiv);
	pYCur1 = (((OSAL_UINT8 *) pSrcImg) + ROIRect.top*iWidth + ROIRect.left);//pSrcData;第一行
	pYCur2 = pYCur1 + iWidth;//下一行
	pUCur  = pSrcImg + iWidth *(iHeight+ROIRect.top/2)+ROIRect.left;
	pVCur  = pUCur + 1; //pSrcData + nw *(nh*5/4);
	pCur1  = pDstImg/*+ROIRect.top*nWidthPerLine+ROIRect.left*3*/;
	pCur2  = pCur1 + nWidthPerLine;


	/*根据YUV的特性，注意两行一起处理，四个RGB像素点共有一个UV*/
	//for(i=ROIRect.top; i <ROIRect.bottom; i+= 2)
	for(i = 0; i < iH; i+= 2)
	{
		pYCur1Temp = pYCur1;
		pYCur2Temp = pYCur2;
		pUCurTemp  = pUCur;
		pVCurTemp  = pVCur;
		pCur1Temp  = pCur1;
		pCur2Temp  = pCur2;

		pCur1Temp2 =  pCur1Temp + 3;
		pCur2Temp2 =  pCur2Temp + 3;

		for (j=0; j<nWidthDiv; j++)
		{
			// 1.首先求出Y,U,V三个分量
			y1 = *(pYCur1Temp );
			y2 = *(pYCur1Temp + 1);
			y3 = *(pYCur2Temp );
			y4 = *(pYCur2Temp + 1);
			pYCur1Temp += 2;
			pYCur2Temp += 2;

			u  = *(pUCurTemp ++);
			v  = *(pVCurTemp ++);

			pUCurTemp ++;
			pVCurTemp ++;

			// 2.用公式求出R，G，B三个分量
			// YCrCb转换成BGR应用如下公式计算
			//	B = 1.164(Y - 16) + 2.018(U - 128)
			//	G = 1.164(Y - 16) - 0.813(V - 128) - 0.391(U - 128)
			//	R = 1.164(Y - 16) + 1.596(V - 128)

			Y1 = YM*(y1 - 16);
			Y2 = YM*(y2 - 16);
			Y3 = YM*(y3 - 16);
			Y4 = YM*(y4 - 16);
			U  = u - 128;
			V  = v - 128;

			nUBU = UB*U;
			nVGV = VG*V;
			nUGU = UG*U;
			nVRV = VR*V;

			B = ((Y1 + nUBU) >> RGB2YUV_SHIFT);
			G = ((Y1 + nVGV + nUGU) >> RGB2YUV_SHIFT);
			R = ((Y1 + nVRV) >> RGB2YUV_SHIFT);

			b = GETU8SCALE(B);
			g = GETU8SCALE(G);
			r = GETU8SCALE(R);

			*(pCur1Temp ) = b;
			*(pCur1Temp + 1) = g;
			*(pCur1Temp + 2) = r;			

			B2 = ((Y2 + nUBU) >> RGB2YUV_SHIFT);
			G2 = ((Y2 + nVGV + nUGU) >> RGB2YUV_SHIFT);
			R2 = ((Y2 + nVRV) >> RGB2YUV_SHIFT);

			b2 = GETU8SCALE(B2);
			g2 = GETU8SCALE(G2);
			r2 = GETU8SCALE(R2);

			*(pCur1Temp2 ) = b2;
			*(pCur1Temp2 + 1) = g2;
			*(pCur1Temp2 + 2) = r2;			


			B3 = ((Y3 + nUBU) >> RGB2YUV_SHIFT);
			G3 = ((Y3 + nVGV + nUGU) >> RGB2YUV_SHIFT);
			R3 = ((Y3 + nVRV) >> RGB2YUV_SHIFT);

			b3 = GETU8SCALE(B3);
			g3 = GETU8SCALE(G3);
			r3 = GETU8SCALE(R3);

			*(pCur2Temp ) = b3;
			*(pCur2Temp + 1) = g3;
			*(pCur2Temp + 2) = r3;			

			B4 = ((Y4 + nUBU) >> RGB2YUV_SHIFT);
			G4 = ((Y4 + nVGV + nUGU) >> RGB2YUV_SHIFT);
			R4 = ((Y4 + nVRV) >> RGB2YUV_SHIFT);

			b4 = GETU8SCALE(B4);
			g4 = GETU8SCALE(G4);
			r4 = GETU8SCALE(R4);

			*(pCur2Temp2 ) = b4;
			*(pCur2Temp2 + 1) = g4;
			*(pCur2Temp2 + 2) = r4;

			pCur1Temp += 6;
			pCur1Temp2 += 6;
			pCur2Temp += 6;
			pCur2Temp2 += 6;
		}

		//.求第二行相应位置的RGB值
		pYCur1 += (iWidth<<1);
		pYCur2 += (iWidth<<1);
		pUCur  += iWidth; //nWidthDiv;
		pVCur  += iWidth; //nWidthDiv;
		pCur1  += (nWidthPerLine<<1);
		pCur2  += (nWidthPerLine<<1);
	}
}


//void rgbToHsv(byte r, byte g, byte b, double hsv[]) 
//{
//	double rd = (double) r/255;
//	double gd = (double) g/255;
//	double bd = (double) b/255;
//	double max = threeway_max(rd, gd, bd), min = threeway_min(rd, gd, bd);
//	double h, s, v = max;
//
//	double d = max - min;
//	s = max == 0 ? 0 : d / max;
//
//	if (max == min) { 
//		h = 0; // achromatic
//	} else {
//		if (max == rd) {
//			h = (gd - bd) / d + (gd < bd ? 6 : 0);
//		} else if (max == gd) {
//			h = (bd - rd) / d + 2;
//		} else if (max == bd) {
//			h = (rd - gd) / d + 4;
//		}
//		h /= 6;
//	}
//
//	hsv[0] = h;
//	hsv[1] = s;
//	hsv[2] = v;
//}



void RGBtoHSV(  OSAL_UCHAR b,OSAL_UCHAR g,OSAL_UCHAR r,  OSAL_PUCHAR h, OSAL_PUCHAR s, OSAL_PUCHAR v )
{
    double min, max;
    double delta;

	double rd = (double) r/255;
	double gd = (double) g/255;
	double bd = (double) b/255;

    min = MIN( rd, gd, bd );
    max = MAX( rd, gd, bd );
	double H, S, V = max;
    delta = max - min;

	S = (max == 0 ? 0 : delta / max);

    if( max == min )
    {
        H = 0; 
    }
	else
	{
		if (max == rd) 
		{
			H = (gd - bd) / delta + (gd < bd ? 6 : 0);
		} 
		else if (max == gd) 
		{
			H = (bd - rd) / delta + 2;
		}
		else if (max == bd) 
		{
			H = (rd - gd) / delta + 4;
		}
		H /= 6;
	}

	*h = (char)(180.0*H);
	*s = (char)(S*255.0);
	*v = (char)(V*255.0);
}

void BI_BGRtoHSV(OSAL_PUCHAR pDstImg, const OSAL_PUCHAR pSrcImg, OSAL_INT32 iWidth, OSAL_INT32 iHeight)
{
	if( pSrcImg == OSAL_NULL || pDstImg == OSAL_NULL)
	{
		return;
	}

	int x = 0,y = 0;
	for(y = 0 ; y < iHeight; y++)
	{
		OSAL_PUCHAR ptr = pSrcImg + 3*y*iWidth;
		OSAL_PUCHAR ptr1= pDstImg + 3*y*iWidth;
		for (x = 0; x < iWidth; x++)
		{
			RGBtoHSV(ptr[3*x],ptr[3*x + 1], ptr[3*x + 2],&ptr1[3*x],&ptr1[3*x + 1], &ptr1[3*x + 2]);
		}
	}
}

void Vc_getBgrRectData(OSAL_PUCHAR pDstImg,  OSAL_PUCHAR pSrcImg, OSAL_INT32 iWidth, OSAL_INT32 iHeight,VRECT* Rect)
{
	if( pSrcImg == OSAL_NULL )
	{
		return;
	}

	int  i,j;
	int h = Vc_h;
	int w = Vc_w; 
	for(i = 0; i < h; i++)
	{
		OSAL_PUCHAR ptr  = pSrcImg + 3*((Rect->top + i)*iWidth + Rect->left);
		OSAL_PUCHAR ptr1 = pDstImg + 3*i*w ;
		for (j = 0; j < w;j++)
		{
			ptr1[3*j] = ptr[3*j];
			ptr1[3*j +1] = ptr[3*j + 1];
			ptr1[3*j +2] = ptr[3*j + 2];
		}
 	 //	memcpy(pDstImg + i*3*((w+3)/4)*4, pSrcImg + 3*((Rect->top + i)*iWidth + Rect->left), 3*w*sizeof(OSAL_PUCHAR) );
	}

	return;
}

void calAverageData(int*Aver,OSAL_PUCHAR hsvData,OSAL_INT32 iWidth, OSAL_INT32 iHeight)
{
	if (hsvData==OSAL_NULL)
	{
		return;
	}

	int x = 0,y = 0;
	for(y = 0 ; y < iHeight; y++)
	{
		OSAL_PUCHAR ptr = hsvData + 3*y*iWidth;
		for (x = 0; x < iWidth; x++)
		{
			Aver[0]+= ptr[3*x];
			Aver[1]+= ptr[3*x + 1];
			Aver[2]+= ptr[3*x + 2];
		}
	}
	Aver[0]/= 200;
	Aver[1]/= 200;
	Aver[2]/= 200;
}

int color_recognize(int* m)
{
	int h = (int)m[0];
	int s = (int)m[1];
	int v = (int)m[2];

	if ((s < 30)||(v < 46))
	{
		if (v < 46)
		{
			return V_Black;
		}

		if ((s<30)&&((v>221)&&(v<255)))
		{
			return V_White;
		}

		if ((s<43)&&((v>46)&&(v<220)))
		{
			return V_Gray;
		}
	}

	if ((s > 30)&&(v > 46))
	{
		if ((h <= 10)||(h>=156))
		{
			return V_Red;
		}

		if ((h <= 25)&&(h>=11))
		{
			return V_Orange;
		}

		if ((h <= 34)&&(h>=26))
		{
			return V_Yellow;
		}

		if ((h <= 77)&&(h>=35))
		{
			return V_Green;
		}

		if ((h <= 99)&&(h>=78))
		{
			return V_Indigo;
		}

		if ((h <= 124)&&(h>=100))
		{
			if (s<=100)
			{
				return V_Sliver;
			}
			return V_Blue;
		}

		if ((h <= 155)&&(h>=125))
		{
			return V_Purple;
		}
	}

	return -1;
}

SDKErrCode  vehicle_color_detect(OSAL_HANDLE handle,OSAL_PUCHAR pImage,OSAL_INT32 iW,OSAL_INT32 iH,ImageDataType eType,int* ppELAResult )
{
	VRECT RightRect = {0};
	VRECT MidRect = {0};
	VRECT LeftRect = {0};

	VRECT *pROIRect = OSAL_NULL;

	OSAL_PUCHAR bgrData = (OSAL_PUCHAR)malloc(3*Vc_w*Vc_h*sizeof(OSAL_UCHAR));
	OSAL_PUCHAR hsvData = (OSAL_PUCHAR)malloc(3*Vc_w*Vc_h*sizeof(OSAL_UCHAR));


	int Aver_l[3] = {0};
	int Aver_m[3] = {0};
	int Aver_r[3] = {0};

	int color[3] = {0};

	if(handle == OSAL_NULL)
	{
	 	return SDK_ERR_NONE;
	}
	else
	{
		//ROIRect = *pROIRect;
		//感兴趣区域必须四字节对齐
		pROIRect = (VRECT*)handle;
		if (((pROIRect->right - pROIRect->left) < 40)||((pROIRect->bottom - pROIRect->top) < 20))
		{
			return SDK_ERR_NONE;
		}

		LeftRect.bottom = pROIRect->top + 20;
		LeftRect.top    = pROIRect->top + 10;
		LeftRect.left   = pROIRect->left + 20;
		LeftRect.right  = pROIRect->left + 40;

		MidRect.bottom = pROIRect->top + 20;
		MidRect.top    = pROIRect->top + 10;
		MidRect.left   = ((pROIRect->left + pROIRect->right)>>1) -10 ;
		MidRect.right  = ((pROIRect->left + pROIRect->right)>>1) +10 ;

		RightRect.bottom = pROIRect->top + 20;
		RightRect.top	 = pROIRect->top + 10;
		RightRect.left   = pROIRect->right - 40;
		RightRect.right  = pROIRect->right - 20;
	}

	switch (eType)
	{
	 case  IMAGE_TYPE_BGR:
		 
		   memset(bgrData,0,3*20*10*sizeof(OSAL_UCHAR));
		   memset(hsvData,0,3*20*10*sizeof(OSAL_UCHAR));

		   Vc_getBgrRectData(bgrData,pImage,iW,iH,&LeftRect);//left
		   BI_BGRtoHSV(hsvData, bgrData, 20,10);
		   calAverageData(Aver_l,hsvData,20,10);
		   color[0] = color_recognize(Aver_l);

		   memset(bgrData,0,20*10*sizeof(OSAL_UCHAR));
		   memset(hsvData,0,20*10*sizeof(OSAL_UCHAR));
		   Vc_getBgrRectData(bgrData,pImage,iW,iH,&MidRect);//mid
		   BI_BGRtoHSV(hsvData, bgrData, 20,10);
		   calAverageData(Aver_m,hsvData,20,10);
		   color[1] = color_recognize(Aver_m);

		   memset(bgrData,0,20*10*sizeof(OSAL_UCHAR));
		   memset(hsvData,0,20*10*sizeof(OSAL_UCHAR));
		   Vc_getBgrRectData(bgrData,pImage,iW,iH,&RightRect);//mid
		   BI_BGRtoHSV(hsvData, bgrData, 20,10);
		   calAverageData(Aver_r,hsvData,20,10);
		   color[2] = color_recognize(Aver_r);

		   break;

     case  IMAGE_TYPE_YUV:
		 /************************************************************************/
		 /*                                     left                             */
		 /************************************************************************/
		 memset(bgrData,0,Vc_w*Vc_h*sizeof(OSAL_UCHAR));
		 memset(hsvData,0,Vc_w*Vc_h*sizeof(OSAL_UCHAR));

		 Vc_YUV420SP2BGR(bgrData,Vc_w,Vc_h,pImage, iW, iH, &LeftRect);//left
		 BI_BGRtoHSV(hsvData, bgrData, Vc_w,Vc_h);
		 calAverageData(Aver_l,hsvData,Vc_w,Vc_h);
		 color[0] = color_recognize(Aver_l);

		 /************************************************************************/
		 /*                                     Mid                            */
		 /************************************************************************/
		 memset(bgrData,0,Vc_w*Vc_h*sizeof(OSAL_UCHAR));
		 memset(hsvData,0,Vc_w*Vc_h*sizeof(OSAL_UCHAR));

		 Vc_YUV420SP2BGR(bgrData,Vc_w,Vc_h,pImage, iW, iH, &MidRect);//mid
		 BI_BGRtoHSV(hsvData, bgrData, Vc_w,Vc_h);
		 calAverageData(Aver_m,hsvData,Vc_w,Vc_h);
		 color[1] = color_recognize(Aver_m);

		 /************************************************************************/
		 /*                                     Right                         */
		 /************************************************************************/
		 memset(bgrData,0,Vc_w*Vc_h*sizeof(OSAL_UCHAR));
		 memset(hsvData,0,Vc_w*Vc_h*sizeof(OSAL_UCHAR));

		 Vc_YUV420SP2BGR(bgrData,Vc_w,Vc_h,pImage, iW, iH, &RightRect);//Right
		 BI_BGRtoHSV(hsvData, bgrData, Vc_w,Vc_h);
		 calAverageData(Aver_r,hsvData,Vc_w,Vc_h);
		 color[2] = color_recognize(Aver_r);

		 break;
	}

	free(bgrData);
	free(hsvData);

	if (color[0]==color[1])
	{
		* ppELAResult = color[1];
		return SDK_ERR_NONE;
	}

	if (color[1]==color[2])
	{
		* ppELAResult =  color[2];
		return SDK_ERR_NONE;
	}

	if (color[0]==color[2])
	{
		* ppELAResult =  color[2];
		return SDK_ERR_NONE;
	}

	else
	{
		* ppELAResult =  color[1];
        return SDK_ERR_NONE;
	}
		

	return SDK_ERR_NONE;

}