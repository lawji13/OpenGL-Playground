#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include<sys/time.h>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "util.h"
#include "transform.h"
#include "render_window.h"

#define SEGMENTS 36
#define CAPACITY 4096
#define FPS 60
#define US_PER_FRAME 1*1000*1000/FPS
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define ASPECT_RATIO ((float) WINDOW_WIDTH/WINDOW_HEIGHT)
#define FOV M_PI/4
#define CUBE_COUNT 3

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

void push_back_ib(struct Index_Buffer* buff, int i)
{
    assert(buff->size < CAPACITY);

    buff->buffer[buff->size] = i;
    buff->size++;
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


void make_cube_geom(Float_Buffer* buff, Index_Buffer* ibuff)
{
    //top back left
    push_back_fb(buff, -0.25);
    push_back_fb(buff, 0.25);
    push_back_fb(buff, -0.25);

    //top back right
    push_back_fb(buff, 0.25);
    push_back_fb(buff, 0.25);
    push_back_fb(buff, -0.25);

    //bot back left
    push_back_fb(buff, -0.25);
    push_back_fb(buff, -0.25);
    push_back_fb(buff, -0.25);

    //bot back right
    push_back_fb(buff, 0.25);
    push_back_fb(buff, -0.25);
    push_back_fb(buff, -0.25);

    //top front left
    push_back_fb(buff, -0.25);
    push_back_fb(buff, 0.25);
    push_back_fb(buff, 0.25);

    //top front right
    push_back_fb(buff, 0.25);
    push_back_fb(buff, 0.25);
    push_back_fb(buff, 0.25);

    //bot front left
    push_back_fb(buff, -0.25);
    push_back_fb(buff, -0.25);
    push_back_fb(buff, 0.25);

    //bot front right
    push_back_fb(buff, 0.25);
    push_back_fb(buff, -0.25);
    push_back_fb(buff, 0.25);
/*
      Back Face    Front Face
    0 -------- 1  4 -------- 5
    |          |  |          |
    |          |  |          |
    |          |  |          |
    |          |  |          |
    2 -------- 3  6 -------- 7

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
    push_back_ib(ibuff, 3);
    push_back_ib(ibuff, 2);
    push_back_ib(ibuff, 6);
    push_back_ib(ibuff, 7);
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

void make_circle_geom(Float_Buffer* buff, Index_Buffer* ibuff)
{
    push_back_fb(buff, 0.0f); // center x
    push_back_fb(buff, 0.0f); // center y
    push_back_fb(buff, 0.5f); // center s
    push_back_fb(buff, 0.5f); // center t

    float deltaTheta = (2*M_PI)/SEGMENTS;
    for(int i=0; i<SEGMENTS; ++i)
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
    push_back_ib(ibuff, SEGMENTS);
    push_back_ib(ibuff, 0);
}

bool compile_shader(const char* shader_src, int type, unsigned int* shader_handle)
{
    *shader_handle = glCreateShader(type);
    glShaderSource(*shader_handle, 1, &shader_src, NULL);
    glCompileShader(*shader_handle);

    int success;
    char infoLog[512];
    glGetShaderiv(*shader_handle, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(*shader_handle, 512, NULL, infoLog);
        printf("ERROR::SHADER::%s::COMPILATION_FAILED\n%s", type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT", infoLog);
        return false;
    }

    return true;
}

int main()
{
    RenderWindow window = {0};
    render_window_init(&window, WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGl");
    
    Index_Buffer ibuff = {0};
    Vertex_Buffer vbuff = {0};
    make_cube_geom_vb(&vbuff, &ibuff);
    Vec3 cube_positions[CUBE_COUNT] = {
        (Vec3){ 0.70f,  0.5f, 40.0f},
        (Vec3){ -5.60f,  -0.5f, 10.0f},
        (Vec3){ 3.40f,  -8.5f, 4.0f},
    };
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }

    char *vertex_shader_src, *fragment_shader_src;
    if(!(vertex_shader_src = read_file("vertex.glsl")))
    {
        perror("Error reading vertex shader file");
        exit(0);
    }
    if(!(fragment_shader_src = read_file("fragment.glsl")))
    {
        perror("Error reading fragment shader file");
        exit(0);
    }

    unsigned int vs, fs;
    if(!compile_shader(vertex_shader_src, GL_VERTEX_SHADER, &vs))
    {
        perror("Error compiling vertex shader file");
        exit(0);
    }
    if(!compile_shader(fragment_shader_src, GL_FRAGMENT_SHADER, &fs))
    {
        perror("Error compiling fragment shader file");
        exit(0);
    }
    
    free(vertex_shader_src);
    free(fragment_shader_src);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
    }
    glDeleteShader(vs);
    glDeleteShader(fs);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vbuff.size, &vbuff, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibuff.buffer), ibuff.buffer, GL_STATIC_DRAW);

    //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);

    //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    //text
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(2);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
  
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load("smiley.jpg", &width, &height, &nrChannels, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture");
    }
    stbi_image_free(data);

    struct timeval start_time = {0};
    struct timeval end_time = {0};

    TransformList model = {0};
    
    TransformList view = {0};
    Vec3 eye_pos = (Vec3) {0.0f, 0.0f, -1.0f};
    Vec3 u = (Vec3) {1.0f, 0.0f, 0.0f};
    Vec3 v = (Vec3) {0.0f, 1.0f, 0.0f};
    Vec3 n = (Vec3) {0.0f, 0.0f, 1.0f};

    transform_list_push(&view, (float[16]) {
            u.x, u.y, u.z, -eye_pos.x,
            v.x, v.y, v.z, -eye_pos.y,
            n.x, n.y, n.z, -eye_pos.z,
               0.0f, 0.0f, 0.0f, 1.0f}
        );
    
    TransformList projection = {0};
    float near = 1.0f;
    float far = 10.0f;
    float diff = near - far;
    /* https://ogldev.org/www/tutorial12/tutorial12.html */
    transform_list_push(&projection, (float[16]) {
            (1/tanf(FOV/2))/ASPECT_RATIO, 0.0f, 0.0f, 0.0f,
            0.0f, 1/tanf(FOV/2), 0.0f, 0.0f,
            0.0f, 0.0f, (-far - near)/diff, 2.0f * far * near / diff,
            0.0f, 0.0f, 1.0f, 0.0f}
        );

    #define RPS .1
    float angle = 0;
    float delta_rotation = RPS/FPS * 2 * (float) M_PI;
    float eye = -10.0f;
    while (!render_window_should_close(&window))
    {
        gettimeofday(&start_time, NULL);
        eye += .1;
        angle = fmodf((angle + delta_rotation), 2 * M_PI);
        if(eye > 10.0){ eye = -10.0f;}
        render_window_process_input(&window);
        glClearColor(0.0f, 0.6f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        for(size_t i=0; i<CUBE_COUNT; ++i)
        {
            transform_list_clear(&model);
            transform_list_clear(&view);

            float xp,yp,zp;
            xp = cube_positions[i].x;
            yp = cube_positions[i].y;
            zp = cube_positions[i].z;
            translate(&model, xp/5.0f, yp/5.0f, zp/5.0f);
            /* scale(&model, 2.5, 2.5, 2.5); */
            rotate_cw_x(&model, angle);
            rotate_cw_y(&model, angle);

            Vec3 eye_pos = (Vec3) {0.0f, 0.0f, eye};
            Vec3 u = (Vec3) {1.0f, 0.0f, 0.0f};
            Vec3 v = (Vec3) {0.0f, 1.0f, 0.0f};
            Vec3 n = (Vec3) {0.0f, 0.0f, 1.0f};

            transform_list_push(&view, (float[16]) {
                    u.x, u.y, u.z, -eye_pos.x,
                    v.x, v.y, v.z, -eye_pos.y,
                    n.x, n.y, n.z, -eye_pos.z,
                    0.0f, 0.0f, 0.0f, 1.0f}
                );
      
            glUseProgram(shaderProgram);
            unsigned int model_loc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(model_loc, model.size, GL_TRUE, (float*) model.transformations[0]);
            unsigned int count_loc = glGetUniformLocation(shaderProgram, "model_count");
            glUniform1i(count_loc, model.size);

            unsigned int view_loc = glGetUniformLocation(shaderProgram, "view");
            glUniformMatrix4fv(view_loc, view.size, GL_TRUE, (float*) view.transformations[0]);

            unsigned int projection_loc = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(projection_loc, projection.size, GL_TRUE, (float*) projection.transformations[0]);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glDrawElements(GL_TRIANGLES, ibuff.size, GL_UNSIGNED_INT, 0);
        }
        glfwSwapBuffers(window.window);
        glfwPollEvents();

        gettimeofday(&end_time, NULL);
        long ellapsed_time_us = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);
        /* printf("ellapsed time in us = %ld\n", ellapsed_time_us); */
        if(ellapsed_time_us < US_PER_FRAME)
        {
            usleep(US_PER_FRAME - ellapsed_time_us);
        }
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    
    return 0;
}





