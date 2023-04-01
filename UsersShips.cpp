#include "UsersShip.h"

UsersShip::UsersShip(Ui::MainWindow* CurrentUiPointer,QPoint SpawnCoordinates, double SpeedMult)
{
    srand(time(NULL));
    ShipTexture = new QGraphicsPixmapItem(QPixmap(":/recource/WarShipTexture.png"));
    ShipsCoordinates = SpawnCoordinates;
    ShipTexture->setPos(SpawnCoordinates);
    MainStartPoint = SpawnCoordinates;
    StartPoint = SpawnCoordinates;
    std::cout << "Current multipl:" << SpeedMultipl << "\n";
    CurrentShipType = WAR_SHIP;
    ShipTexture->setFlag(QGraphicsItem::ItemIsSelectable, true);
    ShipTexture->setFlag(QGraphicsItem::ItemIsMovable, true);
    UiPointer = CurrentUiPointer;
    SpeedMultipl = SpeedMult;
    SpeedValue = 2 * SpeedMultipl;

    while(1)//генеруємо ID корабля - чотиризначне число
    {
        ShipsID = rand() % 9999;
        if(ShipsID >= 1000)
        {
            break;
        }
    }

    CurrentAnimationClock = NULL; //NULL означає, що корабель ще не мав жодної анімації руху

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
}

UsersShip::UsersShip(const UsersShip& Ship1)
{
    ShipTexture = Ship1.ShipTexture;
    ShipsID = Ship1.ShipsID;
    ShipsCoordinates = Ship1.ShipsCoordinates;
    CurrentShipType = Ship1.CurrentShipType;
    SpeedValue = Ship1.SpeedValue;
}

UsersShip::~UsersShip()
{

}

void UsersShip::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if(ShipTexture->isUnderMouse() == true && event->buttons() == Qt::LeftButton)
    {
        int ShipsNumber = AllShipsColisions.size();
        for(int i = 0; i < ShipsNumber; i++)
        {
            if(AllShipsColisions[i]->isVisible() == true)
            {
                AllShipsColisions[i]->setVisible(false);
            }
        }

        QString ControlCode;
        ControlCode += "Green_" + QString::number(ShipsID);

        CollisionEllipse->setVisible(true);
        UiPointer->label_3->setText(QString::number(ShipsID));
        UiPointer->label_4->setText("Military ship");
        UiPointer->label_6->setText("User's ship");
        UiPointer->label_8->setText("--------------");
        UiPointer->label_14->setText(ControlCode);
        UiPointer->label_14->setStyleSheet("QLabel { background-color : lightgreen; color : green; }");
        UiPointer->label_16->setText("X:" + QString::number(FinishPoint.x()));
        UiPointer->label_17->setText("Y:" + QString::number(FinishPoint.y()));
    }
}

void UsersShip::SetFinishPoint(QPoint newPoint)
{
    FinishPoint = newPoint;
}

void UsersShip::onAnimationFinished()
{
    sender()->deleteLater();
    CurrentAnimationClock = NULL;
}

void UsersShip::setSpeedMultipl(double newSpeedMultipl)
{
    SpeedValue /= SpeedMultipl;  //повертаємо початкову швидкість
    SpeedMultipl = newSpeedMultipl;
    SpeedValue *= SpeedMultipl;
}

void UsersShip::MoveShip()
{
    if(CurrentAnimationClock != NULL)
    {
        CurrentAnimationClock->stop();
        StartPoint.setX(pos().x() + MainStartPoint.x());
        StartPoint.setY(pos().y() + MainStartPoint.y());
    }
    qDebug() << "Start position:" << StartPoint.x() << " " << StartPoint.y();
    qDebug() << "Finish Position:" << FinishPoint.x() << " " << FinishPoint.y();
    UiPointer->label_16->setText("X:" + QString::number(FinishPoint.x()));
    UiPointer->label_17->setText("Y:" + QString::number(FinishPoint.y()));

    int vect_x = 0, vect_y = 0; //складові вектора курсу
    float Angle = 0; // кут між напрямком руху та меридіаном, який проходить через корабель
    float cos_value = 1; //косинус кута Angle
    int AnimationDuration = 0; //тривалість анімації руху корабля по одному відрізку ламаної
    vect_x = FinishPoint.x() - StartPoint.x();
    vect_y =  FinishPoint.y() - StartPoint.y();

    AnimationDuration = (int)(sqrt(pow(vect_x, 2) + pow(vect_y, 2)) / SpeedValue) * 100;
    QTimeLine* ShipsAnimationClock = new QTimeLine(AnimationDuration, this);
    QGraphicsItemAnimation* ShipsAnimation = new QGraphicsItemAnimation(ShipsAnimationClock);
    ShipsAnimation->setItem(this);
    connect(ShipsAnimationClock, SIGNAL(finished()), SLOT(onAnimationFinished()));

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

    QPoint newCoordinates;

    newCoordinates.setX(vect_x);
    newCoordinates.setY(vect_y);

    ShipsAnimation->setPosAt(1.0, newCoordinates + pos());
    ShipsAnimation->setTimeLine(ShipsAnimationClock);
    ShipsAnimationClock->start();//запуск анімації корабля

    std::cout << "Current position:" << pos().x() << " " << pos().y() << std::endl;

    CurrentAnimationClock = ShipsAnimationClock;
    StartPoint = FinishPoint;
}
