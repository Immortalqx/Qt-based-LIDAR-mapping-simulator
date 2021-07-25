#include "include/simulator.h"
#include "include/logprinter.h"
#include "include/lidar.h"
#include "include/mapper.h"
#include "include/viewer.h"
#include <QStyleOption>

const double PI = 3.1415926535;

Simulator *Simulator::ptr = nullptr;

Simulator *Simulator::getSimulator(QWidget *parent)
{
    if (ptr == nullptr)
    {
        if (parent == nullptr)
            throw "ERROR:\tCreate Viewer by nullptr!";

        ptr = new Simulator(parent);
    }
    return ptr;
}

Simulator::Simulator(QWidget *parent) :
        QWidget(parent)
{

    carImage = QImage(":/car.png");
    carLabel = new QLabel(this);
    carLabel->setPixmap(QPixmap::fromImage(carImage));
    carLabel->setGeometry(QRect(0, 751, 48, 48));
    car = new Car(24, 775, -90, 24, 36);

    map = new RMap(16, 16, true);

    click = false;
}

void Simulator::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    QWidget::paintEvent(event);

    //绘制障碍物
    this->drawMap(painter);
}

void Simulator::drawMap(QPainter &painter)
{
    // 反走样
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 设置画笔颜色、宽度
    painter.setPen(QPen(QColor(0, 160, 230), 1));
    // 设置画刷颜色
    painter.setBrush(QColor(255, 160, 90));
    // 画出矩形
    for (int i = 0; i < map->getCol(); i++)
        for (int j = 0; j < map->getRow(); j++)
            if (map->query(i, j) == 1)
                painter.drawRect(i * 50, j * 50, 50, 50);

    //绘制起点和终点
    painter.setBrush(QColor(255, 0, 0));
    painter.drawRect(14 * 50, 0, 100, 100);
    painter.setBrush(QColor(0, 0, 255));
    painter.drawRect(0, 14 * 50, 100, 100);
}

void Simulator::keyPressEvent(QKeyEvent *event)
{
    bool couldPass = false;
    bool rightKey = false;
    QMatrix matrix;

    switch (event->key())
    {
        case Qt::Key_Up:
            couldPass = car->move_car(1, *map);
            rightKey = true;
            break;
        case Qt::Key_Down:
            couldPass = car->move_car(2, *map);
            rightKey = true;
            break;
        case Qt::Key_Right:
            couldPass = car->move_car(3, *map);
            rightKey = true;
            break;
        case Qt::Key_Left:
            couldPass = car->move_car(4, *map);
            rightKey = true;
            break;
        default:
            break;
    }
    if (!rightKey)
        return;

    if (couldPass)
    {
        carLabel->move(car->X() - 24, car->Y() - 24);//QLabel的坐标是矩形左上角坐标，需要转化一下

        matrix.rotate(car->Angle() + 90);//这个旋转的角度也是需要调整的
        carLabel->setPixmap(QPixmap::fromImage(carImage.transformed(matrix)));

        Viewer::getViewer()->updateViewer(car->getLidar()->getLidarMapTurn());
        Mapper::getMapper()->updateMapper(car->X(), car->Y(), car->getLidar()->getMap());

        LogPrinter::getLogPrinter()->printLog(
                "Car Move To: (" +
                std::to_string(car->X()) +
                ", " +
                std::to_string(car->Y()) +
                ")" +
                " Angle: " +
                std::to_string(int(car->Angle()))
        );
    }
    else
        LogPrinter::getLogPrinter()->printLog("Blocked!!!");
}

void Simulator::mousePressEvent(QMouseEvent *event)
{
    if (!click) return;

    switch (event->button())
    {
        case Qt::LeftButton:
            map->addIN(event->pos().y() / 50, event->pos().x() / 50, true);
            update();
            break;
        case Qt::RightButton:
            map->deleteOUT(event->pos().y() / 50, event->pos().x() / 50);
            update();
            break;
        default:
            break;
    }
}

void Simulator::mouseMoveEvent(QMouseEvent *event)
{
    //QTime time;
    //time.start();
    //while (time.elapsed() < 10);

    if (!click) return;

    if (event->buttons() & Qt::LeftButton)
    {
        map->addIN(event->pos().y() / 50, event->pos().x() / 50, true);
        update();
    }
    else if (event->buttons() & Qt::RightButton)
    {
        map->deleteOUT(event->pos().y() / 50, event->pos().x() / 50);
        update();
    }
}

void Simulator::clearMap()
{
    this->map->clear();
}

void Simulator::creatNewMap()
{
    this->map = new RMap(16, 16, true);
}

void Simulator::setMouseClick()
{
    if (click)
        click = false;
    else
        click = true;
}

void Simulator::reBuildMap()
{
    this->car->getLidar()->reBuildMap();
}

bool Simulator::saveMap(std::string filepath)
{
    if (filepath == "")
        return false;
    return map->saveRmap(filepath);
}

bool Simulator::loadMap(std::string filepath)
{
    if (filepath == "")
        return false;
    return map->loadRmap(filepath);
}
