#include "NPCShip.h"
#define _USE_MATH_DEFINES

NPCShip::NPCShip(Ui::MainWindow* CurrentUiPointer,QPoint SpawnCoordinates, QPoint FinishCoordinates, double SpeedMult)
{
    srand(time(NULL));
    StartPoint = SpawnCoordinates;
    MainFinishPoint = FinishCoordinates;
    ShipsID = rand() % 1000 + SpawnCoordinates.x();
    ClocksInfo.CurrentCheckPoint = SpawnCoordinates;
    ClocksInfo.CurrentVector.setShipsVector(0,0);
    SpeedMultipl  = SpeedMult;
    UiPointer = CurrentUiPointer;

    setNPCWay(SpawnCoordinates, FinishCoordinates);

    IsStillRunning = true;

    //визначаємо тип корабля та текстуру
    int someRandNum = rand() % 3;

    if(someRandNum == 0)
    {
        CurrentShipType = WAR_SHIP;
        ShipTexture = new QGraphicsPixmapItem(QPixmap(":/recource/WarShipTexture.png"));
        SpeedValue = 2.2;
        float eps = rand() % 3;
        if(rand() % 2 == 0)
        {
            eps = -eps;
        }
        SpeedValue += eps/10;
    }
    if(someRandNum == 1)
    {
        CurrentShipType = CARGO_SHIP;
        ShipTexture = new QGraphicsPixmapItem(QPixmap(":/recource/CargoShipTexture.png"));
        SpeedValue = 1.5;
        float eps = rand() % 3;
        if(rand() % 2 == 0)
        {
            eps = -eps;
        }
        SpeedValue += eps/10;
    }
    if(someRandNum == 2)
    {
        CurrentShipType = PASSENGER_SHIP;
        ShipTexture = new QGraphicsPixmapItem(QPixmap(":/recource/PassengerShipTexture.png"));
        SpeedValue = 1.8;
        float eps = rand() % 3;
        if(rand() % 2 == 0)
        {
            eps = -eps;
        }
        SpeedValue += eps/10;
    }
    SpeedValue *= SpeedMultipl;

    ShipsCoordinates = SpawnCoordinates;
    ShipTexture->setPos(SpawnCoordinates);

    ShipTexture->setFlag(QGraphicsItem::ItemIsSelectable, true);
    ShipTexture->setFlag(QGraphicsItem::ItemIsMovable, true);

    //малюємо "радар" - еліпс колізії
    QPen newPen;
    newPen.setColor(QColor(16,114,201)); //визначає колір контура круга ColisionElipse
    ColisionDiametr = 100;
    CollisionEllipse = new QGraphicsEllipseItem(
                ShipTexture->boundingRect().center().x() + ShipTexture->pos().x() - ColisionDiametr / 2,
                ShipTexture->boundingRect().center().y() + ShipTexture->pos().y() - ColisionDiametr / 2,
                ColisionDiametr,
                ColisionDiametr);
    CollisionEllipse->setVisible(false);
    CollisionEllipse->setOpacity(0.5);
    CollisionEllipse->setBrush(QColor("lightblue"));
    CollisionEllipse->setPen(newPen);

    AllShipsColisions.push_back(CollisionEllipse);

    addToGroup(CollisionEllipse);
    addToGroup(ShipTexture);

    ClocksInfo.AnimationClock = NULL;
    ClocksInfo.CollisionCheckingTimer = NULL;
    ClocksInfo.NewIterLauncher = NULL;

    QTimer* CheckingCollision = new QTimer;
    connect(CheckingCollision, SIGNAL(timeout()), this, SLOT(CollisionChecking()));
    ClocksInfo.CollisionCheckingTimer = CheckingCollision;
    CheckingCollision->start(20);
}

NPCShip::NPCShip(const NPCShip& Ship1)
{
    ShipTexture = Ship1.ShipTexture;
    ShipsWay = Ship1.ShipsWay;
    ShipsCheckPoints = Ship1.ShipsCheckPoints;
    ShipsID = Ship1.ShipsID;
    ShipsCoordinates = Ship1.ShipsCoordinates;
    CurrentShipType = Ship1.CurrentShipType;
    SpeedValue = Ship1.SpeedValue;
    SpeedMultipl = Ship1.SpeedMultipl;
}

NPCShip::~NPCShip()
{

}

