#ifndef GEOM_H
#define GEOM_H

#include <stddef.h>

#define CAPACITY 4096

typedef struct Float_Buffer
{
    float buffer[CAPACITY];
    size_t size;
}Float_Buffer;

typedef struct Vec3
{
    float x,y,z;
}Vec3;

typedef struct Color
{
    float r,g,b;
}Color;

typedef struct Vertex
{
    Vec3 pos;
    Color col;
}Vertex;

typedef struct Vertex_Buffer
{
    Vertex buffer[CAPACITY];
    size_t size;
}Vertex_Buffer;

typedef struct Index_Buffer
{
    int buffer[CAPACITY];
    size_t size;
} Index_Buffer;

void push_back_vb(struct Vertex_Buffer* buff, Vertex vertex);

void push_back_fb(struct Float_Buffer* buff, float f);

void push_back_ib(struct Index_Buffer* buff, int i);

void make_cube_geom_vb(Vertex_Buffer* buff, Index_Buffer* ibuff);

void make_sphere_geom(Vertex_Buffer* buff, Index_Buffer* ibuff, int sectors, int stacks);
    
void make_circle_geom(Float_Buffer* buff, Index_Buffer* ibuff, int segments);


#endif // GEOM_H
