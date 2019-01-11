#include <QtWidgets/QApplication>

#include "QDlgMain.h"
#include "GlobalFunction.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;
#include "Huawei/dfspottestapi.h"

cv::Point OC_Test_MV_2(QString filePathR, QString filePathW)
{
	Mat imgDst, imgGray,imgBlurHor,imgBlurVer,imgBinHor,imgBinVer, imgGrayHor, imgGrayVer;
	Mat imgSrc = imread(filePathR.toStdString());
	clock_t startTime, endTime;
	double costTime;
	startTime = clock();
	cvtColor(imgSrc,imgGray,COLOR_RGB2GRAY);
	int ImgSize[2] = { imgGray.rows, imgGray.cols };
	int startY = imgGray.rows / 3;
	int endY = imgGray.rows / 3*2;
	int startX = imgGray.cols / 3;
	int endX = imgGray.cols / 3 * 2;
	imgGrayHor = imgGray(Range(0, ImgSize[0]), Range(startX, endX));
	imgGrayVer = imgGray(Range(startY, endY), Range(0, ImgSize[1]));
	GaussianBlur(imgGrayHor, imgBlurHor, Size(51, 1), 25, 0);
	GaussianBlur(imgGrayVer, imgBlurVer, Size(1, 51), 0, 25);
	endTime = clock();
	threshold(imgBlurHor, imgBinHor , 0, 255, CV_THRESH_OTSU);
	threshold(imgBlurVer, imgBinVer, 0, 255, CV_THRESH_OTSU);
	double sumY = 0;
	double sumP = 0;// Sum of pixel gray
	double sumX = 0;
	double ocY = 0;
	double ocX = 0;
	//%��������
	for (int i = 0; i < ImgSize[0]; i++) 
	{
		for (int j=0; j < (endX - startX); j++)
		{
			sumY = (i)*imgBinHor.at<uchar>(i, j) + sumY;
			sumP = imgBinHor.at<uchar>(i, j) + sumP;
		}
	}
	ocY = (sumY / sumP);
	sumP = 0;
	for (int i = 0; i < (endY - startY); i++)
	{
		for (int j=0; j < ImgSize[1]; j++)
		{
			sumX = (j)*imgBinVer.at<uchar>(i, j) + sumX;
			sumP = imgBinVer.at<uchar>(i, j) + sumP;
		}

	}
	ocX = (sumX / sumP);

	costTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
	cout << "mask��������Ϊ��" << ocX << "     " << ocY << endl;
	cout << "����ʱ��Ϊ��" << costTime << endl;
	cv::Point centerPt;//�����㣬���Ի���ͼ����    
	centerPt.x = ocX;//��������ͼ���к�����    
	centerPt.y = ocY;//��������ͼ����������    
	Scalar clr = Scalar(0, 0, 255);
	circle(imgSrc, centerPt, 10, clr, -1);//��ͼ���л��������㣬2��Բ�İ뾶
	line(imgSrc, Point(0, centerPt.y), Point(imgSrc.cols, centerPt.y), clr);
	line(imgSrc, Point(centerPt.x, 0), Point(centerPt.x, imgSrc.rows), clr);
	imwrite(filePathW.toStdString(), imgSrc);
	return centerPt;
// 	namedWindow("imgSrc", 2);
// 	imshow("imgSrc", imgSrc);
// 	namedWindow("imgBinHor", 2);
// 	imshow("imgBinHor", imgBinHor);
// 	namedWindow("imgBinVer", 2);
// 	imshow("imgBinVer", imgBinVer);
// 	waitKey(0);
}



Mat Gaussian_kernal(int kernel_size, int sigma, int direction = 1)
{
	const double PI = 3.14159265358979323846;
	int m = kernel_size / 2;
	Mat kernel(kernel_size, kernel_size, CV_32FC1);
	float s = 2 * sigma*sigma;
	switch (direction)
	{
	case 1:
		for (int i = 0; i < kernel_size; i++)
		{
			for (int j = 0; j < kernel_size; j++)
			{
				int x = i - m, y=j - m;
				kernel.ptr<float>(i)[j] = exp(-(x*x + y*y) / s) / (PI*s);
			}
		}
		break;
	case 2:
		for (int w = 0; w < kernel_size; w++)
		{
			for (int h = 0; h < kernel_size; h++)
			{
				int x = w - m, y= w - m;
				kernel.ptr<float>(h)[w] = exp(-(x*x + y*y) / s) / (PI*s);
			}
		}
		break;
	default:
		break;
	}
	
	return kernel;
}

