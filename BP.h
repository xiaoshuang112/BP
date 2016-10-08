#ifndef _BP_HPP_
#define _BP_HPP_

#include <windows.h>
namespace ANN {

#define num_node_input_BP	1764//756//3780//784 //输入层节点数
#define width_image_BP		64//64//28 //归一化图像宽
#define height_image_BP		64//32//28 //归一化图像高
#define num_node_hidden_BP	120//160//120 //隐含层节点数
#define num_node_output_BP	4 //输出层节点数
#define alpha_learning_BP	0.8//0.8 //输出层至隐含层学习率
#define beta_learning_BP	0.6//0.6 //隐含层至输入层学习率
#define patterns_train_BP	60000//60000 //训练模式对数(总数)
#define patterns_test_BP	10000//10000 //测试模式对数(总数)
#define iterations_BP		200   //10000 //最大训练次数
#define accuracy_rate_BP	0.95 //要求达到的准确率

	class BP {
	public:
		BP();
		~BP();

		void init(const char* initrain,const char* initest); //初始化，分配空间
		bool train(); //训练
		int predict(const int* data, int width, int height); //预测
		bool readModelFile(const char* name); //读取已训练好的BP model
		int VL_GetHOGFeature(int *pFeature, unsigned char* pSrc,int iW,int iH); //获取HOG特征

	protected:
		void release(); //释放申请的空间
		bool saveModelFile(const char* name); //将训练好的model保存起来，包括各层的节点数，权值和阈值
		bool initWeightThreshold(); //初始化，产生[-1, 1]之间的随机小数
		int  ReadFile(char *path, int* data_dst,int* Label_dst); 
		bool getSrcData(const char* initrain,const char* initest); //读取JPG或BMP图片
		bool getSrcData(); //读取MNIST数据
		void calcHiddenLayer(const int* data); //计算隐含层输出
		void calcOutputLayer(); //计算输出层输出
		void calcAdjuctOutputLayer(const int* data); //计算输出层校正误差
		void calcAdjuctHiddenLayer(); //计算隐含层校正误差

		float calcActivationFunction(float x); //计算激活函数，对数S形函数
	//	float calcActivationFunction_outlayer(float x); //计算输出激活函数

		void updateWeightThresholdOutputLayer(); //更新输出层至隐含层权值和阈值
		void updateWeightThresholdHiddenLayer(const int* data); //更新隐含层至输入层权值和阈值
		float test(); //训练完一次计算一次准确率

	private:
		float* weight1/*[num_node_input_BP][num_node_hidden_BP]*/; //输入层至隐含层连接权值
		float* weight2/*[num_node_hidden_BP][num_node_output_BP]*/; //隐含层至输出层连接权值
		float* threshold1/*[num_node_hidden_BP]*/; //隐含层阈值
		float* threshold2/*[num_node_output_BP]*/; //输出层阈值
		float* output_hiddenLayer/*[num_node_hidden_BP]*/; //顺传播，隐含层输出值
		float* output_outputLayer/*[num_node_output_BP]*/; //顺传播，输出层输出值
		float* adjust_error_outputLayer/*[num_node_output_BP]*/; //逆传播，输出层校正误差
		float* adjust_error_hiddenLayer/*[num_node_hidden_BP]*/; //逆传播，隐含层校正误差

		int* data_input_train; //原始标准输入数据，训练
		int* data_output_train; //原始标准期望结果，训练
		int* data_input_test; //原始标准输入数据，测试
		int* data_output_test; //原始标准期望结果，测试
		char m_traindatainipath[MAX_PATH];   //训练集集数据记录文件ini路径
		char m_testdatainipath[MAX_PATH];    //测试集数据记录文件ini路径

		int  m_trainNum;  //实际训练样本数
		int  m_testNum;   //实际测试样本数
	};

}

#endif //_BP_HPP_