void NPCShip::deleteObject()
{
    std::vector<QGraphicsEllipseItem*> NewVector;
    int currentIndex = 0;
    for(int i = 0; i < AllShipsColisions.size(); i++)
    {
        if(AllShipsColisions[i] == CollisionEllipse)
        {
            currentIndex = i;
        }
    }
    for(int i = 0; i < currentIndex; i++)
    {
        NewVector.push_back(AllShipsColisions[i]);
    }
    for(int i = currentIndex + 1; i < AllShipsColisions.size(); i++)
    {
        NewVector.push_back(AllShipsColisions[i]);
    }
    AllShipsColisions.clear();
    for(int i = 0; i < NewVector.size(); i++)
    {
        AllShipsColisions.push_back(NewVector[i]);
    }

    removeFromGroup(ShipTexture);
    removeFromGroup(CollisionEllipse);
    scene()->removeItem(ShipTexture);
    scene()->removeItem(CollisionEllipse);

    delete ShipTexture;
    delete CollisionEllipse;
    delete this;
}

void NPCShip::onAnimationFinished()
{
    sender()->deleteLater();
}

void NPCShip::CollisionChecking()
{
    QList<QGraphicsItem*> Items = ShipTexture->collidingItems();
    QList<NPCShip*> ShipsItems;
    QTimer* resumeChecking = new QTimer();
    int AnotherShipIndex = 0;

    foreach(QGraphicsItem* graphicsItem, Items)
    {
        NPCShip* newShip = dynamic_cast<NPCShip*>(graphicsItem);
        if(newShip)
        {
            ShipsItems.append(newShip);
        }
    }
    //об'єкт корабля завжди буде зіштовхуватися із іншим об'єктом - із власним CollisionElipse,
    //тому потрібно чекати колізії мінімум 2 об'єктів
    if(ShipsItems.size() == 2)
    {
        int vect_x_1 = 0, vect_y_1 = 0, vect_x_2 = 0, vect_y_2 = 0;
        ClocksInfo.CurrentVector.getShipsVector(&vect_x_1, &vect_y_1);

        if(ShipsItems[0]->ShipsID != ShipsID)
        {
            int x_value, y_value;
            AnotherShipIndex = 0;
            ShipsItems[0]->ClocksInfo.CurrentVector.getShipsVector(&x_value, &y_value);
            if(x_value != vect_x_1 && y_value != vect_y_1)
            {
                vect_x_2 = x_value;
                vect_y_2 = y_value;
            }
        }

        if(ShipsItems[1]->ShipsID != ShipsID)
        {
            int x_value, y_value;
            AnotherShipIndex = 1;
            ShipsItems[1]->ClocksInfo.CurrentVector.getShipsVector(&x_value, &y_value);
            if(x_value != vect_x_1 && y_value != vect_y_1)
            {
                vect_x_2 = x_value;
                vect_y_2 = y_value;
            }
        }

        qDebug() << "First ship:" << ShipsID << vect_x_1 << ";"<< vect_y_1;
        qDebug() << "Second ship:" << ShipsItems[AnotherShipIndex]->ShipsID <<vect_x_2 << ";"<< vect_y_2;

        if(vect_y_1 * vect_y_2 <= 0 && ShipsWay.size() > 0 && ShipsCheckPoints.size() > 0)  //якщо кораблі рухаються у різному напрямку відносно осі OY
        {
            std::vector<ShipsVector>::iterator ShipsWayIter = ShipsWay.begin(); //ітератор на перший елемент вектора ShipsWay
            std::vector<QPoint>::iterator CheckPointsIter = ShipsCheckPoints.begin();//ітератор на перший елемент веткора ShipsCheckPoints

            std::vector<ShipsVector> newShipsWay;
            std::vector<QPoint> newCheckPoints;

            QPoint newSecondPoint = *CheckPointsIter;
            ShipsVector firstVector(0,0), secondVector(0,0);

            QPointF CurrentLocation = pos() + StartPoint;
            QPointF NextPoint;
            vect_x_1 /= 2;
            vect_y_1 /= 2;

            ShipsVector NewVector1(0,0);
            ShipsVector NewVector2(0,0);

            if(vect_x_1 * vect_y_1 < 0) //ліве обертання
            {
                NewVector1.setShipsVector(vect_y_1, (-vect_x_1));
                NextPoint.setX(CurrentLocation.x() + vect_y_1);
                NextPoint.setY(CurrentLocation.y() + (-vect_x_1));
            }
            if(vect_x_1 * vect_y_1 > 0) //праве обертання
            {
                NewVector1.setShipsVector(-vect_y_1, vect_x_1);
                NextPoint.setX(CurrentLocation.x() + (-vect_y_1));
                NextPoint.setY(CurrentLocation.y() + vect_x_1);
            }

            int newvect_x, newvect_y;
            newvect_x = newSecondPoint.x() - NextPoint.x();
            newvect_y = newSecondPoint.y() - NextPoint.y();
            NewVector2.setShipsVector(newvect_x, newvect_y);
            ShipsWay.erase(ShipsWayIter);//видаляємо зайвий елемент у векторі векторів руху
            ShipsCheckPoints.erase(CheckPointsIter);//видаляємо зайвий елемент у векторі проміжних точок маршруту
            newShipsWay.push_back(NewVector1);
            newShipsWay.push_back(NewVector2);

            //редагуємо вектор векторів курсу ShipsWay та вектор проміжних точок ShipsCheckPoints
            QPoint NextPointCopy; //створюємо копію об'єкта NextPoint (просто не зміг перетворити тип QPointf в QPoint)
            NextPointCopy.setX(NextPoint.x());
            NextPointCopy.setY(NextPoint.y());
            newCheckPoints.push_back(NextPointCopy);
            newCheckPoints.push_back(newSecondPoint);
            for(int i = 0; i < ShipsWay.size(); i++)
            {
                newShipsWay.push_back(ShipsWay[i]);//формуємо копію вектора шляху разом із новими доповненими
            }
            for(int i = 0; i < ShipsCheckPoints.size(); i++)
            {
                newCheckPoints.push_back(ShipsCheckPoints[i]);
            }
            ShipsWay.clear();//повністю очищую вектор курсів
            ShipsCheckPoints.clear();
            for(int i = 0; i < newShipsWay.size(); i++)
            {
                ShipsWay.push_back(newShipsWay[i]);//знову заповнюю його новими елементами
            }
            for(int i = 0; i < newCheckPoints.size(); i++)
            {
                ShipsCheckPoints.push_back(newCheckPoints[i]);
            }

            //запуск анімації корабля - анімація запуститься після завершення попередньої
            ClocksInfo.CollisionCheckingTimer->stop();
            ClocksInfo.NewIterLauncher->setInterval(5);
            ClocksInfo.AnimationClock->stop();
            resumeChecking->singleShot(6000 / SpeedValue, this, SLOT(ResumeNewVectorAnimation()));
        }
        //наступні два випадки є абсолютно різними, хоча і виконують один і той же алгоритм

        //якщо кораблі рухаються у різних напрямках відносно осі Oy
        //якщо корабель прибуває до своєї точки призначення, у якій заспавнився новий
        //корабель, що прибуває, зупиняється і чекає, поки новий омине його
        if(vect_y_1 * vect_y_2 < 0 && ShipsWay.size() == 0)
        {
            if(StartPoint.y() + pos().y() > ShipsItems[AnotherShipIndex]->pos().y()
                    + ShipsItems[AnotherShipIndex]->StartPoint.y() && vect_y_1 < 0)
            {
                StopAnimation();
            }
            if(StartPoint.y() + pos().y() < ShipsItems[AnotherShipIndex]->pos().y()
                    + ShipsItems[AnotherShipIndex]->StartPoint.y() && vect_y_1 > 0)
            {
                StopAnimation();
            }
        }

        //якщо кораблі рухаються у однаковому напрямку відносно осі Oy
        //немає значення, чи корабель прибуває до точки призначення, чи ще має певні проміжні точки
        if(vect_y_1 * vect_y_2 > 0)
        {
            if(StartPoint.y() + pos().y() > ShipsItems[AnotherShipIndex]->pos().y()
                    + ShipsItems[AnotherShipIndex]->StartPoint.y() && vect_y_1 < 0)
            {
                StopAnimation();
            }
            if(StartPoint.y() + pos().y() < ShipsItems[AnotherShipIndex]->pos().y()
                    + ShipsItems[AnotherShipIndex]->StartPoint.y() && vect_y_1 > 0)
            {
                StopAnimation();
            }
        }
    }
}

