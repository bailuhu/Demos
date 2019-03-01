
#ifndef GLOBAL_FUNC_H
#define GLOBAL_FUNC_H

#include <Windows.h>	// for GetModuleFileName
#include <atlstr.h>

// C:\\Users\\Administrator\\Documents\\Visual Studio 2010\\Projects\\DemoOpenCV\\resource\\;

char* CStringToChar(CString src);
CString GetExeDir();
CString GetResDir();

#define LENA "D:\\Res\\lena.jpg"
#define JPG2 "D:\\Res\\2.jpg"
#define JPG3 "D:\\Res\\3.jpg"
#define JPG4 "D:\\Res\\4.jpg"
#define JPG5 "D:\\Res\\5.jpg"


#define JPG_boldt "D:\\Res\\boldt.jpg"
#define JPG_rain "D:\\Res\\rain.jpg"
#define BMP_logo "D:\\Res\\logo.bmp"


#define PrintImgInfo(img)\
{\
	printf("< sizeof(IplImage): %d\n", img->nSize);\
	printf("< version (=0): %d\n", img->ID);\
	printf("< channels: %d\n", img->nChannels);\
	printf("< alphaChannel: %d\n", img->alphaChannel);\
	printf("< Pixel depth in bits: %d\n", img->depth);\
	printf("< colorModel: %d\n", img->colorModel);\
	printf("< channelSeq[4]: %s\n", img->channelSeq);\
	printf("< dataOrder: %d\n", img->dataOrder);\
	printf("< origin: %d\n", img->origin);\
	printf("< align: %d\n", img->align);\
	printf("< width x height: %dx%d\n", img->width, img->height);\
	if (img->roi != nullptr)\
	{\
		printf("< roi.coi %d\n", img->roi->coi);\
		printf("< roi.width x roi.height: %dx%d\n", img->roi->width, img->roi->height);\
		printf("< roi.xOffset: %d\n", img->roi->xOffset);\
		printf("< roi.xOffset: %d\n", img->roi->yOffset);\
	}\
	printf("< imageSize: %d\n", img->imageSize);\
	printf("< widthStep: %d\n", img->widthStep);\
	printf("< BorderMode[4]: %s\n", img->BorderMode);\
	printf("< BorderConst[4]: %s\n", img->BorderConst);\
}

#endif