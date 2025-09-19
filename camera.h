#ifndef CAMERA_H
#define CAMERA_H
#include "geom.h"
typedef struct Camera
{
    Vec3 position;
    Vec3 target;
    Vec3 up;
}Camera;


#endif // CAMERA_H