void Otsu2Threshold(Mat& src, Mat& dst)
{
	Mat img = src;
	//dst.create(src.size(), src.type());
	CV_Assert(1 == img.channels());

	int height = img.rows;
	int width  = img.cols;
	int Threshold1 = 0;//��ֵ
	int Threshold2 = 0;//��ֵ
	double gSum_object;//��һ��,ǰ���Ҷ���ֵ
	double gSum_middle;
	double gSum_blackground;//�ڶ��࣬�����Ҷ���ֵ

	double N_object = 0;//ǰ��������
	double N_middle = 0;
	double N_blackground = 0;//����������

	double u0 = 0;//ǰ������ƽ���Ҷ�
	double u1 = 0;
	double u2 = 0;//��������ƽ���Ҷ�
	double w0 = 0;//ǰ�����ص���ռ����ͼ��ı���Ϊ��0
	double w1 = 0;
	double w2 = 0;//�������ص���ռ����ͼ��ı���Ϊ��1
	double u = 0;//��ƽ���Ҷ�
	double tempg = -1;//��ʱ��䷽��
	double g = -1;//��䷽��
	double Histogram[256]={0};//�Ҷ�ֱ��ͼ
	double N_allpixel = width * height;//��������
	for(int k = 0; k < height; k++)
	{
		//����ֱ��ͼ
		uchar* data = img.ptr<uchar>(k);
		for(int n = 0; n < width; n++)
		{
			Histogram[data[n]]++;
		} 
	}

	// 	Mat Hist = Mat::zeros(256, 256, CV_8UC1);
	// 	//double *tmpHistogram = Histogram;
	// 	for(int k = 0; k < 256; k++)
	// 	{
	// 		//����ֱ��ͼ
	// 		uchar* data = Hist.ptr<uchar>(k);
	// 		Histogram[k] = Histogram[k]/2773*255;
	// 		for(int n = 0; n < (int)Histogram[k]; n++)
	// 		{
	// 			data[n] = 255;
	// 		} 
	// 	}
	// 	imshow("hist", Hist);



	//������ֵ
	int tmpBlack = 0;
	int tmpMiddle = 0;
	for (int i = 0; i < 256; i++)
	{
		N_object += Histogram[i];
		N_blackground = 0;
		for (int k = i+10; k < 256; k++)
		{
			N_blackground += Histogram[k];
		}
		//if(0 == N_object) continue;//û��ǰ������ѭ��
		for (int j = i+10; j < 256; j++)
		{
			gSum_object = 0;
			gSum_middle = 0;
			gSum_blackground = 0;

			N_middle = N_allpixel - N_object - N_blackground;
			w0 = N_object / N_allpixel;
			w2 = N_blackground / N_allpixel;
			w1 = 1 - w0 - w2;

			//��u0
			for (int k = 0; k <= i; k++)
			{
				gSum_object += k*Histogram[k];
			}
			u0 = gSum_object / N_object;

			//��u1
			for (int k = i+1; k <= j; k++)
			{
				gSum_middle += k*Histogram[k] ;
			}
			u1 = gSum_middle / N_middle;

			//��u2
			for(int k = j+1; k < 256; k++)
			{
				gSum_blackground += k*Histogram[k];
			}
			u2 = gSum_blackground / N_blackground;

			u = w0*u0 + w1*u1 + w2*u2;
			g = w0*(u - u0)*(u - u0) + w1*(u - u1)*(u - u1) + w2*(u - u2)*(u - u2); 

			if (tempg < g)//ͨ����䷽�����޸���ֵ
			{
				tempg = g;
				Threshold1 = (int)i;
				Threshold2 = (int)j;
				tmpBlack = N_object;
				tmpMiddle = N_middle;
			}

			N_blackground -= Histogram[j];

		}
	}

	int MiddlePixel = 255;
	if (tmpBlack > tmpMiddle)
	{
		MiddlePixel = 0;
	}


	for (int y=0; y < height; y++)
	{ 
		uchar* dstdata = dst.ptr<uchar>(y);
		uchar* srcdata = src.ptr<uchar>(y);
		for (int x=0; x < width; x++)
		{ 

			if ((int)srcdata[x] > Threshold2) 
				dstdata[x] = 255; 
			if ((int)srcdata[x] > Threshold1 && (int)srcdata[x] <= Threshold2) 
				dstdata[x] = MiddlePixel;
			if ((int)srcdata[x] <= Threshold1) 
				dstdata[x] = 0;
		} 
	}


	//��ɫ������������40%����Ϊ����Ϊ�ף��滻Ϊ�ڱ���
	int BlackgroundNum = 0;
	int AllNum = dst.rows * dst.cols;
	for (int i = 0; i < dst.rows; i++)
	{
		uchar* data = dst.ptr<uchar>(i);
		for (int j = 0; j < dst.cols; j++)
		{
			if (data[j] == 0)
			{
				BlackgroundNum++;
			}
		}
	}

	float BlackgroundRatio = 0;
	BlackgroundRatio = (float)BlackgroundNum / (float)AllNum;
	if (BlackgroundRatio < 0.4)
	{
		dst = 255 - dst;
	}
}

