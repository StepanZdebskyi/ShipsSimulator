#include "mainwindow.h"
#define MainCheckPointsNumber 6

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //формуємо сцену
    ui->setupUi(this);
    newScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(newScene);
    ui->graphicsView->setSceneRect(0,0,800,500);
    QGraphicsItem* newGraphicItem = new QGraphicsPixmapItem(QPixmap(":/recource/ShipsGameScene1.png"));
    newScene->addItem(newGraphicItem);
    for(int i = 0; i < 7; i++)
    {
        newScene->addLine((100* (i + 1)), 0, (100* (i + 1)), 500, QPen(QColor(49,142,247)));
    }
    for(int i = 0; i < 4; i++)
    {
        newScene->addLine(0, 100*(i + 1), 800, 100*(i + 1), QPen(QColor(49,142,247)));
    }
    ui->lineEdit->setText("1.0");
    IsSimulationStarted = false;
}

void MainWindow::on_pushButton_clicked() //Запуск симуляції
{
    std::string SpeedMultString = ui->lineEdit->text().toStdString();
    double CheckValue = 0;
    int PointCounter = 0;
    try
    {
       if(IsSimulationStarted)
       {
           throw(true);
       }

       for(int i = 0; i < SpeedMultString.size(); i++)
       {
           if(isdigit(SpeedMultString[i]) == 0)
           {
               if(i == 0 && SpeedMultString[i] == '-')
               {

               }
               else if(SpeedMultString[i] == '.' && i != 0)
               {
                   PointCounter++;
                   if(PointCounter > 1)
                   {
                       throw(1);
                   }
               }
               else
               {
                   throw(1);
               }
           }
       }
       CheckValue = atof(SpeedMultString.c_str());
       if(CheckValue <= 0)
       {
          throw(2);
       }
       if(CheckValue >= 6)
       {
           throw(3);
       }
    }

    catch(int value)
    {
        QString ErrorMessage = "";
        if(value == 3)
        {
            ErrorMessage += "Speed multiplication should be less than 6!\n";
        }
        if(value == 2)
        {
            ErrorMessage += "Speed multiplication should be positive number!\n";
        }
        if(value == 1)
        {
           ErrorMessage += "Speed multiplication can't contain letters!\n";
        }
        QMessageBox Attention;
        Attention.setText(ErrorMessage);
        ui->lineEdit->setText("1.0");
        Attention.exec();
        return;
    }
    catch(bool value)
    {
        QString ErrorMessage = "";
        if(value)
        {
            ErrorMessage += "Simulation is already started!\n";

        }
        QMessageBox Attention;
        Attention.setText(ErrorMessage);
        Attention.exec();
        return;
    }

    SetCheckPoints(&MainCheckPoints);
    int StartObjectsNum = ui->comboBox->currentIndex() + 1;
    srand(time(NULL));

    for(int i = 0; i < StartObjectsNum; i++)
    {
        int StartPointNumber, FinishPointNumber;
        while(1)
        {
            StartPointNumber = rand() % 6;
            FinishPointNumber = rand() % 6;
            if(fabs(FinishPointNumber - StartPointNumber) >= 3
                    && !IsMainCheckPointUsed[StartPointNumber])
            {
                IsMainCheckPointUsed[StartPointNumber] = true;
                break;
            }
        }
        NPCShip* newShip = new NPCShip(ui, MainCheckPoints[StartPointNumber].getSpawnPoint(), MainCheckPoints[FinishPointNumber].getSpawnPoint(), CheckValue);
        NPCShips.push_back(newShip);
        newScene->addItem(newShip);
    }

    QPoint UsersShipSpawnCoordinates(680, 80);
    UsersShip* newUserShip = new UsersShip(ui, UsersShipSpawnCoordinates, CheckValue);
    PlayableShips.push_back(newUserShip);
    newScene->addItem(newUserShip);

    NPCMoveTimer = new QTimer();
    NPCMoveTimer->singleShot(500, this, SLOT(StartNPCMoving()));
    CollisionCheckTimer = new QTimer;
    connect(CollisionCheckTimer, SIGNAL(timeout()), this, SLOT(SpawnExtraNPC()));
    CollisionCheckTimer->start(500);
    IsSimulationStarted = true;
}

