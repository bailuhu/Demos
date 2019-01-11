#include "GlobalFunction.h"

#include <QDebug.h>

void TraverseDir(QString dirPath, QString filter, vector<QString>& vFilePath)
{  
	QDir dir(dirPath);  

	foreach(QFileInfo fileInfo, dir.entryInfoList())  
	{  
		if(fileInfo.isFile())  
		{  
			
			if (fileInfo.suffix() == filter)
			{
				qDebug()<< "Find file:" << fileInfo.filePath();
				vFilePath.push_back(fileInfo.filePath());
			}
		}
		else  
		{  
			if(fileInfo.fileName() == "." || fileInfo.fileName() == "..") continue;  
			qDebug() << "Find Dir:" << fileInfo.absoluteFilePath();
			TraverseDir(fileInfo.absoluteFilePath(), filter, vFilePath);
		}  
	}  

}  
