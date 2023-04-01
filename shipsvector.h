#ifndef SHIPSVECTOR_H
#define SHIPSVECTOR_H

class ShipsVector
{
private:
    int XPart;
    int YPart;
public:
    ShipsVector() {XPart = 0; YPart = 0;}
    ShipsVector(int newX, int newY) {XPart = newX; YPart = newY;}
    ShipsVector(const ShipsVector& Vector1){XPart = Vector1.XPart; YPart = Vector1.YPart;}
    ~ShipsVector(){}
    void getShipsVector(int* X, int* Y) {*X = XPart; *Y = YPart;}
    void setShipsVector(int newX, int newY){XPart = newX; YPart = newY;}
};
#endif // SHIPSVECTOR_H
