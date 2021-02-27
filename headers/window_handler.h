#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H

class Core;

#include "GLFW/glfw3.h"

#include "module.h"
#include "keyHandler.h"

class WindowHandler : public Module{
public:
    WindowHandler();
    ~WindowHandler(){};
    void init(Core* engineCore);
    GLFWwindow* getWindow();
    void getWindowEvents();
    std::pair<int,int> getCurrentMousePos();
    void setCurrentMousePos(std::pair<int,int> newMousePos);
protected:
private:
    inline static KeyHandler keyHandler;
    Core* engineCore;
    GLFWwindow* window;
    void initGLFW();
    int windowWidth = 800;
    int windowHeight = 600;    
    bool windowResizedFlag = false;
    std::pair<int,int> currentMousePos;
    static double XP;
    static double YP;
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