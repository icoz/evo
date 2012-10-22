#include "world.h"
#include <QApplication>


World::World(QObject *parent): QObject(parent), current_ID(1), best_animal(NULL), best_fitness(0)
{
}

World::~World()
{
    if (best_animal != NULL){
        qDebug(QString("Best animal (%1) stored with fitness (%2)").arg(best_animal->getID()).arg(best_fitness).toAscii().data());
        best_animal->saveAnimal(QString("best_id%1_fit%2.ani").arg(best_animal->getID()).arg(best_fitness));
        delete best_animal;
    }
}

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
        ani->setID(current_ID++);
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

void World::addAnimal(QList<quint8> cmds,
                   QList<quint8> mems,
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
    //qApp->processEvents();
    emit tick();
}

void World::onMove(Direction direction)
{
    //qDebug("world::onmove(): class-sender = %s",sender()->metaObject()->className());
    if (QString("Animal").compare(sender()->metaObject()->className()) == 0){
        Animal* ani = (Animal*) sender();
        if (ani == NULL) return;
        ani->food--;
        if (ani->food < 0){
            onSuicide();
            return;
        }
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
        if (ani == NULL) return;
        ani->food--;
        //suicide later, give chance to eat
        ObjectCoord oc = ani->coord;
        oc.addDist(1,direction);
        if (map.coordIsValid(oc)) oc = map.correctCoord(oc);
        switch (map.getType(oc)){
        case otFood: //food will be destroyed
            ani->fitnessUp();
            ani->food += 100;
            map.deleteObj(oc);
            //qDebug("eat! :)");
            break;
        case otAnimal:
            //qDebug("eat animal! :)");
            ani->fitnessUp(10);
            ani->food += 1000;
            //delete animal
            {
                Animal* ani2=findAnimalByCoord(oc);
                if (ani2 != NULL){
//                    anis.removeAll(ani2);
                    qDebug(QString("(%1) kill (%2)").arg(ani->getID()).arg(ani2->getID()).toAscii().data());
                    killAnimal(ani2);
                }
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
        if (ani->food < 0){
            onSuicide();
            return;
        }
    }
}

void World::onWait()
{
    if (QString("Animal").compare(sender()->metaObject()->className()) == 0){
        Animal* ani = (Animal*) sender();
        if (ani != NULL)
            ani->food--;
    }
}

void World::onSuicide()
{
    if (QString("Animal").compare(sender()->metaObject()->className()) == 0){
        Animal* ani = (Animal*) sender();
        if (ani != NULL){
            //TODO: add stats
            qDebug(QString("(%1) suicide with fitness(%2), food(%3)").arg(ani->getID()).arg(ani->getFitness()).arg(ani->food).toAscii().data());
            killAnimal(ani);
        }
    }
}

void World::onSplit(Direction direction)
{
    if (QString("Animal").compare(sender()->metaObject()->className()) == 0){
        Animal* ani = (Animal*) sender();
        if (ani == NULL) return;
        ani->food--;
        if (ani->food < 0){
            onSuicide();
            return;
        }
        ObjectCoord oc = ani->coord;
        oc.addDist(1,direction);
        if (map.coordIsValid(oc)) oc = map.correctCoord(oc);
        switch (map.getType(oc)){
        case otNone:
        case otFood: //food will be destroyed
            if (ani->food > 1000){
                map.deleteObj(oc);
                ani->fitnessUp(50);
                Animal *new_ani = ani->cloneAnimal();
                addAnimal(new_ani,oc);
                qDebug(QString("(%1) fitness(%3), food(%2) splitted to (%4)").arg(ani->getID()).arg(ani->food).arg(ani->getFitness()).arg(new_ani->getID()).toAscii().data());
                ani->food -= 1000;
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

void World::killAnimal(Animal *ani)
{
    if (ani != NULL){
        if (ani->getFitness() > best_fitness){ // store best animal
            if (best_animal != NULL) delete best_animal;
            best_animal = ani->cloneAnimal();
            best_animal->setID(ani->getID());
        }
        qDebug(QString("(%1) fitness(%2), food(%3) is dead").arg(ani->getID()).arg(ani->getFitness()).arg(ani->food).toAscii().data());
        map.deleteObj(ani->coord);  // clean map
        anis.removeAll(ani);        // remove ani from lives animals
        ani->disconnect(this);      // disconnect ani from world
        disconnect(this,SIGNAL(tick()),ani,SLOT(onTick()));
        delete ani;                 // kill ani
    }
}

void World::feedAnimal()
{
    map.addFood(qrand()%1000+100);
}
