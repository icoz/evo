#ifndef COMMON_H
#define COMMON_H

enum Direction {left, right, up, down};

enum ObjectType {none = 0 , stone, food, animal};

struct EyeData{
    char distance;
    ObjectType type;
};

struct ObjectCoord {
    int x,y;
    ObjectCoord addDist(int distance, Direction dir){
        switch (dir){
        case left: x -= distance; break;
        case right: x += distance; break;
        case up: y += distance; break;
        case down: y -= distance; break;
        }
        return *this;
    }
};

#endif // COMMON_H
