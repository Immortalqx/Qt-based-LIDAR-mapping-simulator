#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

#include "include/simulator.h"
#include "include/viewer.h"
#include "include/mapper.h"
#include "include/logprinter.h"
#include "include/console.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private:
    QWidget *centralWidget;
    Simulator *simulator; //仿真环境界面
    Mapper *mapper; //建图的界面
    Viewer *viewer; //雷达看到的界面
    LogPrinter *logPrinter; //输出相关指令的界面
    Console *controller; //控制界面

    void setupUi();
};

#endif // MAINWINDOW_H