int OtsuAlgThreshold(const Mat image)  
{  
    if(image.channels()!=1)  
    {  
        cout<<"Please input Gray-image!"<<endl;  
        return 0;  
    }  
    int T=0; //Otsu�㷨��ֵ  
    double varValue=0; //��䷽���м�ֵ����  
    double w0=0; //ǰ�����ص�����ռ����  
    double w1=0; //�������ص�����ռ����  
    double u0=0; //ǰ��ƽ���Ҷ�  
    double u1=0; //����ƽ���Ҷ�  
    double Histogram[256]={0}; //�Ҷ�ֱ��ͼ���±��ǻҶ�ֵ�����������ǻҶ�ֵ��Ӧ�����ص�����  
    uchar *data=image.data;  
    double totalNum=image.rows*image.cols; //��������  
    //����Ҷ�ֱ��ͼ�ֲ���Histogram�����±��ǻҶ�ֵ�����������ǻҶ�ֵ��Ӧ���ص���  
    for(int i=0;i<image.rows;i++)   //Ϊ������������û�а�rows��cols���������  
    {  
        for(int j=0;j<image.cols;j++)  
        {  
            Histogram[data[i*image.step+j]]++;  
        }  
    }  
    for(int i=0;i<255;i++)  
    {  
        //ÿ�α���֮ǰ��ʼ��������  
        w1=0;       u1=0;       w0=0;       u0=0;  
        //***********����������ֵ����**************************  
        for(int j=0;j<=i;j++) //�������ָ�ֵ����  
        {  
            w1+=Histogram[j];  //�����������ص�����  
            u1+=j*Histogram[j]; //�������������ܻҶȺ�  
        }  
        if(w1==0) //�����������ص���Ϊ0ʱ�˳�  
        {  
            break;  
        }  
        u1=u1/w1; //��������ƽ���Ҷ�  
        w1=w1/totalNum; // �����������ص�����ռ����  
        //***********����������ֵ����**************************  
  
        //***********ǰ��������ֵ����**************************  
        for(int k=i+1;k<255;k++)  
        {  
            w0+=Histogram[k];  //ǰ���������ص�����  
            u0+=k*Histogram[k]; //ǰ�����������ܻҶȺ�  
        }  
        if(w0==0) //ǰ���������ص���Ϊ0ʱ�˳�  
        {  
            break;  
        }  
        u0=u0/w0; //ǰ������ƽ���Ҷ�  
        w0=w0/totalNum; // ǰ���������ص�����ռ����  
        //***********ǰ��������ֵ����**************************  
  
        //***********��䷽�����******************************  
        double varValueI=w0*w1*(u1-u0)*(u1-u0); //��ǰ��䷽�����  
        if(varValue<varValueI)  
        {  
            varValue=varValueI;  
            T=i;  
        }  
    }  
    return T;  
}  


/*
Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
/// Function header
void thresh_callback(int, void* );
int main( int argc, char** argv )
{
	const char* filename = argc >= 2 ? argv[1] : "E:/Test/18.bmp";
	Mat src = imread(filename, 0);
	/// Convert image to gray and blur it
	//cvtColor( src, src_gray, CV_BGR2GRAY );
	blur( src, src_gray, Size(3,3) );
	/// Create Window
	char* source_window = "Source";
	namedWindow( source_window, CV_WINDOW_NORMAL);
	imshow( source_window, src );
	createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
	thresh_callback( 0, 0 );
	waitKey(0);
	return(0);
} 
//	@function thresh_callback 
	void thresh_callback(int, void* )
{
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	/// Detect edges using Threshold
	threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
	/// Find contours
	findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	/// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );
	for( int i = 0; i < contours.size(); i++ )
	{ approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
	boundRect[i] = boundingRect( Mat(contours_poly[i]) );
	minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
	}
	/// Draw polygonal contour + bonding rects + circles
	Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
	{
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	//	drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
		rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
	//	circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
	}
	/// Show in a window
	namedWindow( "Contours", CV_WINDOW_NORMAL );
	imshow( "Contours", drawing );
}
*/


/*
void help()
{
	cout << "\nThis program demonstrates line finding with the Hough transform.\n"
		"Usage:\n"
		"./houghlines <image_name>, Default is pic1.jpg\n" << endl;
}

int main(int argc, char** argv)
{
	const char* filename = argc >= 2 ? argv[1] : "E:/Test/1_Camera.bmp";
	Mat src = imread(filename, 0);
	if(src.empty())
	{
		help();
		cout << "can not open " << filename << endl;
		return -1;
	}
	Mat dst, cdst;
	//blur(src, cdst, Size(3,3));
// 	Canny(src, dst, 50, 255, 3);
// 	cvtColor(dst, cdst, CV_GRAY2BGR);

#if 0
	vector<Vec2f> lines;
	HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );
	for( size_t i = 0; i < lines.size(); i++ )
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
	}
#else
	int maxLen = 0;
	int minLen = 1000;
	vector<Vec4i> lines;
	HoughLinesP(src, lines, 1, CV_PI/180, 10, 100, 10 );
	for( size_t i = 0; i < lines.size(); i++ )
	{
		int x1 = lines[i][0];
		int y1 = lines[i][1];
		int x2 = lines[i][2];
		int y2 = lines[i][3];
		int len = sqrt(x1*x1 + y1*y2);

		
		maxLen = max(len, maxLen);
		
		minLen = min(len, minLen);
		if (len > 2000)
		{
			Vec4i l = lines[i];
			line( src, Point(x1, y1), Point(x2, y2), Scalar(0,0,255), 1, CV_AA);
		}

		double k = (lines[i][3] - lines[i][1])/(lines[i][2]-lines[i][0]);
		
	}
#endif
// 	imshow("source", src);
// 	imshow("detected lines", cdst);
// 	waitKey();
	const char* filenameW = argc >= 2 ? argv[1] : "E:/Test/18-HoughLinesP.bmp";
	imwrite(filenameW, src);
	
	return 0;
}
*/

