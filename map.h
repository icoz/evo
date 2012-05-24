#ifndef MAP_H
#define MAP_H

#include <QObject>
#include "common.h"

class Map : public QObject
{
    Q_OBJECT
public:
    explicit Map(QObject *parent = 0);
    ObjectType getType(int x, int y);
    ObjectType getType(ObjectCoord oc);
    char getDistance(ObjectCoord oc, Direction dir);
    bool coordIsValid(int x, int y);
    bool coordIsValid(ObjectCoord oc);
signals:

public slots:
    void moveObj(ObjectCoord oc, Direction dir);
    void createObj(ObjectCoord oc, ObjectType ot);
    void deleteObj(ObjectCoord oc);

protected:
    char map[256][256];

};

#endif // MAP_H
