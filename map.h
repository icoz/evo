#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QImage>
#include "common.h"

class Map : public QObject
{
    Q_OBJECT
public:
    explicit Map(QObject *parent = 0);
    ObjectType getType(int x, int y);
    ObjectType getType(ObjectCoord oc);
    ObjectType getTypeNear(ObjectCoord oc, Direction dir);
    bool coordIsValid(int x, int y);
    bool coordIsValid(ObjectCoord oc);
    ObjectCoord correctCoord(ObjectCoord oc);
    QImage& getImage();
    //char getDistance(ObjectCoord oc, Direction dir);
    quint8 getDistance(ObjectCoord oc, Direction dir);
    void addFood(quint32 count = 100);
signals:

public slots:
    void moveObj(ObjectCoord oc, Direction dir);
    void createObj(ObjectCoord oc, ObjectType ot);
    void deleteObj(ObjectCoord oc);

protected:
    char map[MAP_X_SIZE][MAP_Y_SIZE];

};

#endif // MAP_H
