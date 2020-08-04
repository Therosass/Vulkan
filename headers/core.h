#ifndef CORE_H
#define CORE_H

#include "module.h"
#include "window_handler.h"
#include "renderer.h"
//#include "keyHandler.h"


class Core : public Module{
public:
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
    int currentWindowHeight;
    int currentWindowWidth;

/*****
 * 
 * Module function implementation
 * 
 ****/

    void receiveMessage() override;
};

#endif