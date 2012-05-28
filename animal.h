#ifndef ANIMAL_H
#define ANIMAL_H

#include <QObject>
#include "common.h"
#include "map.h"

class Animal : public QObject
{
    Q_OBJECT
public:
//    explicit Animal(World *w, QObject *parent = 0): QObject(parent), world(w) {}
    explicit Animal(Map *mmap, QList<char> cmds,
                    QList<char> mems,
                    int cmd_start_ptr = 0,
                    int mem_start_ptr = 0,
                    QObject *parent = 0)
        : QObject(parent), map(mmap), cmd(cmds),
          mem(mems), cmd_ptr(cmd_start_ptr),
          mem_ptr(mem_start_ptr) { data = 0; searchStart();}
    explicit Animal(QList<char> cmds,
                    QList<char> mems,
                    int cmd_start_ptr = 0,
                    int mem_start_ptr = 0,
                    QObject *parent = 0)
        : QObject(parent), cmd(cmds),
          mem(mems), cmd_ptr(cmd_start_ptr),
          mem_ptr(mem_start_ptr) { data = 0; searchStart(); map = NULL;}
    void setMap(Map* mmap) {map = mmap;}
//static
    static QList<char> compile(QList<AnimalCommand> acmd);
    static Animal* loadAnimal(QString filename);
    void saveAnimal(QString filename);
    static void saveAnimal(QString filename,
                           QList<char> cmds,
                           QList<char> mems,
                           int cmd_start_ptr = 0,
                           int mem_start_ptr = 0);

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
    Map *map;
    QList<char> cmd;   // code
    QList<char> mem;            // HDD
    char data;                  // RAM
    int cmd_ptr, //pointer to command cell
        mem_ptr; //pointer to memory cell

    void run();
    void searchStart();
};

#endif // ANIMAL_H
