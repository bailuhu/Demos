/********************************************************************************
** Form generated from reading UI file 'QDlgMain.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QDLGMAIN_H
#define UI_QDLGMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QDlgMain
{
public:

    void setupUi(QWidget *QDlgMain)
    {
        if (QDlgMain->objectName().isEmpty())
            QDlgMain->setObjectName(QStringLiteral("QDlgMain"));
        QDlgMain->resize(1920, 1033);
        QDlgMain->setStyleSheet(QLatin1String("QGroupBox{\n"
"border-style: solid;\n"
"border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0.170455 rgba(188, 157, 225, 255), stop:0.375 rgba(71, 114, 193, 255), stop:0.585227 rgba(32, 34, 130, 255), stop:0.784091 rgba(115, 107, 177, 255));\n"
"border-width:1px;\n"
"}\n"
"QWidget#QDlgMain{\n"
"	background-color: qlineargradient(spread:repeat, x1:0, y1:1, x2:0, y2:0, stop:0.0454545 rgba(229, 236, 254, 255), stop:0.176136 rgba(254, 245, 250, 255), stop:0.3125 rgba(255, 252, 252, 255), stop:0.517045 rgba(242, 238, 254, 255), stop:0.681818 rgba(242, 234, 255, 255), stop:0.75 rgba(230, 240, 255, 255), stop:0.960227 rgba(255, 255, 255, 255));\n"
"\n"
"}"));

        retranslateUi(QDlgMain);

        QMetaObject::connectSlotsByName(QDlgMain);
    } // setupUi

    void retranslateUi(QWidget *QDlgMain)
    {
        QDlgMain->setWindowTitle(QApplication::translate("QDlgMain", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class QDlgMain: public Ui_QDlgMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDLGMAIN_H
