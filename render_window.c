#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

#include "render_window.h"
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void render_window_init(RenderWindow* window, int width, int height, const char* title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (glfw_window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(glfw_window);
    glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback);

    window->window = glfw_window;
}
void render_window_process_input(RenderWindow *window)
{
    if (glfwGetKey(window->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window->window, true);
    }
    if (glfwGetKey(window->window, GLFW_KEY_W) == GLFW_PRESS)
    {
        assert(window->callbacks[GLFW_KEY_W]);
        window->callbacks[GLFW_KEY_W]();
    }
    if (glfwGetKey(window->window, GLFW_KEY_A) == GLFW_PRESS)
    {
        assert(window->callbacks[GLFW_KEY_A]);
        window->callbacks[GLFW_KEY_A]();
    }
    if (glfwGetKey(window->window, GLFW_KEY_S) == GLFW_PRESS)
    {
        assert(window->callbacks[GLFW_KEY_S]);
        window->callbacks[GLFW_KEY_S]();
    }
    if (glfwGetKey(window->window, GLFW_KEY_D) == GLFW_PRESS)
    {
        assert(window->callbacks[GLFW_KEY_D]);
        window->callbacks[GLFW_KEY_D]();
    }
    
}
void render_window_add_callback(RenderWindow *window, int key, void(*cb)(void))
{
    assert(key < CALLBACK_CAPACITY);
    window->callbacks[key] = cb;
}

bool render_window_should_close(RenderWindow *window)
{
    return glfwWindowShouldClose(window->window);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
