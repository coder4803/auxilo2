/********************************************************************************
** Form generated from reading UI file 'graphicalui.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHICALUI_H
#define UI_GRAPHICALUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GraphicalUI
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *displayLayout;
    QVBoxLayout *outoutLayout;
    QLabel *outputLabel;
    QListWidget *outputList;
    QVBoxLayout *debugLayout;
    QLabel *debugLabel;
    QListWidget *debugList;
    QPushButton *quitBtn;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *GraphicalUI)
    {
        if (GraphicalUI->objectName().isEmpty())
            GraphicalUI->setObjectName(QStringLiteral("GraphicalUI"));
        GraphicalUI->resize(800, 600);
        centralwidget = new QWidget(GraphicalUI);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        displayLayout = new QHBoxLayout();
        displayLayout->setObjectName(QStringLiteral("displayLayout"));
        outoutLayout = new QVBoxLayout();
        outoutLayout->setObjectName(QStringLiteral("outoutLayout"));
        outputLabel = new QLabel(centralwidget);
        outputLabel->setObjectName(QStringLiteral("outputLabel"));

        outoutLayout->addWidget(outputLabel);

        outputList = new QListWidget(centralwidget);
        outputList->setObjectName(QStringLiteral("outputList"));

        outoutLayout->addWidget(outputList);


        displayLayout->addLayout(outoutLayout);

        debugLayout = new QVBoxLayout();
        debugLayout->setObjectName(QStringLiteral("debugLayout"));
        debugLabel = new QLabel(centralwidget);
        debugLabel->setObjectName(QStringLiteral("debugLabel"));

        debugLayout->addWidget(debugLabel);

        debugList = new QListWidget(centralwidget);
        debugList->setObjectName(QStringLiteral("debugList"));

        debugLayout->addWidget(debugList);


        displayLayout->addLayout(debugLayout);


        verticalLayout->addLayout(displayLayout);

        quitBtn = new QPushButton(centralwidget);
        quitBtn->setObjectName(QStringLiteral("quitBtn"));

        verticalLayout->addWidget(quitBtn);

        GraphicalUI->setCentralWidget(centralwidget);
        menubar = new QMenuBar(GraphicalUI);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        GraphicalUI->setMenuBar(menubar);
        statusbar = new QStatusBar(GraphicalUI);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        GraphicalUI->setStatusBar(statusbar);

        retranslateUi(GraphicalUI);

        QMetaObject::connectSlotsByName(GraphicalUI);
    } // setupUi

    void retranslateUi(QMainWindow *GraphicalUI)
    {
        GraphicalUI->setWindowTitle(QApplication::translate("GraphicalUI", "MainWindow", 0));
        outputLabel->setText(QApplication::translate("GraphicalUI", "Output", 0));
        debugLabel->setText(QApplication::translate("GraphicalUI", "Debug", 0));
        quitBtn->setText(QApplication::translate("GraphicalUI", "Quit", 0));
    } // retranslateUi

};

namespace Ui {
    class GraphicalUI: public Ui_GraphicalUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHICALUI_H
