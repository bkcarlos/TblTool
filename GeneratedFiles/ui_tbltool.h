/********************************************************************************
** Form generated from reading UI file 'tbltool.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TBLTOOL_H
#define UI_TBLTOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TblTool
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TblTool)
    {
        if (TblTool->objectName().isEmpty())
            TblTool->setObjectName(QStringLiteral("TblTool"));
        TblTool->resize(400, 300);
        menuBar = new QMenuBar(TblTool);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        TblTool->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TblTool);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TblTool->addToolBar(mainToolBar);
        centralWidget = new QWidget(TblTool);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        TblTool->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(TblTool);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        TblTool->setStatusBar(statusBar);

        retranslateUi(TblTool);

        QMetaObject::connectSlotsByName(TblTool);
    } // setupUi

    void retranslateUi(QMainWindow *TblTool)
    {
        TblTool->setWindowTitle(QApplication::translate("TblTool", "TblTool", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TblTool: public Ui_TblTool {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TBLTOOL_H
