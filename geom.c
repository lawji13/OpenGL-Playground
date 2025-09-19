#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#include "geom.h"

Vec3 vec3_cross(Vec3 a,  Vec3 b)
{
    return (Vec3) {.x = a.y * b.z - a.z * b.y,
                   .y = a.z * b.x - a.x * b.z,
                   .z = a.x * b.y - a.y * b.x,
    };
}

Vec3 vec3_normalize(Vec3 v)
{
    float mag = sqrt(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));
    return (Vec3) {.x = v.x/mag,
                   .y = v.y/mag,
                   .z = v.z/mag,
    };
}

void push_back_vb(struct Vertex_Buffer* buff, Vertex vertex)
{
    assert(buff->size < CAPACITY);

    buff->buffer[buff->size] = vertex;
    buff->size++;
}

void push_back_fb(struct Float_Buffer* buff, float f)
{
    assert(buff->size < CAPACITY);

    buff->buffer[buff->size] = f;
    buff->size++;
}


void push_back_ib(struct Index_Buffer* buff, int i)
{
    assert(buff->size < CAPACITY);

    buff->buffer[buff->size] = i;
    buff->size++;
}

void make_cube_geom_vb(Vertex_Buffer* buff, Index_Buffer* ibuff)
{
    //top back left
    push_back_vb(buff,(Vertex) {(Vec3) {-0.25,  0.25,  -0.25}, (Color) {1.0f, 0.0f, 0.0f}});

    //top back right
    push_back_vb(buff,(Vertex) {(Vec3) {0.25,  0.25,  -0.25}, (Color) {1.0f, 0.0f, 0.0f}});

    //bot back left
    push_back_vb(buff,(Vertex) {(Vec3) {-0.25,  -0.25,  -0.25}, (Color) {1.0f, 0.0f, 0.0f}});

    //bot back right
    push_back_vb(buff,(Vertex) {(Vec3) {0.25,  -0.25,  -0.25}, (Color) {1.0f, 0.0f, 0.0f}});

    //top front left
    push_back_vb(buff,(Vertex) {(Vec3) {-0.25,  0.25,  0.25}, (Color) {0.0f, 0.0f, 1.0f}});

    //top front right
    push_back_vb(buff,(Vertex) {(Vec3) {0.25,  0.25,  0.25}, (Color) {0.0f, 0.0f, 1.0f}});

    //bot front left
    push_back_vb(buff,(Vertex) {(Vec3) {-0.25,  -0.25,  0.25}, (Color) {0.0f, 0.0f, 1.0f}});

    //bot front right
    push_back_vb(buff,(Vertex) {(Vec3) {0.25,  -0.25,  0.25}, (Color) {0.0f, 0.0f, 1.0f}});
/*
  Back Face    Front Face
  0 -------- 1  4 -------- 5
  |          |  |          |
  |          |  |          |
  |          |  |          |
  |          |  |          |
  2 -------- 3  6 -------- 7

  Top Face       Bot Face  
  0 -------- 1   6 -------- 7
  |          |   |          |
  |          |   |          |
  |          |   |          |
  |          |   |          |
  4 -------- 5   2 -------- 3
*/
    //back square face
    push_back_ib(ibuff, 0);
    push_back_ib(ibuff, 1);
    push_back_ib(ibuff, 2);
    push_back_ib(ibuff, 2);
    push_back_ib(ibuff, 1);
    push_back_ib(ibuff, 3);

    //front square face
    push_back_ib(ibuff, 4);
    push_back_ib(ibuff, 6);
    push_back_ib(ibuff, 5);
    push_back_ib(ibuff, 6);
    push_back_ib(ibuff, 7);
    push_back_ib(ibuff, 5);

    //top square face
    push_back_ib(ibuff, 4);
    push_back_ib(ibuff, 1);
    push_back_ib(ibuff, 0);
    push_back_ib(ibuff, 4);
    push_back_ib(ibuff, 5);
    push_back_ib(ibuff, 1);

    //bot square face
    push_back_ib(ibuff, 6);
    push_back_ib(ibuff, 2);
    push_back_ib(ibuff, 3);
    push_back_ib(ibuff, 7);
    push_back_ib(ibuff, 6);
    push_back_ib(ibuff, 3);

    //right side face
    push_back_ib(ibuff, 5);
    push_back_ib(ibuff, 3);
    push_back_ib(ibuff, 1);
    push_back_ib(ibuff, 5);
    push_back_ib(ibuff, 7);
    push_back_ib(ibuff, 3);

    //left side face
    push_back_ib(ibuff, 0);
    push_back_ib(ibuff, 2);
    push_back_ib(ibuff, 4);
    push_back_ib(ibuff, 2);
    push_back_ib(ibuff, 6);
    push_back_ib(ibuff, 4);

}

