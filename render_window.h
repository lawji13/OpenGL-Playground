#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H
#include <stdbool.h>
#include <GLFW/glfw3.h>

#define CALLBACK_CAPACITY GLFW_KEY_LAST

typedef struct RenderWindow
{
    GLFWwindow* window;
    void(*callback[CALLBACK_CAPACITY])(void);
}RenderWindow;

void render_window_init(RenderWindow* window, int width, int height, const char* title);
void render_window_process_input(RenderWindow *window);
void render_window_add_callback(int key, void(*cb)(void));
bool render_window_should_close(RenderWindow *window);

#endif // RENDER_WINDOW_H