/*
/// Global variables
Mat src, src_gray;
Mat dst, detected_edges;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";
/// function CannyThreshold
/// brief Trackbar callback - Canny thresholds input with a ratio 1:3

void CannyThreshold(int, void*)
{
	/// Reduce noise with a kernel 3x3
	blur( src_gray, detected_edges, Size(3,3) );
	/// Canny detector
	Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
	/// Using Canny��s output as a mask, we display our result
	dst = Scalar::all(0);
	src.copyTo( dst, detected_edges);
	imshow( window_name, dst );
}

int main( int argc, char** argv )
{
	/// Load an image
	src = imread( "E:/Test/18.bmp");
	if( !src.data )
	{ return -1; }
	/// Create a matrix of the same type and size as src (for dst)
	dst.create( src.size(), src.type() );
	/// Convert the image to grayscale
	cvtColor( src, src_gray, CV_BGR2GRAY );
	/// Create a window
	namedWindow( window_name, CV_WINDOW_NORMAL );
	/// Create a Trackbar for user to enter threshold
	createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
	/// Show the image
	CannyThreshold(0, 0);
	/// Wait until user exit program by pressing a key
	waitKey(0);
	return 0;
}
*/


void FindCenteroID(Mat& src, QString filePathW, int& x, int& y) 
{
	vector<vector<Point> > contours;//contours�����ͣ�˫�ص�vector
	vector<Vec4i> hierarchy;//Vec4i��ָÿһ��vectorԪ�������ĸ�int������
	findContours(src, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	/// �����  
	vector<Moments> mu(contours.size());  
	for (int i = 0; i < contours.size(); i++)  
	{  
		mu[i] = moments(contours[i], false);  
	}  
	///  �������ľ�:  
	vector<Point2f> mc(contours.size());  
	for (int i = 0; i < contours.size(); i++)  
	{  
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
	}  
	/// ��������  
	Mat drawing = Mat::zeros(src.size(), CV_8UC1);
	for (int i = 0; i < contours.size(); i++)  
	{  

		if (contourArea(contours[i]) > 100)
		{
			Scalar color = Scalar(255);  
			drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());  
			circle(src, mc[i], 4, color, -1, 8, 0);
			x = mc[i].x;
			y = mc[i].y;
		}

	}
	imwrite(filePathW.toStdString(), src);
	qDebug() << filePathW.toStdString().c_str();
}

