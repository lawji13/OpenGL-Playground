#include <assert.h>
#include <math.h>
#include <string.h>

#include "transform.h"

void transform_list_push(TransformList* list, float mat[MATRIX_SIZE])
{
    assert(list->size < TRANSFORM_CAPACITY);
    memcpy(list->transformations[list->size], mat, sizeof(float) * MATRIX_SIZE);
    ++list->size;
}

void transform_list_clear(TransformList* list)
{
    list->size = 0;
}

void rotate_cw_x(TransformList* list, float angle)
{
    assert(MATRIX_SIZE == 16);
    float rotate_z_mat[MATRIX_SIZE] = {
        1.0f,        0.0f,         0.0f, 0.0f, 
        0.0f, cosf(angle), -sinf(angle), 0.0f, 
        0.0f, sinf(angle),  cosf(angle), 0.0f, 
        0.0f,        0.0f,         0.0f, 1.0f};

    transform_list_push(list, rotate_z_mat);
}

void rotate_ccw_x(TransformList* list, float angle)
{
    rotate_cw_x(list, -1*angle);
}

void rotate_cw_y(TransformList* list, float angle)
{
    assert(MATRIX_SIZE == 16);
    float rotate_z_mat[MATRIX_SIZE] = {
         cosf(angle), 0.0f, sinf(angle), 0.0f, 
                0.0f, 1.0f,        0.0f, 0.0f, 
        -sinf(angle), 0.0f, cosf(angle), 0.0f, 
                0.0f, 0.0f,        0.0f, 1.0f};

    transform_list_push(list, rotate_z_mat);
}

void rotate_ccw_y(TransformList* list, float angle)
{
    rotate_cw_y(list, -1*angle);
}

void rotate_cw_z(TransformList* list, float angle)
{
    assert(MATRIX_SIZE == 16);
    float rotate_z_mat[MATRIX_SIZE] = {
        cosf(angle), -sinf(angle), 0.0f, 0.0f, 
        sinf(angle),  cosf(angle), 0.0f, 0.0f, 
        0.0f,                0.0f, 1.0f, 0.0f, 
        0.0f,                0.0f, 0.0f, 1.0f};

    transform_list_push(list, rotate_z_mat);
}

void rotate_ccw_z(TransformList* list, float angle)
{
    rotate_cw_z(list, -1*angle);
}

void scale(TransformList* list, float x, float y, float z)
{
    assert(MATRIX_SIZE == 16);
    float scale_mat[16] = {
        x, 0.0f, 0.0f, 0.0f, 
        0.0f, y, 0.0f, 0.0f, 
        0.0f, 0.0f, z, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f};

    transform_list_push(list, scale_mat);
}

void translate(TransformList* list, float x, float y, float z)
{
  assert(MATRIX_SIZE == 16);
    float translate_mat[16] = {
        1.0f, 0.0f, 0.0f, x, 
        0.0f, 1.0f, 0.0f, y, 
        0.0f, 0.0f, 1.0f, z, 
        0.0f, 0.0f, 0.0f, 1.0f};

    transform_list_push(list, translate_mat);  
}
