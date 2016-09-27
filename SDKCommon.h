#ifndef  SDK_COMMON_H
#define  SDK_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*定义输入图像数据类型，支持BGR、YUV*/
typedef enum eImageDataType
{
	IMAGE_TYPE_BGR                         =  0,  /*BGR*/
	IMAGE_TYPE_YUV                         =  1,  /*YUV*/
	IMAGE_TYPE_OTHER                       = -1   /*未知格式*/
} ImageDataType;

/*定义的错误参数，调用者可以根据返回码知道返回执行错误原因*/
typedef enum
{
	SDK_ERR_NONE                      =  0,
	SDK_ERR_NOINIT                    = -1,  /*未初始化*/
	SDK_ERR_PARAM                     = -2,  /*输入参数错误*/
	SDK_ERR_FULLTASK                  = -3,  /*任务已满*/
	SDK_ERR_ALLOC                     = -4,  /*内存分配失败*/
	SDK_ERR_NOBUFF                    = -5,  /*内部图像缓存不足*/
	SDK_ERR_LICENSE                   = -6,  /*license错误*/
	SDK_ERR_OTHER                     = -999
} SDKErrCode;



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SDK_COMMON_H */