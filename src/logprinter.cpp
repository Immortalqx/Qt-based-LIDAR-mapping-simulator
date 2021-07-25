#include "include/logprinter.h"
#include <QDateTime>
#include <QStyleOption>

LogPrinter *LogPrinter::ptr = nullptr;

LogPrinter *LogPrinter::getLogPrinter(QWidget *parent)
{
    if (ptr == nullptr)
    {
        if (parent == nullptr)
            throw "ERROR:\tCreate LogPrinter by nullptr!";

        ptr = new LogPrinter(parent);
    }
    return ptr;
}

LogPrinter::LogPrinter(QWidget *parent) :
        QWidget(parent)
{
    textBrowser = new QTextBrowser(this);
    textBrowser->setObjectName(QStringLiteral("textBrowser"));
    textBrowser->setGeometry(QRect(1, 1, 397, 397));
}

void LogPrinter::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    QWidget::paintEvent(event);

}

void LogPrinter::printLog(std::string c_str)
{
    //获取当前时间并且加到要输出的日志前面
    QDateTime curDateTime = QDateTime::currentDateTime();
    QString q_str = "[" + curDateTime.toString("hh:mm:ss") + "]\t" + QString::fromStdString(c_str);

    this->textBrowser->append(q_str);
}

void LogPrinter::clear()
{
    this->textBrowser->clear();
}
