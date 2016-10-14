// BP_C.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "BP.h"
#include "Bp_sdk.h"
#include "VehicleTypeDetect.h"
#include "Vehiclecolor.h"

#include "opencv2/opencv.hpp"
#include "VLMain.h"

using namespace cv;
using namespace std;



 

int test_BP();

int _tmain(int argc, _TCHAR* argv[])
{
	test_BP();
	std::cout << "ok!" << std::endl;
}



int test_BP()
{
	//1. bp train
	//char TrainPath[MAX_PATH]="E:/1-work/work-space/BP/训练集/vallabel.txt";
	//char TestPath[MAX_PATH]="E:/1-work/work-space/BP/测试集/vallabel.txt";

	//ANN::BP bp1;
	//bp1.init(TrainPath,TestPath);
	//bp1.train();
	// 
	//return 1;

	////////////////////////////////////////////////////////////////////////
	std::string path_images = "E:/1-work/work-space/BP/testData/";
	Bp *bp_s = NULL;
	//Bpinit(&bp_s,756,160,3,0);
	OSAL_HANDLE handle = (OSAL_HANDLE)bp_s;
	VehicleType_TaskCreate(&handle);
		int data_image[756] = {0};
	int nMeasure[2048],nTheta[2048],nHist[2048];

	for (int i = 0; i < 6; i++) 
	{
		char ch[15];
		sprintf_s(ch, "%d", i);
		std::string str;
		str = std::string(ch);
		str += ".jpg";
		str = path_images + str;

		cv::Mat mat = cv::imread(str, 2 | 4);
		if (!mat.data) {
			std::cout << "read image error" << std::endl;
			return -1;
		}
		//////////////////////////////////////////////////////////////////////////


		VRECT rect ;
		rect.top = 0;rect.bottom = mat.rows;rect.left = 0; rect.right = mat.cols;
		int Ret = 0;

		
		long t0 = cv::getTickCount();
		vehicle_color_detect(OSAL_HANDLE(&rect),mat.data, mat.cols,mat.rows,IMAGE_TYPE_BGR,&Ret);
		long t1 = cv::getTickCount();
		double secs = (t1 - t0)/cv::getTickFrequency();

		//////////////////////////////////////////////////////////////////////////
		if (mat.channels() == 3) {
			cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
		}

		if (mat.cols != 64 || mat.rows != 32) 
		{
			cv::resize(mat, mat, cv::Size(64, 32));
		} 

		TYPEResult p ;
	 
		VehicleType_TaskProcess(handle,mat.data,mat.cols,mat.rows,IMAGE_TYPE_BGR,&p);

		//memset(data_image, 0, sizeof(int) * num_node_input_BP);
		//VL_GetImageHogFeature(data_image,mat.data,64,32,nMeasure,nTheta,nHist);



	 	std::cout << "correct result: " << i << ",    actual result: " <<(int)p<<" Time is :" <<secs*1000 << std::endl << "COLOR IS :"<<(int)Ret<< std::endl;
	}

 

	//////////////////////////////////////////////////////////////////////////



//	2. bp predict
	/*ANN::BP bp2;
	bp2.init(NULL,NULL);
	bool flag = bp2.readModelFile("bp.model");
	if (!flag) {
		std::cout << "read bp model error" << std::endl;
		return -1;
	}

	std::string path_images = "E:/1-work/work-space/BP/testData/";

	int data_image[num_node_input_BP],nMeasure[width_image_BP * height_image_BP],nTheta[width_image_BP * height_image_BP],nHist[width_image_BP * height_image_BP];

	for (int i = 0; i < 21; i++) {
		char ch[15];
		sprintf_s(ch, "%d", i);
		std::string str;
		str = std::string(ch);
		str += ".jpg";
		str = path_images + str;

		cv::Mat mat = cv::imread(str, 2 | 4);
		if (!mat.data) {
			std::cout << "read image error" << std::endl;
			return -1;
		}
		if (mat.channels() == 3) {
			cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
		}

		if (mat.cols != width_image_BP || mat.rows != height_image_BP) {
			cv::resize(mat, mat, cv::Size(width_image_BP, height_image_BP));
		}
		memset(data_image, 0, sizeof(int) * num_node_input_BP);
		VL_GetImageHogFeature(data_image,mat.data,width_image_BP,height_image_BP,nMeasure,nTheta,nHist);

		cv::Mat save(height_image_BP,width_image_BP,CV_8UC1);


		for (int h = 0; h < save.rows; h++) {
			uchar* p = save.ptr(h);
			for (int w = 0; w < mat.cols; w++) {
				p[w] = nMeasure[h*width_image_BP+w];
			}
		}
		cv::imwrite("Measure.bmp",save);


		for (int h = 0; h < save.rows; h++) {
			uchar* p = save.ptr(h);
			for (int w = 0; w < mat.cols; w++) {
				p[w] = nTheta[h*width_image_BP+w];
			}
		}
		cv::imwrite("nTheta.bmp",save);

		memset(save.data,0,width_image_BP*height_image_BP);
		for (int h = 0; h < 8; h++) {
			uchar* p = save.ptr(h);
			for (int w = 0; w < 8; w++) {
				p[w] = nHist[h*72+w];
			}
		}
		cv::imwrite("nHist.bmp",save);

		long t0 = cv::getTickCount();
        int ret = bp2.predict(data_image, mat.cols, mat.rows);
		long t1 = cv::getTickCount();
		double secs = (t1 - t0)/cv::getTickFrequency();
		
		std::cout << "correct result: " << i << ",    actual result: " << ret <<" Time is :" <<secs*1000<< std::endl;
	}*/



	getchar();
	return 0;
}

