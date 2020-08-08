#ifndef CORE_H
#define CORE_H

#include "messageHandler.h"
#include "window_handler.h"
#include "renderer.h"
//#include "keyHandler.h"


class Core : public Module{
public:
    Core();
    ~Core(){};
    std::pair<int,int> getCurrentWindowSize();
    void run();

protected:

private:
    WindowHandler* windowHandler;
    Renderer* renderer;
    void getWindowEvents();
    void startModule(MODULES module);
    void initGLFW();
    std::pair<int,int> currentMousePos;
    int currentWindowHeight;
    int currentWindowWidth;

/*****
 * 
 * Module|MessageHandling implementation
 * 
 ****/

    MessageHandler* msgHandler;
    void receiveMessage() override;
};

#endif