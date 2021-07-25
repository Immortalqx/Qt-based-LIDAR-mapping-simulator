#ifndef LOGPRINTER_H
#define LOGPRINTER_H

#include <QWidget>
#include <QPainter>
#include <QTextBrowser>

//指令输出窗口
class LogPrinter : public QWidget
{
    Q_OBJECT
public:
    static LogPrinter *getLogPrinter(QWidget *parent = nullptr);

    void printLog(std::string c_str);

    void clear();

protected:
    void paintEvent(QPaintEvent *event);

private:
    static LogPrinter *ptr;

    QTextBrowser *textBrowser;

    explicit LogPrinter(QWidget *parent = nullptr);
};


#endif // LOGPRINTER_H
