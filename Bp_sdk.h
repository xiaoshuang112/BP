#ifndef BPSDK_H
#define BPSDK_H


typedef struct TagBp
{
	float* weight1/*[num_node_input_BP][num_node_hidden_BP]*/; //输入层至隐含层连接权值
	float* weight2/*[num_node_hidden_BP][num_node_output_BP]*/; //隐含层至输出层连接权值

	float* threshold1/*        [num_node_hidden_BP]*/; //隐含层阈值
	float* threshold2/*        [num_node_output_BP]*/; //输出层阈值
	float* output_hiddenLayer/*[num_node_hidden_BP]*/; //顺传播，隐含层输出值
	float* output_outputLayer/*[num_node_output_BP]*/; //顺传播，输出层输出值

	int Node_input;
	int Node_hiden;
	int Node_output;
}Bp;

bool Bpinit(Bp*bp1,int node_input,int node_hiden,int node_output,int flag);

int   BPpredict(Bp* bp, int* data);

void Bprelease(Bp*bp);

#endif
