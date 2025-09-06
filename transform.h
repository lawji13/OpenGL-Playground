#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <stddef.h>

#define TRANSFORM_CAPACITY 10
#define MATRIX_ROWS 4
#define MATRIX_COLS 4
#define MATRIX_SIZE MATRIX_ROWS * MATRIX_COLS
#define IDENTITY_MATRIX {\
        1.0f, 0.0f, 0.0f, 0.0f,\
        0.0f, 1.0f, 0.0f, 0.0f,\
        0.0f, 0.0f, 1.0f, 0.0f,\
        0.0f, 0.0f, 0.0f, 1.0f}

typedef struct TransformList
{
    float transformations[TRANSFORM_CAPACITY][MATRIX_SIZE];
    size_t size;
}TransformList;

void transform_list_push(TransformList*, float[MATRIX_SIZE]);
void transform_list_clear(TransformList*);
void rotate_cw_x(TransformList*, float);
void rotate_ccw_x(TransformList*, float);
void rotate_cw_y(TransformList*, float);
void rotate_ccw_y(TransformList*, float);
void rotate_cw_z(TransformList*, float);
void rotate_ccw_z(TransformList*, float);

void scale(TransformList*, float, float, float);

void translate(TransformList* list, float x, float y, float z);
#endif // TRANSFORM_H
