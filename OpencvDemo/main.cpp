#include <QtWidgets/QApplication>

#include "QDlgMain.h"
#include "GlobalFunction.h"

int main(int argc, char *argv[])
{

	vector<QString> vFilePath;
	QString filePathW;
	QString test = "E:/Test";
	QString testResult = "E:/Test_Result";
	TraverseDir(test, "bmp", vFilePath);
	qDebug() << "Traverse directory files done.";

	for (vector<QString>::iterator it = vFilePath.begin(); it != vFilePath.end(); ++it)
	{
		QString filePathR = *it;
		QString filePathW = filePathR;
		filePathW.replace(test, testResult);

		QString dirPath = filePathW.left(filePathW.lastIndexOf("/"));
		CreateDir(dirPath);

// 		EdgeDetect_Sobel(filePathR, filePathW);
// 		EdgeDetect_Canny(filePathR, filePathW);

// 		OC_Test_MV_3(filePathR, filePathW);
// 		OC_Test_MV_2(filePathR, filePathW);
//		OC_Test_MV_4(filePathR, filePathW);
//		Find_HW_X(filePathR, filePathW);
// 		Find_HW_Y(filePathR, filePathW);
// 		Find_HW_XY(filePathR, filePathW);
//		Find_HW_XY_2(filePathR, filePathW);
// 		OC_Test_HW_3(filePathR, filePathW);
// 		OC_Test_HW_4(filePathR, filePathW);
// 		OC_Test_HW_1(filePathR, filePathW);

// 		MophologyOpenClose(filePathR, filePathW);
// 		LineDetect_NoName(filePathR, filePathW);
// 		LineDetect_HT(filePathR, filePathW);
//		Mophology_WaterShed(filePathR, filePathW);

//		HarrisDectect_1(filePathR, filePathW);
		FindCircle(filePathR, filePathW);
	}
//	system("pause");

	return 0;


	QApplication a(argc, argv);		
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));

	QDlgMain dlg;
	dlg.show();
	a.exec();

	return 0;
}