void OC_Test_MV(QString filePathR, QString filePathW)
{
	// MV OC Test
	// 6
	int ocX = 0;
	int ocY = 0;
	Mat dstV, dstH, dst;
	Mat src = imread(filePathR.toStdString());
	if (src.empty()) 
	{
		printf("could not load image...\n");
		return ;
	}
	cvtColor(src, dst, CV_BGR2GRAY);  


	// �����˲���ģ��/ƽ����
	// Horizontal binary
	Mat Sobel_x = (Mat_<char>(3, 3) <<	-1, 0, 1, \
										-2, 0, 2, \
										-1, 0, 1); //Sobel��x,ˮƽ��������
// 	Mat Sobel_x = (Mat_<char>(2, 2) << -1,  1, \
// 									   -1,  1); //Sobel��x,ˮƽ��������
	//Sobel_x = Gaussian_kernal(5, 1.0, 1);
	filter2D(dst, dstH, -1, Sobel_x, Point(-1, -1), 0, 0);
//	GaussianBlur(dst, dstH, Size(5, 5), 0, BORDER_DEFAULT);  
//	imshow("Robert_x image", dst);
	filePathW.replace(".bmp", "robert_x.bmp");
	imwrite(filePathW.toStdString(), dstH);
	filePathW.replace("robert_x.bmp", ".bmp");

	
	filePathW.replace(".bmp", "THRESH_OTSU_h.bmp");
	//threshold(dstH, dstH, OtsuAlgThreshold(dstH), 255, THRESH_BINARY|CV_THRESH_OTSU);
	threshold(dstH, dstH, 0, 255, THRESH_BINARY|CV_THRESH_OTSU);
	//Otsu2Threshold(dstH, dstH);
	imwrite(filePathW.toStdString(), dstH);
	qDebug() << filePathW.toStdString().c_str();
 	filePathW.replace("THRESH_OTSU_h.bmp", ".bmp");

	filePathW.replace(".bmp", "-centerH.bmp");
	int tempY = 0;
	FindCenteroID(dstH, filePathW, ocX, tempY);
	filePathW.replace("-centerH.bmp", ".bmp");


// 	Mat dstCannyH;
// 	filePathW.replace(".bmp", "CannyH.bmp");
// 	Canny( dstH, dstCannyH, 20, 255, 3, true);//��Ե���
// 	imwrite(filePathW.toStdString(), dstCannyH);
// 	qDebug() << filePathW.toStdString().c_str();
// 	filePathW.replace("CannyH.bmp", ".bmp");


	// Vertical binary
	Mat Sobel_y = (Mat_<char>(3, 3) <<	-1, -2, -1, \
										 0,  0,  0, \
										 1,  2,  1); //Sobel��y,��ֱ��������
// 	Mat Sobel_y = (Mat_<char>(2, 2) << -1, -1, \
// 										1,  1); //Sobel��y,��ֱ��������
	//Sobel_y = Gaussian_kernal(5, 1.0, 2);
	filter2D(dst, dstV, -1, Sobel_y, Point(-1, -1), 0, 0);
//	GaussianBlur(dst, dstV, Size(5, 5), 0);  
//	imshow("Robert_y image", dst);
	filePathW.replace(".bmp", "robert_y.bmp");
	imwrite(filePathW.toStdString(), dstV);
	filePathW.replace("robert_y.bmp", ".bmp");

	
	filePathW.replace(".bmp", "THRESH_OTSU_v.bmp");
	//threshold(dstH, dstH, OtsuAlgThreshold(dstH), 255, THRESH_BINARY|CV_THRESH_OTSU);
	threshold(dstH, dstH, 0, 255, THRESH_BINARY|CV_THRESH_OTSU);
	//Otsu2Threshold(dstV, dstV);
	imwrite(filePathW.toStdString(), dstV);
	qDebug() << filePathW.toStdString().c_str();
 	filePathW.replace("THRESH_OTSU_v.bmp", ".bmp");

	filePathW.replace(".bmp", "-centerV.bmp");
	int tempX = 0;
	FindCenteroID(dstH, filePathW, tempX, ocY);
	filePathW.replace("-centerV.bmp", ".bmp");

	qDebug() << ocX;
	qDebug() << ocY;

// 	Mat dstCannyV;
// 	filePathW.replace(".bmp", "CannyV.bmp");
// 	Canny( dstV, dstCannyV, 20, 255, 3, true);//��Ե���
// 	imwrite(filePathW.toStdString(), dstCannyV);
// 	qDebug() << filePathW.toStdString().c_str();
// 	filePathW.replace("CannyV.bmp", ".bmp");


	
	
		
}
void OC_Test_HW_2(QString filePathR, QString filePathW)
{
	// 6
	int ocX = 0;
	int ocY = 0;
	int height = 0;
	int width = 0;

	Mat src = imread(filePathR.toStdString());
	// 2456 x 2054
	// ���Ŀ� 160x230 pixel
	height = src.rows;
	width = src.cols;
	{
		Mat dst;  
		cvtColor(src, dst, CV_BGR2GRAY);  
		vector<vector<Point> > contours;//contours�����ͣ�˫�ص�vector
		vector<Vec4i> hierarchy;//Vec4i��ָÿһ��vectorԪ�������ĸ�int������
		GaussianBlur(dst, dst, Size(81, 81), 40, 40);
// 		GaussianBlur(dst, dst, Size(5, 5), 0);
//  	adaptiveThreshold(dst, dst, 255, THRESH_BINARY, 0, 3, 0.5);
		threshold(dst, dst, 30, 255, THRESH_BINARY);
// 		threshold(dst, dst , 0, 255, CV_THRESH_OTSU);
// 		imwrite(filePathW.toStdString(), dst);
// 		qDebug() << filePathW.toStdString().c_str();
		findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		/// �����  
		vector<Moments> mu(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mu[i] = moments(contours[i], false);  
		}
		///  �������ľ�:  
		vector<Point2f> mc(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
		}  
		/// ��������
		double area = 0;
		double maxArea = 0;
		int index = 0;
		for (int i = 0; i < mc.size(); i++)
		{
			Scalar color = Scalar(255);
			drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());
			area = contourArea(contours[i]);
			if (area > maxArea)
			{
				index = i;
			}
		}
		// Draw center and rectangle
		Scalar color = Scalar(0,255,0);
		circle(src, mc[index], 4, color, -1, 8, 0);
		ocX = mc[index].x;
		ocY = mc[index].y;
		rectangle(src, Point(ocX-1000, ocY-800), Point(ocX+1000, ocY+800), color);

		imwrite(filePathW.toStdString(), src);
		qDebug() << filePathW.toStdString().c_str() << ocX << ocY;;
	}
	return ;
}

