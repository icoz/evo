#include "map.h"

Map::Map(QObject *parent) :
    QObject(parent)
{
    for (int i=0; i < 256; i++)
        for (int j=0; j < 256; j++){
            map[i][j] = quint8(none);
            if (i == j) map[i][j] = quint8(food);
        }
}

ObjectType Map::getType(int x, int y)
{
    if (coordIsValid(x,y))
        return (ObjectType)map[x][y];
    else
        return none;
}

ObjectType Map::getType(ObjectCoord oc)
{
    return getType(oc.x, oc.y);
}


bool Map::coordIsValid(ObjectCoord oc)
{
    if (oc.x < 0 || oc.x > 255) return false;
    if (oc.y < 0 || oc.y > 255) return false;
    return true;
}


quint8 Map::getDistance(ObjectCoord oc, Direction dir)
{
    quint8 dist=0;
    int x,y;
    x = oc.x;
    y = oc.y;
    do {
        switch (dir){
        case left: x--; break;
        case right: x++; break;
        case up: y++; break;
        case down: y--; break;
        }
        dist++;
        if (dist == 255) break;
    } while (getType(x,y) == none);
    return dist;
}

bool Map::coordIsValid(int x, int y)
{
    if (x < 0 || x > 255) return false;
    if (y < 0 || y > 255) return false;
    return true;
}
