/************************************************************************
* Copyright(C), 2018-2028, Huawei Tech. Co., Ltd. 
* All rights reserved.  
* $Id: hwdepthcalibapi.h, v1.0 2018/02/26 10:00:00 liuwancheng Exp $    
* Description��  DFɢ�߲��Խӿ��ļ�
* Reference Doc��  
* Modification�� 
* History�� 

v1.0 2014/02/26 - ��ɳ��岢����
*************************************************************************/

#include<vector>
#define MAX_NUM_OF_SPOT (5120)
#define Test_DISTANCE_NUM (3)


//////////////////////////////////////////////////////////////

// base types
typedef struct spotCalibDiameterData
{
	int a;
}spotCalibDiameterData;

typedef struct spotCalibPowerData
{
	int a;
}spotCalibPowerData;

typedef struct FOIAngle
{
	double FOIAngleX;
	double FOIAngleY;
}FOIAngle;

typedef struct dfSpotInputParams
{
    char* pInputImagePath;
    spotCalibDiameterData spotCalibDiameter;
    spotCalibPowerData spotCalibPower;
	char* configFilePath;
}dfSpotInputParams;

typedef struct dfSpotOutputParams
{
	std::vector <float> spotContrastQueue;
	std::vector <float> spotDiameterQueue;
	std::vector <pair<double, cv::Point>> spotPowerQueue;
    FOIAngle FOIAngleOut;
    double patternUniformityRatio;
    double diffractionOrderErrorRatio;
    double patternRotationAngle;
    double signleOrderRotationAngle;
    double zeroOrderUniRatio;
    int numDeadSpot;
    int numDeadSpotCluster;
    int numBlinking;
}dfSpotOutputParams;

typedef struct dfSpotThreshold
{
	int testDistance;
	double testTemperature;
	double minContrast;
	double maxContrast;
	double minDiameter;
	double maxDiameter;
	double minPower;
	double maxPower;

	double minFOIAngleX;
	double maxFOIAngleX;
	double minFOIAngleY;
	double maxFOIAngleY;

	double minPatternUniformity;
	double maxPatternUniformity;
	double minOrderErrPower;
	double maxOrderErrPower;
	double minPatternRotation;
	double maxPatternRotation;

	double minSignleOrderRotation;
	double maxignleOrderRotation;
	double minZeroOrderUni;
	double maxZeroOrderUni;

	double minDeadSpot;
	double maxDeadSpot;
	double minDeadSpotCluster;
	double maxDeadSpotCluster;
	double minBlinking;
	double maxBlinking;

}dfSpotThreshold;

typedef struct dfPoint
{
	int x;
	int y;
}dfPoint;

typedef struct dfSpotCoordinateConfig
{
	int testDistance;
	int windowSize;
	int searchZone;
	dfPoint foiCorner[4];//0-leftup 1-rightup 2-leftdown 3-rightdown 
	dfPoint spotTestCorner[4];//0-leftup 1-rightup 2-leftdown 3-rightdown 
	dfPoint diffractionOrderCorner[4];//0-leftup 1-rightup 2-leftdown 3-rightdown 
	dfPoint blinkingCorner[4];//0-leftup 1-rightup 2-leftdown 3-rightdown 
	dfPoint patternRotationCorner[4];//0-leftup 1-rightup 2-leftdown 3-rightdown 
	dfPoint singleOrderCorner[4];//0-leftup 1-rightup 2-leftdown 3-rightdown 
	dfPoint zeroOrderCorner[4];//0-leftup 1-rightup 2-leftdown 3-rightdown 
}dfSpotCoordinateConfig;

typedef struct dfSpotXmlConfig
{
	int factoryCode;
	int vcselSupplier;
	int testDistanceNum;
	char* testImagePath;
	char* referenceImagePath;
	char* programPath;
	float pixelSizeRatio[Test_DISTANCE_NUM];
	dfSpotCoordinateConfig spotCorner[Test_DISTANCE_NUM];
}dfSpotConfig;



/******************************************************************************
* ����:  dfSpotTestCreate
* ����:    
*   ɢ�߲��Բ�����ʼ������ 
* ����:
*   dfSpotInputParams		- [in]�������
*   OutputParams            - [in]�������
* ����: 
*   retFlag
*******************************************************************************/
int  dfSpotTestCreate(	
	dfSpotInputParams         *inputParams,
	dfSpotOutputParams        *OutputParams
	); 

/******************************************************************************
* ����:  dfSpotTestProcess
* ����:    
*   ɢ�߲��Դ����� 
* ����:
*   dfSpotInputParams		- [in]�������
*   OutputParams            - [out]�������
* ����: 
*   retFlag
*******************************************************************************/
int  dfSpotTestProcess(
	dfSpotInputParams         *inputParams,
	dfSpotOutputParams        *OutputParams
    ); 

/******************************************************************************
* ����:  dfSpotTestDestroy
* ����:    
*   ɢ�߲����������� 
* ����:
*   dfSpotInputParams		- [in]�������
*   OutputParams            - [in]�������
* ����: 
*   retFlag
*******************************************************************************/
int  dfSpotTestDestroy(
	dfSpotInputParams         *inputParams,
	dfSpotOutputParams        *OutputParams
    );

/********************************************************************************/	