void NPCShip::StopAnimation()
{
    int remainingNewIterLauncher = -1;

    ClocksInfo.AnimationClock->setPaused(true);

    ClocksInfo.CollisionCheckingTimer->stop();
    ClocksInfo.CollisionCheckingTimer->setInterval(10);

    remainingNewIterLauncher = ClocksInfo.NewIterLauncher->remainingTime();
    ClocksInfo.NewIterLauncher->stop();
    ClocksInfo.NewIterLauncher->setInterval(remainingNewIterLauncher);
    qDebug() << "Ship is stopped";

    QTimer * newTimer = new QTimer();
    newTimer->singleShot(6000 / SpeedValue, this, SLOT(ResumeAnimation())); //очікує певний час
}

void NPCShip::ResumeAnimation()
{
    ClocksInfo.AnimationClock->setPaused(false);
    ClocksInfo.CollisionCheckingTimer->setInterval(20);
    ClocksInfo.CollisionCheckingTimer->start();
    ClocksInfo.NewIterLauncher->start();
}

void NPCShip::ResumeNewVectorAnimation()
{
    ClocksInfo.CollisionCheckingTimer->setInterval(20);
    ClocksInfo.CollisionCheckingTimer->start();
}

void NPCShip::MoveShip()
{
    if(ShipsWay.size() > 0)//опрацювання анімації лише тоді, якщо вектор векторів курсу не порожній
    {
        if(ClocksInfo.NewIterLauncher)
        {
            ClocksInfo.NewIterLauncher->stop();
        }

        QPoint newCoordinates;
        std::vector<ShipsVector>::iterator iter = ShipsWay.begin(); //ітератор на перший елемент вектора ShipsWay
        std::vector<QPoint>::iterator iter1 = ShipsCheckPoints.begin();//ітератор на перший елемент веткора ShipsCheckPoints
        int vect_x = 0, vect_y = 0; //складові вектора курсу
        float Angle = 0; // кут між напрямком руху та меридіаном, який проходить через корабель
        float cos_value = 1; //косинус кута Angle
        int AnimationDuration = 0; //тривалість анімації руху корабля по одному відрізку ламаної

        ShipsWay[0].getShipsVector(&vect_x, &vect_y);
        //--------------------------------------------------------------------------
        //передавання даних про поточний курс корабля для функції обчислення колізії
        ClocksInfo.CurrentVector.setShipsVector(vect_x, vect_y);
        ClocksInfo.CurrentCheckPoint.setX(iter1->x());
        ClocksInfo.CurrentCheckPoint.setY(iter1->y());
        //--------------------------------------------------------------------------

        AnimationDuration = (int)(sqrt(pow(vect_x, 2) + pow(vect_y, 2)) / SpeedValue) * 100;
        ClocksInfo.AnimationClock = new QTimeLine(AnimationDuration, this);
        QGraphicsItemAnimation* newAnimation = new QGraphicsItemAnimation(ClocksInfo.AnimationClock);
        newAnimation->setItem(this);
        connect(ClocksInfo.AnimationClock, SIGNAL(finished()), SLOT(onAnimationFinished()));

        cos_value = (pow(vect_y, 2)) / (sqrt(pow(vect_x, 2) + pow(vect_y,2)) * vect_y);
        setTransformOriginPoint(this->boundingRect().center());//встановлюємо точку обертання моделі корабля на центр моделі

        if(vect_y == 0)
        {
            cos_value = 0;
        }

        if(vect_x < 0)
        {
            cos_value = -cos_value;
        }
        Angle = acos(cos_value) * 180.0 / M_PI;

        if(vect_x >= 0)
        {
            setRotation(-Angle);
        }
        if(vect_x < 0)
        {
            setRotation(-Angle + 180);
        }

        newCoordinates.setX(vect_x);
        newCoordinates.setY(vect_y);

        newAnimation->setPosAt(1.0, newCoordinates + pos());
        newAnimation->setTimeLine(ClocksInfo.AnimationClock);
        ClocksInfo.AnimationClock->start();//запуск анімації корабля

        ShipsCoordinates.setX(pos().x());
        ShipsCoordinates.setY(pos().y());
        ShipsWay.erase(iter);
        ShipsCheckPoints.erase(iter1);

        ClocksInfo.NewIterLauncher = new QTimer();
        connect(ClocksInfo.NewIterLauncher, SIGNAL(timeout()), this, SLOT(MoveShip()));
                                                                             //запуск цієї ж функції на виконання
                                                                             //через час AnimationDuration
        ClocksInfo.NewIterLauncher->start(AnimationDuration);
    }
    else
    {
        ClocksInfo.CollisionCheckingTimer->stop();
        IsStillRunning = false;
    }
}

