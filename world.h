#ifndef WORLD_H
#define WORLD_H

#include <QtCore>
#include "common.h"
#include "map.h"
#include "animal.h"

class World : public QObject
{
    Q_OBJECT
public:
    explicit World(QObject *parent = 0);
    ~World();
    //ObjectType checkObjectCoord(ObjectCoord c);
    //EyeData getEye(ObjectCoord oc, Direction dir);
    void addAnimal(Animal* ani, ObjectCoord coord_start = ObjectCoord(-1,-1));
    void addAnimal(QList<quint8> cmds,
                   QList<quint8> mems,
                   int cmd_start_ptr = 0,
                   int mem_start_ptr = 0,
                   ObjectCoord coord_start = ObjectCoord(-1,-1));
    QImage getImage();
    void makeStep();
    quint32 getAnimalCount() {return anis.size();}
    quint32 getBestAnimalID(){ if (best_animal == NULL) return 0; return best_animal->getID();}
    quint32 getBestAnimalFitness(){ return best_fitness; }
    void saveBestAnimal(QString filename="");
    void killAnimal(Animal* ani);
    void setSaveBestOnQuit(bool yes) { save_best_on_destroy = yes;}
    Animal* findBestLiveAnimal();
signals:
    void tick();

public slots:
    void onMove(Direction direction);
    void onEat(Direction direction);
    void onWait();
    void onSuicide();
    void onSplit(Direction direction);
    void onSplit_Mutate(Direction direction);
    void feedAnimal();
    void killWeakAnimals();
    /*void onMove(ObjectCoord obj, Direction direction);
    void onEat(ObjectCoord obj, Direction direction);
    void onWait(ObjectCoord obj);
    void onSuicide(ObjectCoord obj);
    void onSplit(ObjectCoord obj, Direction direction);
    void onSplit_Mutate(ObjectCoord obj, Direction direction);*/
protected:
    Map map;
    QList<Animal*> anis;
    Animal* best_animal;
    quint32 best_fitness;
    bool save_best_on_destroy;
    quint32 current_ID;
    Animal* findAnimalByCoord(ObjectCoord oc);
};

#endif // WORLD_H
