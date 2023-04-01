#ifndef ABSTRACTSHIP_H
#define ABSTRACTSHIP_H
#include <vector>
#include <math.h>
#include "ui_mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup>
#include <QGraphicsItemAnimation>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <iostream>
#include <QObject>
#include <QTimer>
#include <QTimeLine>
#include <QBrush>
#include <QPen>
#include <QList>
#include <QCursor>

enum ShipType //список типів корабля
{
  WAR_SHIP,
  CARGO_SHIP,
  PASSENGER_SHIP
};

class AbstractShip: public QObject, public QGraphicsItemGroup
{

protected:

    //елементи, які позначають взаємодію із кораблем
    QGraphicsPixmapItem* ShipTexture;//текстура корабля
    ShipType CurrentShipType;        //тип корабля


    int ShipsID;                     // ідентифікатор корабля
    double SpeedMultipl;             //множник швидкості корабля;
                                     //необхідний для зміни швидкості через інтерфейс
    float SpeedValue;                //швидкість корабля

    //елементи, які визначають переміщення на карті
    QPoint ShipsCoordinates;         //поточні координати
    QPoint StartPoint;               //координати старотової позиції
    QPoint FinishPoint;              //координати пункту призначення

    //елементи для взаємодії об'єктів
    QGraphicsEllipseItem* CollisionEllipse; //вказівник на об'єкт - круг колізії корабля
    int ColisionDiametr;                    //діаметр круга колізії
    static std::vector<QGraphicsEllipseItem*> AllShipsColisions; //вектор вказівників кіл-колізій усіх створених кораблів (лише для візуального відображення)
    Ui::MainWindow* UiPointer;//вказівник на поточний інтерфейс користувача програми

public:
    virtual QPoint getCoordinates() = 0;
    virtual void setCoordinates(QPoint NewCoordinates) = 0;
    virtual int getShipsID() = 0;
    virtual void setSpeedMultipl(double newSpeedMultipl) = 0;

public slots:
    virtual void MoveShip() = 0; //функція для запуску анімації переміщення корабля
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) = 0; //опрацьовування натискання кнопок миші
    virtual void onAnimationFinished() = 0;  //функція, яка завершує анімацію
};
#endif // ABSTRACTSHIP_H
