#ifndef WORLD_H
#define WORLD_H

#include <QObject>
#include "common.h"
#include "map.h"
#include "animal.h"

class World : public QObject
{
    Q_OBJECT
public:
    explicit World(QObject *parent = 0): QObject(parent) {}

    //ObjectType checkObjectCoord(ObjectCoord c);
    EyeData getEye(ObjectCoord oc, Direction dir);

signals:
    void tick();

public slots:
    void onMove(Direction direction);
    void onEat(Direction direction);
    void onWait();
    void onSuicide();
    void onSplit(Direction direction);
    void onSplit_Mutate(Direction direction);
    /*void onMove(ObjectCoord obj, Direction direction);
    void onEat(ObjectCoord obj, Direction direction);
    void onWait(ObjectCoord obj);
    void onSuicide(ObjectCoord obj);
    void onSplit(ObjectCoord obj, Direction direction);
    void onSplit_Mutate(ObjectCoord obj, Direction direction);*/
protected:
    Map map;
    QList<Animal*> anis;
};

#endif // WORLD_H
