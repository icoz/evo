#ifndef ANIMAL_H
#define ANIMAL_H

#include <QObject>
#include "common.h"
//#include "map.h"

enum AnimalCommand {
    //cmd group
    nop,            // no op
    move_cmd_left,  // move carret on cmd left
    move_cmd_right, // move carret on cmd right
    jump_to,        // next byte = signed char, how to move (+right, -left)
    jump_to_ifz,    // next byte = signed char, if data in memory = 0
    jump_to_ifnz,   // next byte = signed char, if data in memory != 0
    restart,        // set cmd_ptr = 0
    //work with sensors (set value of sensor to DATA)
    eye_up_distance,
    eye_up_type,
    eye_down_distance,
    eye_down_type,
    eye_left_distance,
    eye_left_type,
    eye_right_distance,
    eye_right_type,
    //mem group
    move_mem_left,  // move carret on cmd left
    move_mem_right, // move carret on cmd right
    save_to_mem,    // save DATA to current mem-cell
    load_from_mem,  // load to DATA from current mem-cell
    add_mem,        // DATA += curent-mem-cell
    sub_mem,        // DATA -= curent-mem-cell
    set_mem_ptr,    // set mem_ptr = DATA
    //data group
    data_clear,     // set DATA=0
    data_inc,       // DATA++
    data_dec,       // DATA--
    //action group
    action_move_left,
    action_move_right,
    action_move_up,
    action_move_down,
    action_eat_left,
    action_eat_right,
    action_eat_up,
    action_eat_down,
    action_wait,
    action_suicide,
    action_split,
    action_split_mutate
};

class Animal : public QObject
{
    Q_OBJECT
public:
//    explicit Animal(World *w, QObject *parent = 0): QObject(parent), world(w) {}
    explicit Animal(QList<char> cmds,
                    QList<char> mems,
                    int cmd_start_ptr = 0,
                    int mem_start_ptr = 0,
                    QObject *parent = 0)
        : QObject(parent), cmd(cmds), mem(mems), cmd_ptr(cmd_start_ptr), mem_ptr(mem_start_ptr) {}

signals:
    void move(Direction direction);
    void eat(Direction direction);
    void wait();
    void suicide();
    void split(Direction direction);
    void split_Mutate(Direction direction);//*/

public slots:
    void onTick();

    /*bool move(Direction direction);
    bool eat(Direction direction);
    bool wait() {return true;}
    bool suicide();
    bool split(Direction direction);
    bool splitMutate(Direction direction);*/
protected:
    //World *world;
//    ObjectCoord coord;
    QList<char> cmd;   // code
    QList<char> mem;            // HDD
    char data;                  // RAM
    int cmd_ptr, //pointer to command cell
        mem_ptr; //pointer to memory cell
};

#endif // ANIMAL_H
