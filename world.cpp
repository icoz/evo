#include "world.h"



EyeData World::getEye(ObjectCoord oc, Direction dir)
{
    EyeData e;
    e.distance = map.getDistance(oc,dir);
    e.type = map.getType(oc.addDist(e.distance,dir));
    return e;
}

void World::addAnimal(QList<char> cmds,
                   QList<char> mems,
                   int cmd_start_ptr,
                   int mem_start_ptr)
{
    Animal ani(&map,cmds,mems,cmd_start_ptr,mem_start_ptr,this);
    connect(&ani,SIGNAL(move(Direction),SLOT());
    anis.append(ani);
}
