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
    ObjectCoord addDist(int distance, Direction dir){
        switch (dir){
        case Left: x -= distance; break;
        case Right: x += distance; break;
        case Up: y += distance; break;
        case Down: y -= distance; break;
        }
        x %= 256;
        y %= 256;
        //if (x > 255) x = 0;
        //if (y > 255) y = 0;
        //if (x ) x = 0;
        return *this;
    }
};

#endif // COMMON_H
