// BP_C.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "BP.h"
#include "Bp_sdk.h"

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

 //  	return 1;

	//////////////////////////////////////////////////////////////////////////
	std::string path_images = "E:/1-work/work-space/BP/BP_C/testData/";
	Bp bp_s = Bpinit(1764,120,4,0);
	int data_image[1764] = {0};
	int nMeasure[4096],nTheta[4096],nHist[4096];

	for (int i = 0; i < 17; i++) 
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
		if (mat.channels() == 3) {
			cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
		}

		if (mat.cols != 64 || mat.rows != 64) 
		{
			cv::resize(mat, mat, cv::Size(64, 64));
		}
		VL_GetImageHogFeature(data_image,mat.data,64,64,nMeasure,nTheta,nHist);

		long t0 = cv::getTickCount();
		int ret = BPpredict(bp_s,data_image, mat.cols, mat.rows);
		long t1 = cv::getTickCount();
		double secs = (t1 - t0)/cv::getTickFrequency();
		std::cout << "correct result: " << i << ",    actual result: " << ret<<" Time is :" <<secs*1000<< std::endl;
	}


	//////////////////////////////////////////////////////////////////////////



	////2. bp predict
	//ANN::BP bp2;
	//bp2.init(NULL,NULL);
	//bool flag = bp2.readModelFile("bp-big.model");
	//if (!flag) {
	//	std::cout << "read bp model error" << std::endl;
	//	return -1;
	//}

	//int target[20] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19 };
	//std::string path_images = "E:/1-work/work-space/BP/BP_C/testData/";
	////int* data_image = new int[width_image_BP * height_image_BP];
	//int data_image[num_node_input_BP],nMeasure[width_image_BP * height_image_BP],nTheta[width_image_BP * height_image_BP],nHist[width_image_BP * height_image_BP];
	////int *nMeasure = new int[width_image_BP * height_image_BP];
	////int *nTheta = new int[width_image_BP * height_image_BP];
	////int *nHist = new int[width_image_BP * height_image_BP];

	//for (int i = 0; i < 10; i++) {
	//	char ch[15];
	//	sprintf_s(ch, "%d", i);
	//	std::string str;
	//	str = std::string(ch);
	//	str += ".jpg";
	//	str = path_images + str;

	//	cv::Mat mat = cv::imread(str, 2 | 4);
	//	if (!mat.data) {
	//		std::cout << "read image error" << std::endl;
	//		return -1;
	//	}
	//	if (mat.channels() == 3) {
	//		cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
	//	}

	//	if (mat.cols != width_image_BP || mat.rows != height_image_BP) {
	//		cv::resize(mat, mat, cv::Size(width_image_BP, height_image_BP));
	//	}
	//	memset(data_image, 0, sizeof(int) * num_node_input_BP);
	//	//bp2.VL_GetHOGFeature(data_image0,mat.data,mat.cols,mat.rows);
	//	VL_GetImageHogFeature(data_image,mat.data,width_image_BP,height_image_BP,nMeasure,nTheta,nHist);

	//	cv::Mat save(height_image_BP,width_image_BP,CV_8UC1);


	//	for (int h = 0; h < save.rows; h++) {
	//		uchar* p = save.ptr(h);
	//		for (int w = 0; w < mat.cols; w++) {
	//			p[w] = nMeasure[h*width_image_BP+w];
	//		}
	//	}
	//	cv::imwrite("Measure.bmp",save);


	//	for (int h = 0; h < save.rows; h++) {
	//		uchar* p = save.ptr(h);
	//		for (int w = 0; w < mat.cols; w++) {
	//			p[w] = nTheta[h*width_image_BP+w];
	//		}
	//	}
	//	cv::imwrite("nTheta.bmp",save);

	//	memset(save.data,0,width_image_BP*height_image_BP);
	//	for (int h = 0; h < 8; h++) {
	//		uchar* p = save.ptr(h);
	//		for (int w = 0; w < 8; w++) {
	//			p[w] = nHist[h*72+w];
	//		}
	//	}
	//	cv::imwrite("nHist.bmp",save);

	//	int ret = bp2.predict(data_image, mat.cols, mat.rows);
	//	std::cout << "correct result: " << i << ",    actual result: " << ret << std::endl;
	//}

	//delete[] data_image;
	//delete[] nMeasure;
	//delete[] nTheta;
	//delete[] nHist;
	getchar();
	return 0;
}

