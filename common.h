#ifndef COMMON_H
#define COMMON_H

#define MAP_X_SIZE 256
#define MAP_Y_SIZE 256

enum Direction {Left, Right, Up, Down};

enum ObjectType {otNone = 0 , otStone, otFood, otAnimal};

struct EyeData{
    char distance;
    ObjectType type;
};

struct ObjectCoord {
    qint32 x,y;
    ObjectCoord(){x=0; y=0;}
    ObjectCoord(qint32 _x, quint32 _y){x=_x; y=_y;}
    ObjectCoord addDist(int distance, Direction dir){
        switch (dir){
        case Left: x -= distance; break;
        case Right: x += distance; break;
        case Up: y += distance; break;
        case Down: y -= distance; break;
        }
        if (x < 0) x += MAP_X_SIZE;
        if (y < 0) y += MAP_Y_SIZE;
        x %= MAP_X_SIZE;
        y %= MAP_Y_SIZE;
        //if (x > 255) x = 0;
        //if (y > 255) y = 0;
        //if (x ) x = 0;
        return *this;
    }
    bool operator==(ObjectCoord oc){
        return (x == oc.x && y == oc.y);
    }
    bool operator!=(ObjectCoord oc){
        return !(operator==(oc));
    }
};

//todo union: char/enum
enum AnimalCommand {
    //cmd group
    nop,            // no op
    move_cmd_left,  // move carret on cmd left
    move_cmd_right, // move carret on cmd right
    jump_to,        // next byte = signed char, how to move (+right, -left)
    jump_to_ifz,    // next byte = signed char, if data in memory = 0
    jump_to_ifnz,   // next byte = signed char, if data in memory != 0
    start,          // nop, but START marker
    restart,        // set cmd_ptr = 0
    end,            // look for next start
    //work with sensors (set value of sensor to DATA)
    eye_up_distance,
    eye_down_distance,
    eye_left_distance,
    eye_right_distance,
    /*eye_up_type,
    eye_down_type,
    eye_left_type,
    eye_right_type,*/
    //touch_* return type of object near animal
    touch_up,
    touch_down,
    touch_left,
    touch_right,
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

const int MAX_ANIMAL_COMMAND = int(action_split_mutate);

#endif // COMMON_H
