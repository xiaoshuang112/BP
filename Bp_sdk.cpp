#include "stdafx.h"
#include "malloc.h"
#include "string.h"
#include "math.h"
#include "Array_bp.h"
#include "Bp_sdk.h"
#include <intrin.h>

#pragma intrinsic(exp)

float BPcalcActivationFunction(float x)
{
	return 1.0 / (1.0 + exp(-x)); //formula[4] formula[5] formula[7]
}


void GetBpPara(Bp*para,int flag)//flag 为了区分调用的数组类别；//flag:0,小车，对应_S;flag:1,大车，对应_B
{
//	Arrayinit(para->weight1, para->weight2,para->threshold1,para->threshold2, flag);

	FILE* fp = NULL;
	fopen_s(&fp, "bp.model", "rb");
	if (fp == NULL) {
		return  ;
	}

	int num_node_input, num_node_hidden, num_node_output;

	fread(&num_node_input, sizeof(int), 1, fp);
	fread(&num_node_hidden, sizeof(int), 1, fp);
	fread(&num_node_output, sizeof(int), 1, fp);
 
	fread(para->weight1, sizeof(float)*num_node_input*num_node_hidden, 1, fp);
	fread(para->threshold1, sizeof(float)*num_node_hidden, 1, fp);
	fread(para->weight2, sizeof(float)*num_node_hidden*num_node_output, 1, fp);
	fread(para->threshold2, sizeof(float)*num_node_output, 1, fp);

	fflush(fp);
	fclose(fp);
}


Bp Bpinit(int node_input,int node_hiden,int node_output,int flag)
{
	Bp bp1;
	bp1.Node_hiden = node_hiden;
	bp1.Node_input = node_input;
	bp1.Node_output = node_output;

	bp1.weight1 = (float*)malloc(sizeof(float)*node_input*node_hiden);
	memset(bp1.weight1,0,sizeof(float)*node_input*node_hiden);

	bp1.weight2 = (float*)malloc(sizeof(float)*node_output*node_hiden);
	memset(bp1.weight2,0,sizeof(float)*node_output*node_hiden);

	bp1.threshold1 = (float*)malloc(sizeof(float)*node_hiden);
	memset(bp1.threshold1,0,sizeof(float)*node_hiden);

	bp1.threshold2 = (float*)malloc(sizeof(float)*node_output);
	memset(bp1.threshold2,0,sizeof(float)*node_output);

	bp1.output_hiddenLayer = (float*)malloc(sizeof(float)*node_hiden);
	memset(bp1.output_hiddenLayer,0,sizeof(float)*node_hiden);

	bp1.output_outputLayer = (float*)malloc(sizeof(float)*node_output);
	memset(bp1.output_outputLayer,0,sizeof(float)*node_output);

	GetBpPara(&bp1,flag);

	return bp1;
}


void BPcalcHiddenLayer(Bp bp, const int* data)
{

	for (int i = 0; i < bp.Node_hiden; i++) {
		float tmp = 0;
		for (int j = 0; j < bp.Node_input; j++) {
			tmp += data[j] * bp.weight1[j*bp.Node_hiden+i];
		}

		tmp -= bp.threshold1[i]; //formula[3]
		bp.output_hiddenLayer[i] = BPcalcActivationFunction(tmp);
	}
}

void BPcalcOutputLayer(Bp bp)
{

	for (int i = 0; i < bp.Node_output; i++) 
	{
		float tmp = 0;

		for (int j = 0; j < bp.Node_hiden; j++) 
		{
		 	tmp += bp.output_hiddenLayer[j] * bp.weight2[j*bp.Node_output+i];
		}

		tmp -= bp.threshold2[i]; //formula[6]
		bp.output_outputLayer[i] = BPcalcActivationFunction(tmp);
	}
}


int BPpredict(Bp bp, int* data, int width, int height)
{

	const int* p = data;
	BPcalcHiddenLayer(bp, p);
	BPcalcOutputLayer(bp);

	float max_value = -9999;
	int ret = -1;

	for (int i = 0; i < bp.Node_output; i++) 
	{
		if (bp.output_outputLayer[i] > max_value) 
		{
			max_value = bp.output_outputLayer[i];
			ret = i;
		}
	}

	return ret;
}

void Bprelease(Bp*bp)
{
	if(bp->weight1!=NULL)
	{
		free(bp->weight1);
		bp->weight1=NULL;
	}

	if(bp->weight2!=NULL)
	{
		free(bp->weight2);
		bp->weight2=NULL;
	}

	if(bp->threshold1!=NULL)
	{
		free(bp->threshold1);
		bp->threshold1=NULL;
	}

	if(bp->threshold2!=NULL)
	{
		free(bp->threshold2);
		bp->threshold2=NULL;
	}

	if (bp->output_hiddenLayer!=NULL)
	{
		free(bp->output_hiddenLayer);
		bp->output_hiddenLayer=NULL;
	}

	if (bp->output_hiddenLayer!=NULL)
	{
		free(bp->output_outputLayer);
		bp->output_outputLayer=NULL;
	}
}
