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
            push_back_vb(buff, (Vertex) {(Vec3) {x, y, z}, col, (Texture){.s = 0.0f, .t = 0.0f}});
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

void make_earth_geom(Vertex_Buffer* buff, Index_Buffer* ibuff)
{
    int sectors = 32;
    int stacks = 32;
    srand(time(0));

    float deltaTheta = (float) (2*M_PI)/sectors;
    float deltaPhi = (float) M_PI/stacks;
    float deltaS = 1.0f/(sectors/2);
    float deltaT = 1.0f/(stacks/2);

    for(int i = 0; i <= stacks/2; ++i)
    {
        float phi = (float) M_PI/2 - (i * deltaPhi);
        for(int j = 0; j <= sectors/2; ++j)
        {
            float theta = -1 * M_PI + (j * deltaTheta);
            float x = sinf(phi) * cosf(theta);
            float y = cosf(phi);
            float z = sinf(phi) * sinf(theta);

            float r = (double) rand() / RAND_MAX;
            float g = (double) rand() / RAND_MAX;
            float b = (double) rand() / RAND_MAX;
            Color col = (Color){.r = r, .g = g, .b = b};

            float s = j * deltaS;
            float t = 1 - (i * deltaT);
            Texture tex = (Texture){.s = s, .t = t};

            push_back_vb(buff, (Vertex) {(Vec3) {x, y, z}, col, tex});
        }
    }
    for(int i = 0; i < stacks/2; ++i)
    {
        for(int j = 0; j < sectors/2; ++j)
        {
            int temp = i * (sectors/2 + 1) + j;
            int temp2 = sectors/2 * (i + 1) + 1 + i + j;

            push_back_ib(ibuff, temp + 1);
            push_back_ib(ibuff, temp);
            push_back_ib(ibuff, temp2);

            push_back_ib(ibuff, temp + 1);
            push_back_ib(ibuff, temp2);
            push_back_ib(ibuff, temp2 + 1);
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
