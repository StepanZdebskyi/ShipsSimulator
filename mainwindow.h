#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdlib.h>
#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include "spawnpoint.h"
#include "AbstractShip.h"
#include "NPCShip.h"
#include "UsersShip.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* newScene;
    QTimer* NPCMoveTimer;
    QTimer* CollisionCheckTimer;

    std::vector<NPCShip*> NPCShips; //вектор NPC - кораблів
    std::vector<UsersShip*> PlayableShips; //вектор кораблів користувача
    std::vector<SpawnPoint> MainCheckPoints; //перші 3 - верхні, другі 3 - нижні
    bool* IsMainCheckPointUsed;              //масив значень true/false позначає, які спавн-точки зайняті
                                             //також потрібен для вибору наступної точки
    bool IsSimulationStarted;                //визначає, чи користувач вже запустив симуляцію

private slots:
    void StartNPCMoving();
    void mousePressEvent(QMouseEvent* event);
    void SpawnExtraNPC();
    void SetCheckPoints(std::vector<SpawnPoint>* CheckPoints);
    void on_pushButton_clicked(); //кнопка для старту симуляції
    void on_pushButton_3_clicked();  // кнопка для зміни коефіцієнту швидкості руху кораблів
};
#endif // MAINWINDOW_H
