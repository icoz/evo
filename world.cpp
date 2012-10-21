#include "world.h"
#include <QApplication>


/*EyeData World::getEye(ObjectCoord oc, Direction dir)
{
    EyeData e;
    e.distance = map.getDistance(oc,dir);
    e.type = map.getType(oc.addDist(e.distance,dir));
    return e;
}*/


void World::addAnimal(Animal *ani, ObjectCoord coord_start)
{
    if (ani != NULL){
        ani->setParent(this);
        ani->setMap(&map);
        connect(ani,SIGNAL(move(Direction)),SLOT(onMove(Direction)));
        connect(ani,SIGNAL(eat(Direction)),SLOT(onEat(Direction)));
        connect(ani,SIGNAL(suicide()),SLOT(onSuicide()));
        connect(ani,SIGNAL(wait()),SLOT(onWait()));
        connect(ani,SIGNAL(split(Direction)),SLOT(onSplit(Direction)));
        connect(ani,SIGNAL(splitMutate(Direction)),SLOT(onSplit_Mutate(Direction)));
    //    connect(ani,SIGNAL(),SLOT());
    //    connect(ani,SIGNAL(),SLOT());
        connect(this,SIGNAL(tick()),ani,SLOT(onTick()));
        if ((coord_start.x != -1) && (coord_start.y != -1)){
            ani->coord.x = coord_start.x;
            ani->coord.y = coord_start.y;
        }else{
            do {
                ani->coord.x = qrand() % MAP_X_SIZE;
                ani->coord.y = qrand() % MAP_Y_SIZE;
            } while (map.getType(ani->coord) == otAnimal);
        }
        map.createObj(ani->coord, otAnimal);
        anis.append(ani);
    }
}

void World::addAnimal(QList<char> cmds,
                   QList<char> mems,
                   int cmd_start_ptr,
                   int mem_start_ptr, ObjectCoord coord_start)
{
    Animal* ani = new Animal(&map,cmds,mems,cmd_start_ptr,mem_start_ptr,this);
    addAnimal(ani,coord_start);
}

QImage World::getImage()
{
    return map.getImage();
}

void World::makeStep()
{
    qApp->processEvents();
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
            break;
        }
    }
}

void World::onEat(Direction direction)
{
    if (QString("Animal").compare(sender()->metaObject()->className()) == 0){
        Animal* ani = (Animal*) sender();
        ObjectCoord oc = ani->coord;
        oc.addDist(1,direction);
        if (map.coordIsValid(oc)) oc = map.correctCoord(oc);
        switch (map.getType(oc)){
        case otFood: //food will be destroyed
            ani->food++;
            map.deleteObj(oc);
            //qDebug("eat! :)");
            break;
        case otAnimal:
            //qDebug("eat animal! :)");
            ani->food += 100;
            //delete animal
            {
                Animal* ani2=findAnimalByCoord(oc);
                if (ani2 != NULL)
                    anis.removeAll(ani2);
            }
            map.deleteObj(oc);
            break;
        case otNone:
        case otStone:
            //You cannot eat Stone!
            //map.moveObj(ani->coord, direction);
            //ani->coord.addDist(1,direction);
            break;
        }
    }
}

void World::onWait()
{
}

void World::onSuicide()
{
    if (QString("Animal").compare(sender()->metaObject()->className()) == 0){
        Animal* ani = (Animal*) sender();
        if (ani != NULL){
            map.deleteObj(ani->coord);
            anis.removeAll(ani);
            delete ani;
            qDebug("suicide :(");
        }
    }
}

void World::onSplit(Direction direction)
{
    if (QString("Animal").compare(sender()->metaObject()->className()) == 0){
        Animal* ani = (Animal*) sender();
        ObjectCoord oc = ani->coord;
        oc.addDist(1,direction);
        if (map.coordIsValid(oc)) oc = map.correctCoord(oc);
        switch (map.getType(oc)){
        case otNone:
        case otFood: //food will be destroyed
            map.deleteObj(oc);
            if (ani->food > 10){
                //addAnimal(ani->cmd, ani->mem);
                Animal *new_ani = ani->cloneAnimal();
                addAnimal(new_ani,oc);
                ani->food -= 10;
                qDebug("split! :)");
            }
            break;
        case otAnimal:
        case otStone:
            //You cannot split on Stone and Animal!
            break;
        }
    }
}

void World::onSplit_Mutate(Direction direction)
{
    Q_UNUSED(direction)
}

Animal *World::findAnimalByCoord(ObjectCoord oc)
{
    Animal* ani;
    foreach (ani, anis){
        if (ani->coord == oc) return ani;
    }
    return NULL;
}

