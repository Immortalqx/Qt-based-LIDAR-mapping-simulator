#include "include/console.h"
#include "include/logprinter.h"
#include "include/mapper.h"
#include "include/simulator.h"
#include <QApplication>
#include <QFileDialog>
#include <QStyleOption>

Console *Console::ptr = nullptr;

Console *Console::getController(QWidget *parent)
{
    if (ptr == nullptr)
    {
        if (parent == nullptr)
            throw "ERROR:\tCreate Controller by nullptr！";

        ptr = new Console(parent);
    }
    return ptr;
}

Console::Console(QWidget *parent) :
        QWidget(parent)
{
    creatMap = new QPushButton(this);
    creatMap->setObjectName(QStringLiteral("creatMap"));
    creatMap->setGeometry(QRect(10, 10, 120, 40));
    clearMap = new QPushButton(this);
    clearMap->setObjectName(QStringLiteral("clearMap"));
    clearMap->setGeometry(QRect(10, 70, 120, 40));
    reBuildMap = new QPushButton(this);
    reBuildMap->setObjectName(QStringLiteral("reBuildMap"));
    reBuildMap->setGeometry(QRect(140, 10, 120, 40));
    setMouse = new QPushButton(this);
    setMouse->setObjectName(QStringLiteral("setMouse"));
    setMouse->setGeometry(QRect(10, 130, 180, 40));
    clearLogs = new QPushButton(this);
    clearLogs->setObjectName(QStringLiteral("clearLogs"));
    clearLogs->setGeometry(QRect(270, 10, 120, 40));
    exit = new QPushButton(this);
    exit->setObjectName(QStringLiteral("exit"));
    exit->setGeometry(QRect(140, 190, 120, 40));
    loadMap = new QPushButton(this);
    loadMap->setObjectName(QStringLiteral("loadMap"));
    loadMap->setGeometry(QRect(270, 70, 120, 40));
    saveBuildMap = new QPushButton(this);
    saveBuildMap->setObjectName(QStringLiteral("saveBuildMap"));
    saveBuildMap->setGeometry(QRect(140, 70, 120, 40));
    setAutoMove = new QPushButton(this);
    setAutoMove->setObjectName(QStringLiteral("setAutoMove"));
    setAutoMove->setGeometry(QRect(210, 130, 180, 40));

    creatMap->setText(QApplication::translate("MainWindow", "创建随机地图", Q_NULLPTR));
    clearMap->setText(QApplication::translate("MainWindow", "清除地图", Q_NULLPTR));
    reBuildMap->setText(QApplication::translate("MainWindow", "重新建图", Q_NULLPTR));
    setMouse->setText(QApplication::translate("MainWindow", "开启鼠标添删障碍物", Q_NULLPTR));
    clearLogs->setText(QApplication::translate("MainWindow", "清除日志", Q_NULLPTR));
    exit->setText(QApplication::translate("MainWindow", "退出程序", Q_NULLPTR));
    loadMap->setText(QApplication::translate("MainWindow", "加载地图", Q_NULLPTR));
    saveBuildMap->setText(QApplication::translate("MainWindow", "保存地图", Q_NULLPTR));
    setAutoMove->setText(QApplication::translate("MainWindow", "开启小车自主运动", Q_NULLPTR));

    QMetaObject::connectSlotsByName(this);

    mouse_click = auto_move = false;
}

void Console::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    QWidget::paintEvent(event);

}

// 每个部分的功能最后会在这里串起来，可能要重构代码
void Console::on_exit_clicked()
{
    qApp->exit(0);
}

void Console::on_creatMap_clicked()
{
    Simulator::getSimulator()->creatNewMap();
    Simulator::getSimulator()->reBuildMap();
    Simulator::getSimulator()->update();
    Mapper::getMapper()->update();
    LogPrinter::getLogPrinter()->printLog("成功创建随机地图！");
}

void Console::on_clearMap_clicked()
{
    Simulator::getSimulator()->clearMap();
    Simulator::getSimulator()->reBuildMap();
    Simulator::getSimulator()->update();
    Mapper::getMapper()->update();
    LogPrinter::getLogPrinter()->printLog("成功清空地图！");
}

void Console::on_setMouse_clicked()
{
    if (!mouse_click)
    {
        setMouse->setText(QApplication::translate("MainWindow", "关闭鼠标添删障碍物", Q_NULLPTR));
        LogPrinter::getLogPrinter()->printLog("开启鼠标添删障碍物！");
    }
    else
    {
        setMouse->setText(QApplication::translate("MainWindow", "开启鼠标添删障碍物", Q_NULLPTR));
        LogPrinter::getLogPrinter()->printLog("关闭鼠标添删障碍物！");
    }
    mouse_click = !mouse_click;
    Simulator::getSimulator()->setMouseClick();
}

void Console::on_saveBuildMap_clicked()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, tr("Save Map Files"),
                                            "", tr("Map Files (*.map);; All Files (*.*)"));
    if (!fileName.isNull())
    {
        LogPrinter::getLogPrinter()->printLog("保存到文件：" + fileName.toStdString());
    }
    else
    {
        LogPrinter::getLogPrinter()->printLog("已取消保存地图！");
        return;

    }
    if (Simulator::getSimulator()->saveMap(fileName.toStdString()))
        LogPrinter::getLogPrinter()->printLog("地图保存成功！");
    else
        LogPrinter::getLogPrinter()->printLog("地图保存失败！");
}

void Console::on_reBuildMap_clicked()
{
    Simulator::getSimulator()->reBuildMap();
    Mapper::getMapper()->update();
    LogPrinter::getLogPrinter()->printLog("开始重新建图！");
}

void Console::on_clearLogs_clicked()
{
    LogPrinter::getLogPrinter()->clear();
    LogPrinter::getLogPrinter()->printLog("Logs are Cleared！");
}

void Console::on_loadMap_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Open Map Files"),
                                            "", tr("Map Files (*.map);; All Files (*.*)"));

    if (!fileName.isNull())//fileName即是选择的文件名
    {
        LogPrinter::getLogPrinter()->printLog("打开地图文件：" + fileName.toStdString());
    }
    else //如果点击取消按钮或没有选择文件
    {
        LogPrinter::getLogPrinter()->printLog("已取消加载地图");
        return;

    }

    if (Simulator::getSimulator()->loadMap(fileName.toStdString()))
    {
        LogPrinter::getLogPrinter()->printLog("地图已加载！");
        Simulator::getSimulator()->reBuildMap();
        Simulator::getSimulator()->update();
        Mapper::getMapper()->update();
    }
    else
        LogPrinter::getLogPrinter()->printLog("地图加载失败！");
}

void Console::on_setAutoMove_clicked()
{
    if (!auto_move)
        setAutoMove->setText(QApplication::translate("MainWindow", "关闭小车自主运动", Q_NULLPTR));
    else
        setAutoMove->setText(QApplication::translate("MainWindow", "开启小车自主运动", Q_NULLPTR));

    auto_move = !auto_move;

//    LogPrinter::getLogPrinter()->printLog("Auto Move Start!");
    LogPrinter::getLogPrinter()->printLog("功能暂时未完成！");
}
