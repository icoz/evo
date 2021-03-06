#include "map.h"
#include <QColor>

Map::Map(QObject *parent) :
    QObject(parent)
{
    //todo random seed in main
    //qsrand(1);
    for (int i=0; i < MAP_X_SIZE; i++)
        for (int j=0; j < MAP_Y_SIZE; j++){
            quint32 r = qrand();
            if (r % 50 == 0) map[i][j] = quint8(otStone);
            else if (r % 49 == 0) map[i][j] = quint8(otFood);
            else map[i][j] = quint8(otNone);
            //map[i][j] = quint8(otNone);
            //if (i == j) map[i][j] = quint8(otFood);
        }
}

ObjectType Map::getType(int x, int y)
{
    if (coordIsValid(x,y))
        return (ObjectType)map[x][y];
    else
        return otNone;
}

ObjectType Map::getType(ObjectCoord oc)
{
    return getType(oc.x, oc.y);
}

ObjectType Map::getTypeNear(ObjectCoord oc, Direction dir)
{
    return getType(oc.addDist(1,dir));
}


bool Map::coordIsValid(ObjectCoord oc)
{
    if (oc.x < 0 || oc.x > MAP_X_SIZE) return false;
    if (oc.y < 0 || oc.y > MAP_Y_SIZE) return false;
    return true;
}

ObjectCoord Map::correctCoord(ObjectCoord oc)
{
    ObjectCoord oc2 = oc;
    if (oc2.x < 0) oc2.x += MAP_X_SIZE;
    if (oc2.y < 0) oc2.y += MAP_Y_SIZE;
    oc2.x = oc2.x % MAP_X_SIZE;
    oc2.y = oc2.y % MAP_Y_SIZE;
    return oc2;
}

QImage& Map::getImage()
{
    static QRgb a[4] = {0xffffffff, 0xffff0000, 0xff00ff00 , 0xff0000ff};
    static QImage p(MAP_X_SIZE,MAP_Y_SIZE,QImage::Format_RGB32);
    static bool first = true;
    p = p.scaled(MAP_X_SIZE,MAP_Y_SIZE);
    for (int i=0; i<MAP_X_SIZE; ++i)
        for (int j=0; j<MAP_Y_SIZE; ++j){
            QRgb c = a[ObjectType(map[i][MAP_Y_SIZE - j])];
            if(c!=0xffff0000 || first){
                p.setPixel(i ,j ,c);
            }
        }
    first = false;
    p = p.scaled(2*MAP_X_SIZE,2*MAP_Y_SIZE);
    return p;
}

quint8 Map::getDistance(ObjectCoord oc, Direction dir)
{
    quint8 dist=0;
    int x,y;
    x = oc.x;
    y = oc.y;
    do {
        switch (dir){
        case Left: x--; break;
        case Right: x++; break;
        case Up: y++; break;
        case Down: y--; break;
        }
        dist++;
        if ((dir == Left || dir == Right) && (dist == MAP_X_SIZE-1)) break;
        if ((dir == Up || dir == Down) && (dist == MAP_Y_SIZE-1)) break;
    } while (getType(x,y) == otNone);
    return dist;
}

void Map::addFood(quint32 count)
{
    for (quint32 i=0; i < count; i++){
        int x,y;
        for (int cnt=0; cnt < 10; cnt++){
            x = qrand() % MAP_X_SIZE;
            y = qrand() % MAP_Y_SIZE;
            if (map[x][y] == quint8(otNone)){
                map[x][y] = quint8(otFood);
                break;
            }
        }
    }
}

void Map::moveObj(ObjectCoord oc, Direction dir)
{
    ObjectCoord oc2 = oc;
    oc2.addDist(1,dir);
    if (coordIsValid(oc2)) oc2 = correctCoord(oc2);
    map[oc2.x][oc2.y] = map[oc.x][oc.y];
    map[oc.x][oc.y] = otNone;
}

void Map::createObj(ObjectCoord oc, ObjectType ot)
{
    map[oc.x][oc.y] = ot;
}

void Map::deleteObj(ObjectCoord oc)
{
    map[oc.x][oc.y] = otNone;
}

bool Map::coordIsValid(int x, int y)
{
    if (x < 0 || x >= MAP_X_SIZE) return false;
    if (y < 0 || y >= MAP_Y_SIZE) return false;
    return true;
}
