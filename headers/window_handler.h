#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H

class Core;

#include "GLFW/glfw3.h"




#include "module.h"


class WindowHandler : public Module{
public:
    ~WindowHandler(){};
    void start(Core* engineCore);
    GLFWwindow* getWindow();
    void getWindowEvents();
protected:
private:
    Core* engineCore;
    GLFWwindow* window;
    void initGLFW();
    uint32_t WIDTH = 800;
    uint32_t HEIGHT = 600;    
    bool windowResizedFlag = false;
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double xPos, double yPos);

/*****
 * 
 * Module function implementation
 * 
 ****/

    void receiveMessage() override;
};

#endif