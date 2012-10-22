#ifndef ANIMAL_H
#define ANIMAL_H

#include <QObject>
#include "common.h"
#include "map.h"

const quint32 MAX_STEPS = 100;

class Animal : public QObject
{
    Q_OBJECT
public:
//    explicit Animal(World *w, QObject *parent = 0): QObject(parent), world(w) {}
    explicit Animal(Map *mmap, QList<quint8> cmds,
                    QList<quint8> mems,
                    int cmd_start_ptr = 0,
                    int mem_start_ptr = 0,
                    QObject *parent = 0) { Animal(cmds,mems,cmd_start_ptr,mem_start_ptr,parent); map=mmap;}
    explicit Animal(QList<quint8> cmds,
                    QList<quint8> mems,
                    int cmd_start_ptr = 0,
                    int mem_start_ptr = 0,
                    QObject *parent = 0)
        : QObject(parent), cmd(cmds),
          mem(mems), cmd_ptr(cmd_start_ptr),
          mem_ptr(mem_start_ptr) { food = 750; fitness = 0; data = 0; searchStart(); map = NULL;}
    void setMap(Map* mmap) {map = mmap;}
//static
    static QList<quint8> compile(QList<AnimalCommand> acmd);
    static Animal* loadAnimal(QString filename);
    void saveAnimal(QString filename);
    static void saveAnimal(QString filename,
                           QList<quint8> cmds,
                           QList<quint8> mems,
                           int cmd_start_ptr = 0,
                           int mem_start_ptr = 0);
    Animal* cloneAnimal();
    quint32 getID() { return ID; }
    void setID(quint32 _ID) { ID = _ID;}
    void fitnessUp(quint32 up = 1) { fitness+=up; }
    quint32 getFitness() { return fitness;}
signals:
    void move(Direction direction);
    void eat(Direction direction);
    void wait();
    void suicide();
    void split(Direction direction);
    void splitMutate(Direction direction);//*/

public slots:
    void onTick();

    /*bool move(Direction direction);
    bool eat(Direction direction);
    bool wait() {return true;}
    bool suicide();
    bool split(Direction direction);
    bool splitMutate(Direction direction);*/
public:
    ObjectCoord coord;
    int food;

protected:
    quint32 ID;
    quint32 fitness;
    Map *map;
    QList<quint8> cmd;   // code
    QList<quint8> mem;            // HDD
    quint8 data;                  // RAM
    int cmd_ptr, //pointer to command cell
        mem_ptr; //pointer to memory cell

    void run();
    void searchStart();
    bool checkMemSize(int mem_size, bool expand);
};

#endif // ANIMAL_H
