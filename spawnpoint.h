#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H
#include <QPoint>
#include <QTimer>

class SpawnPoint: public QObject
{
    Q_OBJECT
private:
    QPoint ShipsSpawnPoint;
    double TimeAfterSpawn;
    QTimer* CountingTimer;
    bool IsFree;

public:
    SpawnPoint(int X_Value, int Y_Value);
    SpawnPoint(const SpawnPoint &newPoint);
    QPoint getSpawnPoint();
    void setSpawnPoint(int X_Value, int Y_Value);
    void ReFreshTimer();
    bool IsSpawnPointFree();
    ~SpawnPoint();

public slots:
    void CheckTime();
};

#endif // SPAWNPOINT_H