void MainWindow::on_pushButton_3_clicked() //Зміна коефіцієнту швидкості
{
    std::string SpeedMultString = ui->lineEdit->text().toStdString();
    int PointCounter = 0;
    double CheckValue = 0;
    try
    {
       for(int i = 0; i < SpeedMultString.size(); i++)
       {
           if(isdigit(SpeedMultString[i]) == 0)
           {
               if(i == 0 && SpeedMultString[i] == '-')
               {

               }
               else if(SpeedMultString[i] == '.' && i != 0)
               {
                   PointCounter++;
                   if(PointCounter > 1)
                   {
                       throw(1);
                   }
               }
               else
               {
                   throw(1);
               }
           }
       }
       CheckValue = atof(SpeedMultString.c_str());
       if(CheckValue <= 0)
       {
          throw(2);
       }
       if(CheckValue >= 6)
       {
           throw(3);
       }
    }

    catch(int value)
    {
        QString ErrorMessage = "";
        if(value == 3)
        {
            ErrorMessage += "Speed multiplication should be less than 6!\n";
        }
        if(value == 2)
        {
            ErrorMessage += "Speed multiplication should be positive number!\n";
        }
        if(value == 1)
        {
           ErrorMessage += "Speed multiplication can't contain letters!\n";
        }
        QMessageBox Attention;
        Attention.setText(ErrorMessage);
        ui->lineEdit->setText("1.0");
        Attention.exec();
        return;
    }

    double newValue = CheckValue;
    for(int i = 0; i < NPCShips.size(); i++)
    {
        NPCShips[i]->setSpeedMultipl(newValue);
    }
    for(int i = 0; i < PlayableShips.size(); i++)
    {
        PlayableShips[i]->setSpeedMultipl(newValue);
    }
}

void MainWindow::SetCheckPoints(std::vector<SpawnPoint>* CheckPoints)
{
    SpawnPoint *newPoint = new SpawnPoint(40,-50);
    CheckPoints->push_back(*newPoint);
    newPoint = new SpawnPoint(300, -50);
    CheckPoints->push_back(*newPoint);
    newPoint = new SpawnPoint(560, -50);
    CheckPoints->push_back(*newPoint);

    newPoint = new SpawnPoint(200, 540);
    CheckPoints->push_back(*newPoint);
    newPoint = new SpawnPoint(460, 540);
    CheckPoints->push_back(*newPoint);
    newPoint = new SpawnPoint(720, 540);
    CheckPoints->push_back(*newPoint);

    IsMainCheckPointUsed = new bool[MainCheckPointsNumber];
    for(int i = 0; i < MainCheckPointsNumber; i++)
    {
        IsMainCheckPointUsed[i] = false;
    }
}

void MainWindow::StartNPCMoving()
{
    for(int i = 0; i < NPCShips.size(); i++)
    {
        NPCShips[i]->MoveShip();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SpawnExtraNPC()
{
    srand(time(NULL));
    std::string SpeedMultString = ui->lineEdit->text().toStdString();
    double CheckValue = 0;
    int PointCounter = 0;
    try
    {
       for(int i = 0; i < SpeedMultString.size(); i++)
       {
           if(isdigit(SpeedMultString[i]) == 0)
           {
               if(i == 0 && SpeedMultString[i] == '-')
               {

               }
               else if(SpeedMultString[i] == '.' && i != 0)
               {
                   PointCounter++;
                   if(PointCounter > 1)
                   {
                       throw(1);
                   }
               }
               else
               {
                   throw(1);
               }
           }
       }
       CheckValue = atof(SpeedMultString.c_str());
       if(CheckValue <= 0)
       {
          throw(2);
       }
       if(CheckValue >= 6)
       {
           throw(3);
       }
    }

    catch(int value)
    {
        CheckValue = 1.0;
    }

    for(int i = 0; i < MainCheckPoints.size(); i++)
    {
        if(MainCheckPoints[i].IsSpawnPointFree())
        {
            std::cout << "True!!\n";
        }
        else
        {
            std::cout << "False!!\n";
        }
    }

    for(int i = 0; i < NPCShips.size(); i++)
    {
        if(!NPCShips[i]->getIsStillRunningValue())
        {
            int rand_number1, rand_number2;
            while(1)
            {
                rand_number1 = rand() % 6;
                rand_number2 = rand() % 6;
                if( fabs(rand_number2 - rand_number1) >= 3
                        && MainCheckPoints[rand_number1].IsSpawnPointFree())
                {
                    break;
                }
            }

            qDebug() << "Some NPC is waiting to be deleted";
            NPCShips[i]->deleteObject();
            MainCheckPoints[rand_number1].ReFreshTimer();
            NPCShips[i] = new NPCShip(ui, MainCheckPoints[rand_number1].getSpawnPoint(), MainCheckPoints[rand_number2].getSpawnPoint(), CheckValue);
            IsMainCheckPointUsed[rand_number1] = true;
            newScene->addItem(NPCShips[i]);
            NPCShips[i]->MoveShip();
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if(event->buttons() == Qt::RightButton && PlayableShips.size() > 0)
    {
        //у даній функції ви використовуємо вказівник на об'єкт QMouseEvent, який позначає
        //подію у площині форми програми
        //тобто нам при обчисленнях точки на об'єкті (QGraphicsView) форми потрібно врахувати розміщення самого об'єкта
        QPoint FinishPoint(0,0);
        FinishPoint.setX(event->scenePosition().x() - 10);
        FinishPoint.setY(event->scenePosition().y() - 10);
        PlayableShips[0]->SetFinishPoint(FinishPoint);
        std::cout << "Current mouse pos:" << FinishPoint.x() << " " << FinishPoint.y() << std::endl;
        PlayableShips[0]->MoveShip();
    }
}

