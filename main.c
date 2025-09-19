#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include<sys/time.h>

#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "util.h"
#include "transform.h"
#include "render_window.h"
#include "geom.h"
#include "camera.h"

#define SEGMENTS 36

#define FPS 60
#define US_PER_FRAME 1*1000*1000/FPS
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define ASPECT_RATIO ((float) WINDOW_WIDTH/WINDOW_HEIGHT)
#define FOV M_PI/4
#define CUBE_COUNT 3

RenderWindow window = {0};
Camera camera = {.position = (Vec3) {0.0f, 0.0f, -5.5f},
                 .up = (Vec3) {0.0f, 1.0f, 0.0f},
                 .target = (Vec3) {0.0f, 0.0f, 1.0f},
};

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

void move_eye_forward(void)
{
    camera.position = (Vec3) {camera.position.x, camera.position.y, camera.position.z + 0.1f};
}

void move_eye_backward(void)
{
    camera.position = (Vec3) {camera.position.x, camera.position.y, camera.position.z - 0.1f};
}

void move_eye_right(void)
{
    camera.position = (Vec3) {camera.position.x + 0.1, camera.position.y, camera.position.z};
}

void move_eye_left(void)
{
    camera.position = (Vec3) {camera.position.x - 0.1, camera.position.y, camera.position.z};
}

void mouse_click(void)
{
    double x, y;
    render_window_get_mouse_pos(&window, &x, &y);
    printf("mouse clicked at %f, %f\n", x, y);
}

int main()
{

    render_window_init(&window, WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGl");
    render_window_add_callback(&window, GLFW_KEY_W, &move_eye_forward);
    render_window_add_callback(&window, GLFW_KEY_S, &move_eye_backward);
    render_window_add_callback(&window, GLFW_KEY_D, &move_eye_right);
    render_window_add_callback(&window, GLFW_KEY_A, &move_eye_left);
    
    Index_Buffer ibuff = {0};
    Vertex_Buffer vbuff = {0};
    make_sphere_geom(&vbuff, &ibuff, 30, 30);

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
    TransformList projection = {0};
    
    float near = 0.0f;
    float far = 10.0f;
    float diff = near - far;
    double prev_mouse_x = WINDOW_WIDTH/2;
    double prev_mouse_y = WINDOW_HEIGHT/2;
    double curr_mouse_x, curr_mouse_y;
    float x_rot, y_rot = 0.0f;
    float x_total_rot, y_total_rot = 0.0f;
    float rotation_sensitivity = 2.0f;
    /* https://ogldev.org/www/tutorial12/tutorial12.html */
    transform_list_push(&projection, (float[16]) {
            (1/tanf(FOV/2))/ASPECT_RATIO, 0.0f, 0.0f, 0.0f,
            0.0f, 1/tanf(FOV/2), 0.0f, 0.0f,
            0.0f, 0.0f, (-far - near)/diff, 2.0f * far * near / diff,
            0.0f, 0.0f, 1.0f, 0.0f}
        );

    while (!render_window_should_close(&window))
    {
        gettimeofday(&start_time, NULL);
        render_window_process_input(&window);
        glClearColor(0.0f, 0.6f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        transform_list_clear(&model);
        transform_list_clear(&view);
        x_rot = 0.0f;
        y_rot = 0.0f;
        
        render_window_get_mouse_pos(&window, &curr_mouse_x, &curr_mouse_y);

        if(render_window_get_mouse_dragging(&window)) {
            x_rot = rotation_sensitivity * (float) ((curr_mouse_x - prev_mouse_x) / WINDOW_WIDTH);
            y_rot = rotation_sensitivity * (float) ((curr_mouse_y - prev_mouse_y) / WINDOW_HEIGHT);
        }
        x_total_rot += x_rot;
        y_total_rot += y_rot;
        rotate_ccw_y(&model, x_total_rot);
        rotate_ccw_x(&model, y_total_rot);

        transform_list_push(&view, (float[16]) {
                1.0f,0.0f,0.0f, -camera.position.x,
                0.0f,1.0f,0.0f, -camera.position.y,
                0.0f,0.0f,1.0f,-camera.position.z,
                0.0f, 0.0f, 0.0f, 1.0f});

            prev_mouse_x = curr_mouse_x;
            prev_mouse_y = curr_mouse_y;
        
            
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





