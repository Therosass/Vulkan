#ifndef CORE_H
#define CORE_H

#include "messageHandler.h"
#include "window_handler.h"
#include "renderer.h"
#include <boost/thread.hpp>
#include "resourceHandler.h"
#include "sceneGraph.h"


class Core : public Module{
public:
    Core();
    ~Core(){};
    std::pair<int,int> getCurrentWindowSize();
    Renderer* getRenderer();
    void run();

protected:

private:
    WindowHandler* windowHandler;
    Renderer* renderer;
    ResourceHandler* resourceHandler;
    SceneGraph* sceneGraph;
	Renderable item;
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
    boost::thread* message_thread;
    void receiveMessage() override;
};

#endif