void OC_Test_HW_1(QString filePathR, QString filePathW)
{
	// 6
	int ocX = 0;
	int ocY = 0;
	Mat src = imread(filePathR.toStdString());
	int height = src.rows;
	int width = src.cols;
	int w = 500;
	int h = 500;
	{
		Mat dst;  
		cvtColor(src, dst, CV_BGR2GRAY);  
		vector<vector<Point> > contours;//contours�����ͣ�˫�ص�vector
		vector<Vec4i> hierarchy;//Vec4i��ָÿһ��vectorԪ�������ĸ�int������
		GaussianBlur(dst, dst, Size(5, 5), 0);
// 		adaptiveThreshold(dst, dst, 255, THRESH_BINARY, 0, 5, 0.5);
 		threshold(dst, dst, 30, 255, THRESH_BINARY);
//		threshold(dst, dst , 0, 255, CV_THRESH_OTSU);
// 		imwrite(filePathW.toStdString(), dst);
// 		qDebug() << filePathW.toStdString().c_str();
		findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		/// �����  
		vector<Moments> mu(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mu[i] = moments(contours[i], false);  
		}
		///  �������ľ�:  
		vector<Point2f> mc(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
		}  
		/// ��������
		double area = 0;
		double maxArea = 0;
		int index = 0;
		for (int i = 0; i < mc.size(); i++)
		{
			Scalar color = Scalar(255);
			drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());
			area = contourArea(contours[i]);
			if (area > maxArea)
			{
				index = i;
			}
		}
		Scalar color = Scalar(0,255,0);
		circle(src, mc[index], 4, color, -1, 8, 0);
		ocX = mc[index].x;
		ocY = mc[index].y;
		rectangle(src, Point(ocX-1000, ocY-800), Point(ocX+1000, ocY+800), color);

		imwrite(filePathW.toStdString(), src);
		qDebug() << filePathW.toStdString().c_str();
	}
	return ;

	{
		Mat src = imread(filePathR.toStdString());
		Mat dst;  
		cvtColor(src, dst, CV_BGR2GRAY);
		Mat roi(dst, Rect(ocX-w, ocY-h, w*2, h*2));
		Mat element = getStructuringElement(MORPH_RECT, Size(5, 5)); //��һ������MORPH_RECT��ʾ���εľ���ˣ���Ȼ������ѡ����Բ�εġ������͵�
		erode(roi, roi, element);//��ʴ����
		dilate(roi, roi, element);//���Ͳ���
		vector<vector<Point> > contours;//contours�����ͣ�˫�ص�vector
		vector<Vec4i> hierarchy;//Vec4i��ָÿһ��vectorԪ�������ĸ�int������
		GaussianBlur(roi, roi, Size(5, 5), 0);
		threshold(roi, roi, 20, 255, THRESH_BINARY_INV);
		imwrite(filePathW.toStdString(), src);
		qDebug() << filePathW.toStdString().c_str();
		findContours(roi, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		/// �����  
		vector<Moments> mu(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mu[i] = moments(contours[i], false);
		}  
		///  �������ľ�:  
		vector<Point2f> mc(contours.size());  
		for (int i = 0; i < contours.size(); i++)  
		{  
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);

		}
		/// ��������
		for (int i = 0; i < mc.size(); i++)
		{

			mc[i].x = ocX - w + mc[i].x;
			mc[i].y = ocY - h + mc[i].y;
			Scalar color = Scalar(255);
			drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());
			color = Scalar(0,0,255);
			circle(src, mc[i], 4, color, -1, 8, 0);

			double area = contourArea(contours[i]);
			if (area > 20000)
			{
				Scalar color = Scalar(0,255,0);
				circle(src, mc[i], 4, color, -1, 8, 0);
			}
		}
		imwrite(filePathW.toStdString(), src);
		qDebug() << filePathW.toStdString().c_str();
	}
}


void ClockWiseRotate(QString filePathR, QString filePathW)
{
	Mat src = imread(filePathR.toStdString());
	//flipCode: ��תģʽ��
	// ==0 ��ֱ��ת����X�ᷭת��
	// > 0 ˮƽ��ת����Y�ᷭת��
	// < 0 ˮƽ��ֱ��ת������X�ᷭת������Y�ᷭת���ȼ�����ת180�㣩
	//clockwise 90
	transpose(src, src);
	flip(src, src, 1);
	imwrite(filePathW.toStdString(), src);

	//clockwise 180
// 	flip(src, src, -1);
// 	imwrite(filePathW.toStdString(), src);

	//clockwise 270
// 	transpose(src, src);
// 	flip(src, src, 0);
// 	imwrite(filePathW.toStdString(), src);
}

