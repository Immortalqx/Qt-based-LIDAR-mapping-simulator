#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>

//控制台
class Console : public QWidget
{
    Q_OBJECT
public:
    static Console *getController(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);

private:
    static Console *ptr;

    explicit Console(QWidget *parent = nullptr);

    QPushButton *creatMap;
    QPushButton *clearMap;
    QPushButton *reBuildMap;
    QPushButton *setMouse;
    QPushButton *clearLogs;
    QPushButton *exit;
    QPushButton *loadMap;
    QPushButton *saveBuildMap;
    QPushButton *setAutoMove;

    bool auto_move;
    bool mouse_click;

private slots:
    void on_exit_clicked();

    void on_creatMap_clicked();

    void on_clearMap_clicked();

    void on_setMouse_clicked();

    void on_saveBuildMap_clicked();

    void on_reBuildMap_clicked();

    void on_clearLogs_clicked();

    void on_loadMap_clicked();

    void on_setAutoMove_clicked();
};


#endif // CONTROLLER_H