void make_sphere_geom(Vertex_Buffer* buff, Index_Buffer* ibuff, int sectors, int stacks)
{
    assert(sectors > 0);
    assert(stacks > 0);
    srand(time(0));

    float deltaTheta = (float) (2*M_PI)/sectors;
    float deltaPhi = (float) M_PI/stacks;

    //Color col = (Color){.r = .18, .g = .18, .b = .18};
    
    for(int i = 0; i <= stacks; ++i)
    {
        float phi = i * deltaPhi;
        for(int j = 0; j < sectors; ++j)
        {
            float theta = j * deltaTheta;
            float x = sinf(phi) * cosf(theta);
            float y = cosf(phi);
            float z = sinf(phi) * sinf(theta);
            float r = (double) rand() / RAND_MAX;
            float g = (double) rand() / RAND_MAX;
            float b = (double) rand() / RAND_MAX;

            Color col = (Color){.r = r, .g = g, .b = b};
            push_back_vb(buff, (Vertex) {(Vec3) {x, y, z}, col});
        }
    }

    for(int i = 1; i <= stacks; ++i)
    {
        for(int j = 0; j < sectors; ++j)
        {
            push_back_ib(ibuff, stacks * i + j);
            push_back_ib(ibuff, stacks * (i - 1) + j );
            if (j == sectors - 1)
            {
                push_back_ib(ibuff, stacks * (i - 1));
                    
                push_back_ib(ibuff, stacks * i + j);
                push_back_ib(ibuff, stacks * (i - 1));
                push_back_ib(ibuff, stacks * i);
            }
            else
            {
                push_back_ib(ibuff, stacks * (i - 1) + j + 1);

                push_back_ib(ibuff, stacks * i + j);
                push_back_ib(ibuff, stacks * (i - 1) + j + 1);
                push_back_ib(ibuff, stacks * i + j + 1);
            }
        }
    }
}

void make_circle_geom(Float_Buffer* buff, Index_Buffer* ibuff, int segments)
{
    assert(segments > 0);
    
    push_back_fb(buff, 0.0f); // center x
    push_back_fb(buff, 0.0f); // center y
    push_back_fb(buff, 0.5f); // center s
    push_back_fb(buff, 0.5f); // center t

    float deltaTheta = (2*M_PI)/segments;
    for(int i=0; i<segments; ++i)
    {
        float theta = deltaTheta * i;
        float x = .3 * cos(theta);
        float y = .3 * sin(theta);
        float s = (-.5 * cos(theta) + 0.5f);
        float t = (-.5 * sin(theta) + 0.5f);

        push_back_fb(buff, x);
        push_back_fb(buff, y);
        push_back_fb(buff, s);
        push_back_fb(buff, t);
        push_back_ib(ibuff, (i+2));
        push_back_ib(ibuff, (i+1));
        push_back_ib(ibuff, 0);
    }

    push_back_ib(ibuff, 1);
    push_back_ib(ibuff, segments);
    push_back_ib(ibuff, 0);
}