bool HuaweiDllCall()
{
	char cTemp[MAX_PATH];
	sprintf_s(cTemp, "E:/xx/x/xNicolas/OpencvDemo/AA_Single/Huawei/Proj_IS.dll");

	//TraverseDir("E:/xx/x/x11", ".bmp");

	HMODULE hdll = LoadLibraryA(cTemp);
	if (hdll)
	{
		qDebug() << "LoadLibrary OK";
		//���庯��ָ��
		FARPROC fpFun = GetProcAddress(hdll, "dfSpotTestCreate");
		//ִ��dll�ӿں���
		if (!fpFun)
		{

		}
		
	}
	else
	{
		qDebug() << "LoadLibrary NG";
	//	DWORD dwErr = GetLastError();
		LPVOID lpMsgBuf;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), 0, (LPSTR)&lpMsgBuf, 0, NULL);

		char cTemp[MAX_PATH];
		sprintf_s(cTemp, "%s", lpMsgBuf);
		qDebug() << cTemp;
	}
	system("pause");
	return 0;
}

void MophologyOpenClose(QString filePathR, QString filePathW)
{
	Mat img = imread(filePathR.toStdString());
// 	namedWindow("ԭʼͼ", WINDOW_NORMAL);
// 	imshow("ԭʼͼ", img);
	//��ȡ�Զ����
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5)); //��һ������MORPH_RECT��ʾ���εľ���ˣ���Ȼ������ѡ����Բ�εġ������͵�
	Mat img1,img2;
// 	dilate(img, img1, element);//���Ͳ���
// 	erode(img1, img2, element);//��ʴ����
// 	namedWindow("��ʴ����", WINDOW_NORMAL);
// 	imshow("��ʴ����", img2);

	Mat out;
 	morphologyEx(img, img1, MORPH_CLOSE, element);
// 	namedWindow("MORPH_CLOSE", WINDOW_NORMAL);
// 	imshow("MORPH_CLOSE", img1);
	filePathW.replace(".bmp", "-close.bmp");
	imwrite(filePathW.toStdString(), img1);
	qDebug() << filePathW.toStdString().c_str();
	filePathW.replace("-close.bmp", ".bmp");

	morphologyEx(img, img2, MORPH_OPEN, element);
// 	namedWindow("MORPH_OPEN", WINDOW_NORMAL);
// 	imshow("MORPH_OPEN", img2);
	filePathW.replace(".bmp", "-open.bmp");
	imwrite(filePathW.toStdString(), img2);
	qDebug() << filePathW.toStdString().c_str();
}


void LineDetect_NoName(QString filePathR, QString filePathW)
{

	//����ͼ��ת��Ϊ�Ҷ�ͼ  
	Mat src = imread(filePathR.toStdString()); 
	Mat dst;
	//Ϊcanny��Եͼ������ռ䣬1��ʾ��ͨ���Ҷ�ͼ
	cvtColor(src, dst, CV_BGR2GRAY);
	/// Reduce the noise so we avoid false circle detection
	//threshold(dst, dst, 30, 255, THRESH_BINARY);
	threshold(dst, dst , 0, 255, CV_THRESH_OTSU);
	imwrite(filePathW.toStdString(), dst);
	qDebug() << filePathW.toStdString().c_str();
}


void EdgeDetect_Canny(QString filePathR, QString filePathW)
{

	//����ͼ��ת��Ϊ�Ҷ�ͼ  
	Mat src = imread(filePathR.toStdString()); 
	Mat dst;
	//Ϊcanny��Եͼ������ռ䣬1��ʾ��ͨ���Ҷ�ͼ
	cvtColor(src, dst, CV_BGR2GRAY);
	Canny( dst, dst, 20, 255, 3, true);//��Ե���
// 	namedWindow( "src", CV_WINDOW_AUTOSIZE);  
// 	namedWindow( "canny", CV_WINDOW_AUTOSIZE);  
// 	imshow( "src", src );  
// 	imshow( "canny", dst );
	imwrite(filePathW.toStdString(), dst);
	qDebug() << filePathW.toStdString().c_str();
}

int EdgeDetect_Sobel(QString filePathR, QString filePathW)
{
	Mat src, dst, src_gray;
	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	int c;
	/// Load an image
	src = imread(filePathR.toStdString());
	if( !src.data )
	{ return -1; }
	GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
	/// Convert it to gray
	cvtColor( src, src_gray, CV_RGB2GRAY );
	/// Create window
	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_x, abs_grad_x );
	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_y, abs_grad_y );
	/// Total Gradient (approximate)
	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
	//imshow( window_name, grad );
	imwrite(filePathW.toStdString(), grad);
	qDebug() << filePathW.toStdString().c_str();
	return 0;
} 


