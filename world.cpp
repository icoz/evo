#include "world.h"



/*EyeData World::getEye(ObjectCoord oc, Direction dir)
{
    EyeData e;
    e.distance = map.getDistance(oc,dir);
    e.type = map.getType(oc.addDist(e.distance,dir));
    return e;
}*/

void World::addAnimal(QList<char> cmds,
                   QList<char> mems,
                   int cmd_start_ptr,
                   int mem_start_ptr)
{
    Animal* ani = new Animal(&map,cmds,mems,cmd_start_ptr,mem_start_ptr,this);
    connect(ani,SIGNAL(move(Direction)),SLOT(onMove(Direction)));
    connect(this,SIGNAL(tick()),ani,SLOT(onTick()));
    ani->coord.x = qrand() % 256;
    ani->coord.y = qrand() % 256;
    map.createObj(ani->coord, otAnimal);
    anis.append(ani);
}

QImage World::getImage()
{
    return map.getImage();
}

void World::makeStep()
{
    emit tick();
}

void World::onMove(Direction direction)
{
    //qDebug("world::onmove(): class-sender = %s",sender()->metaObject()->className());
    if (QString("Animal").compare(sender()->metaObject()->className()) == 0){
        Animal* ani = (Animal*) sender();
        ObjectCoord oc = ani->coord;
        oc.addDist(1,direction);
        if (map.coordIsValid(oc)) oc = map.correctCoord(oc);
        switch (map.getType(oc)){
        case otNone:
        case otFood: //food will be destroyed
            map.moveObj(ani->coord, direction);
            ani->coord.addDist(1,direction);
            break;
        case otStone:
        case otAnimal:
            //You cannot go on Stone or Animal!
            //map.moveObj(ani->coord, direction);
            //ani->coord.addDist(1,direction);
            break;
        }
    }
}

void World::onEat(Direction direction)
{
}

void World::onWait()
{
}

void World::onSuicide()
{
}

void World::onSplit(Direction direction)
{
}

void World::onSplit_Mutate(Direction direction)
{
}
