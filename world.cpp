#include "world.h"



EyeData World::getEye(ObjectCoord oc, Direction dir)
{
    EyeData e;
    e.distance = map.getDistance(oc,dir);
    e.type = map.getType(oc.addDist(e.distance,dir));
    return e;
}
