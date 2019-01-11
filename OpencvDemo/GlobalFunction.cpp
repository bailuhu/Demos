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

bool CreateDir(QString dirPath)
{
	
	QDir dir;
	if (dir.exists(dirPath) == true)
	{
		return true;
	}
	else
	{
		int index = 2; // È¥µôÅÌ·û
		do
		{
			index = dirPath.indexOf("/", index+1);
			QString tempPath = dirPath.left(index);
			if (dir.exists(dirPath) == false)
			{
				if (dir.mkdir(tempPath) == true)
				{
					qDebug() << "Create Dir:" << tempPath;
				}
			}
		}
		while (dir.exists(dirPath) == false);
	}

	

	return true;
}
