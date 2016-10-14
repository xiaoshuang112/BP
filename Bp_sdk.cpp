
#include "stdafx.h"
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include "string.h"
#include "math.h"
#include "Array_bp.h"
#include "Bp_sdk.h"

float BPcalcActivationFunction(float x)
{
	float A = 1.0 / (1.0 + exp((-1)*x)); //formula[4] formula[5] formula[7]
	return A;
}

void GetBpPara(Bp*para,int flag)//flag Ϊ�����ֵ��õ��������//flag:0,С������Ӧ_S;flag:1,�󳵣���Ӧ_B
{
	Arrayinit(&para->weight1, &para->weight2,&para->threshold1,&para->threshold2, flag);
}


bool Bpinit(Bp*bp1,int node_input,int node_hiden,int node_output,int flag)
{
	if ((node_hiden!=NODE_HIDE)||(node_input!=NODE_INPUT)||(node_output!=NODE_OUTPUT))
	{
		return false;
	}

	bp1->Node_hiden  = node_hiden;
	bp1->Node_input  = node_input;
	bp1->Node_output = node_output;

	/*bp1->weight1 = (float*)malloc(sizeof(float)*node_input*node_hiden);
	memset(bp1->weight1,0,sizeof(float)*node_input*node_hiden);

	bp1->weight2 = (float*)malloc(sizeof(float)*node_output*node_hiden);
	memset(bp1->weight2,0,sizeof(float)*node_output*node_hiden);

	bp1->threshold1 = (float*)malloc(sizeof(float)*node_hiden);
	memset(bp1->threshold1,0,sizeof(float)*node_hiden);

	bp1->threshold2 = (float*)malloc(sizeof(float)*node_output);
	memset(bp1->threshold2,0,sizeof(float)*node_output);*/

	bp1->output_hiddenLayer = (float*)malloc(sizeof(float)*node_hiden);
	memset(bp1->output_hiddenLayer,0,sizeof(float)*node_hiden);

	bp1->output_outputLayer = (float*)malloc(sizeof(float)*node_output);
	memset(bp1->output_outputLayer,0,sizeof(float)*node_output);

 	GetBpPara(bp1,flag);

 	return true;

}


void BPcalcHiddenLayer(Bp* bp, const int* data)
{
	int i = 0;
	int j = 0;


	for ( i = 0; i < bp->Node_hiden; i++) {
		float Temp = 0;
		for (j = 0; j < bp->Node_input; j++) {
			Temp += data[j] * bp->weight1[j*bp->Node_hiden+i];
		}

		Temp -= bp->threshold1[i]; //formula[3]
		bp->output_hiddenLayer[i] = BPcalcActivationFunction(Temp);
	}
}

void BPcalcOutputLayer(Bp* bp)
{
	int i;
	int j  ;
	for (i = 0; i < bp->Node_output; i++) {
		float tmp = 0.0;
		for (j = 0; j < bp->Node_hiden; j++) {
			tmp += bp->output_hiddenLayer[j] * bp->weight2[j*bp->Node_output+i];
		}

		tmp -= bp->threshold2[i]; //formula[6]
		bp->output_outputLayer[i] = BPcalcActivationFunction(tmp);
	}
}


int BPpredict(Bp* bp, int* data)
{

	const int* p = data;
	BPcalcHiddenLayer(bp, p);
	BPcalcOutputLayer(bp);

	float max_value = -9999.0;
	int ret = -1;

	int i;
	for (  i = 0; i < bp->Node_output; i++)
	{
		if (bp->output_outputLayer[i] > max_value)
		{
			max_value = bp->output_outputLayer[i];
			ret = i;
		}
	}

	return ret;
}

void Bprelease(Bp*bp)
{
	//if(bp->weight1!=NULL)
	//{
	//	free(bp->weight1);
	//	bp->weight1=NULL;
	//}

	//if(bp->weight2!=NULL)
	//{
	//	free(bp->weight2);
	//	bp->weight2=NULL;
	//}

	//if(bp->threshold1!=NULL)
	//{
	//	free(bp->threshold1);
	//	bp->threshold1=NULL;
	//}

	//if(bp->threshold2!=NULL)
	//{
	//	free(bp->threshold2);
	//	bp->threshold2=NULL;
	//}

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
