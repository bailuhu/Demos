//////////////////////////////////////////////////////////////////////
//Description:  软件的主界面，基于QMainWindow        
//Time：7/1/2016
//////////////////////////////////////////////////////////////////////

#pragma once

#include <QtWidgets>
#include "ui_QDlgMain.h"


class QDlgMain :public QMainWindow
{
	Q_OBJECT
public:
	QDlgMain(QWidget *parent=0);
	~QDlgMain(void);


};

