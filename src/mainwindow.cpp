#include "include/mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent)
{
    this->setupUi();
}

//把用户界面给画出来
void MainWindow::setupUi()
{
    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("MainWindow"));

    this->resize(1600, 801);
    this->setMinimumSize(QSize(1600, 801));
    this->setMaximumSize(QSize(1600, 801));
    this->setStyleSheet("background:rgb(100,100,100)");

    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QStringLiteral("centralWidget"));

    simulator = Simulator::getSimulator(centralWidget);
    simulator->setObjectName(QStringLiteral("simulator"));
    simulator->setGeometry(QRect(1, 1, 798, 799));
    simulator->setStyleSheet("background:rgb(200,200,200)");
    simulator->setFocusPolicy(Qt::ClickFocus);

    mapper = Mapper::getMapper(centralWidget);
    mapper->setObjectName(QStringLiteral("mapper"));
    mapper->setGeometry(QRect(800, 1, 399, 399));
    mapper->setStyleSheet("background:rgb(200,200,200)");

    viewer = Viewer::getViewer(centralWidget);
    viewer->setObjectName(QStringLiteral("viewer"));
    viewer->setGeometry(QRect(800, 401, 399, 399));
    viewer->setStyleSheet("background:rgb(200,200,200)");

    logPrinter = LogPrinter::getLogPrinter(centralWidget);
    logPrinter->setObjectName(QStringLiteral("logPrinter"));
    logPrinter->setGeometry(QRect(1200, 1, 399, 399));
    logPrinter->setStyleSheet("background:rgb(255,255,255)");

    controller = Console::getController(centralWidget);
    controller->setObjectName(QStringLiteral("controller"));
    controller->setGeometry(QRect(1200, 401, 399, 399));
    controller->setStyleSheet("background:rgb(200,200,200)");

    this->setCentralWidget(centralWidget);
    this->setWindowTitle(QApplication::translate("MainWindow", "模拟雷达建图", Q_NULLPTR));

    //按名称连接插槽（作用？）
    QMetaObject::connectSlotsByName(this);

} // setupUi
