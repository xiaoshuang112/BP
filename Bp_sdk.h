#ifndef BPSDK_H
#define BPSDK_H


typedef struct TagBp
{
	float* weight1/*[num_node_input_BP][num_node_hidden_BP]*/; //�����������������Ȩֵ
	float* weight2/*[num_node_hidden_BP][num_node_output_BP]*/; //�����������������Ȩֵ

	float* threshold1/*        [num_node_hidden_BP]*/; //��������ֵ
	float* threshold2/*        [num_node_output_BP]*/; //�������ֵ
	float* output_hiddenLayer/*[num_node_hidden_BP]*/; //˳���������������ֵ
	float* output_outputLayer/*[num_node_output_BP]*/; //˳��������������ֵ

	int Node_input;
	int Node_hiden;
	int Node_output;
}Bp;

bool Bpinit(Bp*bp1,int node_input,int node_hiden,int node_output,int flag);

int   BPpredict(Bp* bp, int* data);

void Bprelease(Bp*bp);

#endif
