#include "AbstractShip.h"
#include "shipsvector.h"
#ifndef NPCSHIP_H
#define NPCSHIP_H

struct ShipsClockInfo
{
  ShipsVector CurrentVector;
  QPoint CurrentCheckPoint;
  QTimer* CollisionCheckingTimer;
  QTimer* NewIterLauncher;
  QTimeLine* AnimationClock;
};

class NPCShip: public AbstractShip
{
private:

    Q_OBJECT

    std::vector<ShipsVector> ShipsWay; //вектор векторів маршруту корабля
    std::vector<QPoint> ShipsCheckPoints; //ключові точки маршруту корабля
    ShipsClockInfo ClocksInfo; //об'єкт структури із вказівниками на таймери анімацій
    QPoint MainFinishPoint;    //кінцева точка прибуття; не змінюється упродовж руху
    bool IsStillRunning;

public:
    NPCShip(Ui::MainWindow* CurrentUiPointer, QPoint SpawnCoordinates, QPoint FinishCoordinates, double SpeedMult);
    NPCShip(const NPCShip& Ship1);
    ~NPCShip();


    QPoint getCoordinates() {return ShipsCoordinates;}
    void setCoordinates(QPoint NewCoordinates) {ShipsCoordinates = NewCoordinates;}
    int getShipsID() {return ShipsID;}
    void setNPCWay(QPoint StartPos, QPoint FinishPos);
    void setSpeedMultipl(double newSpeedMultiplValue);

public slots:
    void onAnimationFinished();
    void CollisionChecking();
    void MoveShip();
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void StopAnimation();              //функція для призупинення анімації
    void ResumeAnimation();            //функція для відновлення анімації
    void ResumeNewVectorAnimation();   //функція для відновлення анімації після зміни маршруту
    bool getIsStillRunningValue();     //повертає значення isStillRunning
    void deleteObject();               //видалення об'єкта
};
#endif // NPCSHIP_H