void NPCShip::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if(ShipTexture->isUnderMouse() == true && event->buttons() == Qt::LeftButton)
    {
        int ShipsNumber = AllShipsColisions.size();
        for(int i = 0; i < ShipsNumber; i++)
        {
            if(AllShipsColisions[i] == NULL)
            {
                qDebug() << "NULL pointer here: " << i;
            }
        }
        for(int i = 0; i < ShipsNumber; i++)
        {
            if(AllShipsColisions[i]->isVisible() == true)
            {
                AllShipsColisions[i]->setVisible(false);
            }
        }
        CollisionEllipse->setVisible(true);

        UiPointer->label_3->setText(QString::number(ShipsID));
        UiPointer->label_4->setText("Military ship");

        if(CurrentShipType == CARGO_SHIP)
        {
            UiPointer->label_4->setText("Cargo ship");
        }
        if(CurrentShipType == PASSENGER_SHIP)
        {
            UiPointer->label_4->setText("Passenger ship");
        }
        UiPointer->label_6->setText("NPC ship");
        UiPointer->label_8->setText(QString::number(MainFinishPoint.x()) + ";" + QString::number(MainFinishPoint.y()));
        UiPointer->label_14->setText("-------------");
        UiPointer->label_14->setStyleSheet("QLabel { background-color : red; color : black; }");
        UiPointer->label_16->setText("-------------");
        UiPointer->label_17->setText("-------------");
    }
}