int main(int argc, char *argv[])
{
	vector<QString> vFilePath;
	QString filePathW;
	TraverseDir("F:/Test", "bmp", vFilePath);
	qDebug() << "Traverse directory files done.";

	double x = 0;
	double y = 0;
	for (vector<QString>::iterator it = vFilePath.begin(); it != vFilePath.end(); ++it)
	{
		QString filePathR = *it;
		QString filePathW = filePathR;
		filePathW.replace("F:/Test", "F:/Test_Result");
	// 	EdgeDetect_Sobel(filePathR, filePathW);
	//	OC_Test_MV(filePathR, filePathW);
	//	Point centerPt1 = OC_Test_MV_2(filePathR, filePathW);
	// 	HuaweiDllCall(filePathR, filePathW);
		OC_Test_HW_2(filePathR, filePathW);
	//	OC_Test_HW_1(filePathR, filePathW);
	// 	EdgeDetect_Canny(filePathR, filePathW);
	// 	MophologyOpenClose(filePathR, filePathW);
	//	LineDetect_NoName(filePathR, filePathW);
	}


	return 0;


	QApplication a(argc, argv);		
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));

	QDlgMain dlg;
	dlg.show();
	a.exec();

	return 0;
}





// 3
// 	for(int p = 1; p <= 4; p++)
// 	{
// 		for (int v = 1; v <= 4; v++)
// 		{
// 			for (int n = 0; n <= 30; n+=10)
// 			{
// 				char cTemp[MAX_PATH];
// 				sprintf_s(cTemp, "E:/xx/x/xp%dv%d.bmp", p, v);
// 				Mat src = imread(cTemp);
// 
// 				Mat dst;  
// 				cvtColor(src, dst, CV_BGR2GRAY);  
// 				vector<vector<Point> > contours;//contours�����ͣ�˫�ص�vector
// 				vector<Vec4i> hierarchy;//Vec4i��ָÿһ��vectorԪ�������ĸ�int������
// 				GaussianBlur(dst, dst, Size(5, 5), 0);  
// 				threshold(dst, dst, n, 255, THRESH_BINARY);
// 				findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));  
// 				/// �����  
// 				vector<Moments> mu(contours.size());  
// 				for (int i = 0; i < contours.size(); i++)  
// 				{  
// 					mu[i] = moments(contours[i], false);  
// 				}  
// 				///  �������ľ�:  
// 				vector<Point2f> mc(contours.size());  
// 				for (int i = 0; i < contours.size(); i++)  
// 				{  
// 					mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
// 				}  
// 				/// ��������  
// 				Mat drawing = Mat::zeros(dst.size(), CV_8UC1);  
// 				for (int i = 0; i < contours.size(); i++)  
// 				{  
// 					Scalar color = Scalar(255);  
// 					drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());  
// 					circle(src, mc[i], 4, color, -1, 8, 0);
// 					// 					Rect rect;
// 					// 					rect.x = mc[i].x-75;
// 					// 					rect.y = mc[i].y-100;
// 					// 					rect.width = 150;
// 					// 					rect.height = 200;
// 					// 					rectangle(drawing, rect, color, -1, 8, 0);
// 				}
// 				// namedWindow("outImage", WINDOW_AUTOSIZE);
// 				// imshow("outImage",drawing);
// 				sprintf_s(cTemp, "E:/xx/x/xp%dv%d-%d.bmp", p, v, n);
// 				imwrite(cTemp, src);
// 			}
// 		}
// 	}

	//3-1
// 	for(int p = 1; p <= 15; p++)
// 	{
// 		char cTemp[MAX_PATH];
// 		sprintf_s(cTemp, "E:/xx/x/x%d.bmp", p);
// 		Mat src = imread(cTemp);
// 
// 		Mat dst;  
// 		cvtColor(src, dst, CV_BGR2GRAY);  
// 		vector<vector<Point> > contours;//contours�����ͣ�˫�ص�vector
// 		vector<Vec4i> hierarchy;//Vec4i��ָÿһ��vectorԪ�������ĸ�int������
// 		GaussianBlur(dst, dst, Size(5, 5), 0);  
// 		threshold(dst, dst, 10, 255, THRESH_BINARY);
// 		findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
// 		/// �����  
// 		vector<Moments> mu(contours.size());  
// 		for (int i = 0; i < contours.size(); i++)  
// 		{  
// 			mu[i] = moments(contours[i], false);  
// 		}  
// 		///  �������ľ�:  
// 		vector<Point2f> mc(contours.size());  
// 		for (int i = 0; i < contours.size(); i++)  
// 		{  
// 			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);  
// 		}  
// 		/// ��������  
// 		Mat drawing = Mat::zeros(dst.size(), CV_8UC1);  
// 		for (int i = 0; i < contours.size(); i++)  
// 		{  
// 			Scalar color = Scalar(255);  
// 			drawContours(src, contours, i, color, 2, 8, hierarchy, 0, Point());  
// 			circle(src, mc[i], 4, color, -1, 8, 0);
// 		}
// 		sprintf_s(cTemp, "E:/xx/x/x%d-%d.bmp", p, 1);
// 		imwrite(cTemp, src);
// 		qDebug() << cTemp << endl;
// 	}
// 	return 0;


