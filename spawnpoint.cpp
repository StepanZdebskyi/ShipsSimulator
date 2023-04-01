#include "spawnpoint.h"
#include <iostream>

SpawnPoint::SpawnPoint(int X_Part, int Y_Part)
{
    IsFree =true;
    ShipsSpawnPoint.setX(X_Part);
    ShipsSpawnPoint.setY(Y_Part);
    TimeAfterSpawn = 0;
    CountingTimer = new QTimer();
    connect(CountingTimer, SIGNAL(timeout()), this, SLOT(CheckTime()));
    CountingTimer->start(100);
}

SpawnPoint::SpawnPoint(const SpawnPoint &newPoint)
{
    IsFree = newPoint.IsFree;
    ShipsSpawnPoint = newPoint.ShipsSpawnPoint;
    TimeAfterSpawn = newPoint.TimeAfterSpawn;
    CountingTimer = newPoint.CountingTimer;
}

void SpawnPoint::ReFreshTimer()
{
    IsFree = false;
    CountingTimer = new QTimer();
    connect(CountingTimer, SIGNAL(timeout()), this, SLOT(CheckTime()));
    CountingTimer->start(100);

}

void SpawnPoint::CheckTime()
{
    TimeAfterSpawn += 100;
    if(TimeAfterSpawn >= 5000)
    {
        TimeAfterSpawn = 0;
        IsFree = true;
        std::cout << "Time is out in 5000 ms\n";
        CountingTimer->stop();
    }
}

QPoint SpawnPoint::getSpawnPoint()
{
    return ShipsSpawnPoint;
}

void SpawnPoint::setSpawnPoint(int X_Value, int Y_Value)
{
    ShipsSpawnPoint.setX(X_Value);
    ShipsSpawnPoint.setY(Y_Value);
}

bool SpawnPoint::IsSpawnPointFree()
{
    return IsFree;
}

SpawnPoint::~SpawnPoint()
{

}