void NPCShip::setNPCWay(QPoint StartPosition, QPoint FinishPosition)
{
    int x_vector, y_vector;
    int VectorsNumber;
    x_vector = FinishPosition.x() - StartPosition.x();
    y_vector = FinishPosition.y() - StartPosition.y();
    ShipsVector WayLine(x_vector, y_vector);

    int number = StartPosition.x() * StartPosition.y() + FinishPosition.x() + FinishPosition.y();
    number /= 4;
    srand(time(NULL) + number); //встановлюємо сід генерації

    while(1)
    {
        VectorsNumber = rand() % 6; //кількість векторів шляху - від 5 до 6
        if(VectorsNumber > 4)
        {
            break;
        }
    }

    for(int i = 0; i < VectorsNumber; i++)
    {
        QPoint newCheckPoint;

        if(i < VectorsNumber - 1)
        {
            int xEps = 0, yEps = 0;

            while(1)
            {
                xEps = rand() % x_vector / VectorsNumber; //відхилення від прямолінійного курсу по х-координаті
                if(fabs(xEps) > 0)
                    break;
            }
            while(1)
            {
                yEps = rand() % y_vector / VectorsNumber; //відхидення від прямолінійного курсу по y-координаті
                if(fabs(yEps) > 0)
                    break;
            }

            //rand() приймає тільки невід'ємні значення, тому знак треба керувати вручну
            if(x_vector < 0)//відхилення змінює знак відносно x_vector
            {
                xEps = -xEps;
            }
            if(y_vector < 0)//відхилення змінює знак відносно y_vector
            {
                yEps = -yEps;
            }

            if(rand() % 2 == 0 && i != 0) //випадкове визначення знаку х-складової вектора
            {
                xEps = -xEps;
            }

            newCheckPoint.setX((x_vector / VectorsNumber) * i + xEps + StartPosition.x());
            newCheckPoint.setY((y_vector / VectorsNumber) * i + yEps + StartPosition.y());
        }

        if(i == VectorsNumber - 1)
        {
            newCheckPoint.setX(FinishPosition.x());
            newCheckPoint.setY(FinishPosition.y());
        }
        ShipsCheckPoints.push_back(newCheckPoint);
    }

    for(int i = 0; i < ShipsCheckPoints.size(); i++)
    {
        ShipsVector newVector(0,0);
        if(i == 0)
        {
            int newVector_x = ShipsCheckPoints[i].x() - StartPosition.x();
            int newVector_y = ShipsCheckPoints[i].y() - StartPosition.y();
            newVector.setShipsVector(newVector_x, newVector_y);
        }
        else
        {
            int newVector_x = ShipsCheckPoints[i].x() - ShipsCheckPoints[i - 1].x();
            int newVector_y = ShipsCheckPoints[i].y() - ShipsCheckPoints[i - 1].y();
            newVector.setShipsVector(newVector_x, newVector_y);
        }
        ShipsWay.push_back(newVector);
    }
}

void NPCShip::setSpeedMultipl(double newSpeedMultipl)
{
    SpeedValue /= SpeedMultipl; //повертаємо початкову швидкість
    SpeedMultipl = newSpeedMultipl;
    SpeedValue *= SpeedMultipl;
}

bool NPCShip::getIsStillRunningValue()
{
    return IsStillRunning;
}
