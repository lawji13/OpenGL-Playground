#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include<sys/time.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SEGMENTS 36
#define CAPACITY 4096
#define FPS 60
#define US_PER_FRAME 1*1000*1000/FPS

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

typedef struct Float_Buffer
{
    float buffer[CAPACITY];
    size_t size;
}Float_Buffer;

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

void push_back_fb(struct Float_Buffer* buff, float f)
{
    assert(buff->size < CAPACITY);

    buff->buffer[buff->size] = f;
    buff->size++;
}

bool read_file(const char* file_path, char** file_contents)
{
    FILE* file;
    if ((file = fopen(file_path, "r")) == NULL)
    {
        perror("Error reading file");
        return false;
    }

    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file); 
    fseek(file, 0, SEEK_SET);

    *file_contents = (char *)malloc(fileSize + 1);

    if (*file_contents == NULL)
    {
        perror("Error allocating memory");
        fclose(file);
        return false;
    }

    size_t bytesRead = fread(*file_contents, 1, fileSize, file);
    (*file_contents)[bytesRead] = '\0';

    fclose(file);
    return true;;
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
    Float_Buffer fbuff = {0};
    Index_Buffer ibuff = {0};
    make_circle_geom(&fbuff, &ibuff);
   
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }


    int success;
    char infoLog[512];

    unsigned int vs, fs;
    char* vertex_shader_src;
    char* fragment_shader_src;

    if (!read_file("vertex.glsl", &vertex_shader_src) || !read_file("fragment.glsl", &fragment_shader_src)) {perror("Error reading shader file"); exit(0);}
    if(!compile_shader(vertex_shader_src, GL_VERTEX_SHADER, &vs) || !compile_shader(fragment_shader_src, GL_FRAGMENT_SHADER, &fs)) {perror("Error compiling shader file"); exit(0);}
    free(vertex_shader_src);
    free(fragment_shader_src);
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
    }
    glDeleteShader(vs);
    glDeleteShader(fs);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * fbuff.size, &fbuff, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibuff.buffer), ibuff.buffer, GL_STATIC_DRAW);

    //pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);

    //text
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load("smiley.jpg", &width, &height, &nrChannels, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture");
    }
    stbi_image_free(data);

    float scale_mat[16] = {
        2.5f, 0.0f, 0.0f, 0.0f, 
        0.0f, 2.5f, 0.0f, 0.0f, 
        0.0f, 0.0f, 2.5f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f};

    float angle = 0;
  
    float transformations[2][16] = {0};
    memcpy(transformations[0], scale_mat, sizeof(float) * 16);

    struct timeval start_time = {0};
    struct timeval end_time = {0};
    int frame = 0;
    while (!glfwWindowShouldClose(window))
    {
        gettimeofday(&start_time, NULL);

        angle = (float) (2 * M_PI) * (float) frame/FPS; 

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float rotate_z_mat[16] = {
            cosf(angle), -sinf(angle), 0.0f, 0.0f, 
            sinf(angle), cosf(angle), 0.0f, 0.0f, 
            0.0f,                 0.0f, 0.0f, 0.0f, 
            0.0f,                 0.0f, 0.0f, 1.0f};

        memcpy(transformations[1], rotate_z_mat, sizeof(float) * 16);
      
        glUseProgram(shaderProgram);
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transformations");
        glUniformMatrix4fv(transformLoc, 2, GL_FALSE, (float*) transformations[0]);
        unsigned int count_loc = glGetUniformLocation(shaderProgram, "transformation_count");
        glUniform1i(count_loc, (int) sizeof(transformations)/(sizeof(float) * 16));
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, ibuff.size, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        gettimeofday(&end_time, NULL);
        long ellapsed_time_us = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);
        /* printf("ellapsed time in us = %ld\n", ellapsed_time_us); */
        frame = (frame + 1)%FPS;
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

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

