#ifndef USERSSHIP_H
#define USERSSHIP_H
#include "AbstractShip.h"
#include <QPainter>

class UsersShip: public AbstractShip
{
private:

    Q_OBJECT

    QTimeLine* CurrentAnimationClock; //вказівник на таймер поточної анімації
                               //використовується у функції MoveShip
    QPoint MainStartPoint;     //Головна початкова точка; використовується для визначення координат

public:
    UsersShip(Ui::MainWindow* CurrentUiPointer,QPoint SpawnCoordinates, double SpeedMultipl);
    UsersShip(const UsersShip& NewShip1);
    ~UsersShip();

    QPoint getCoordinates() {return ShipsCoordinates;}
    void setCoordinates(QPoint NewCoordinates) {ShipsCoordinates = NewCoordinates;}
    int getShipsID() {return ShipsID;}
    void SetFinishPoint(QPoint FinishPoint);
    void setSpeedMultipl(double newSpeedMultiplValue);

public slots:
    void MoveShip();
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void onAnimationFinished();
};
#endif // USERSSHIP_H
