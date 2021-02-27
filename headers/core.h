#ifndef CORE_H
#define CORE_H

#include "messageHandler.h"
#include "window_handler.h"
#include "renderer.h"
#include <boost/thread.hpp>
#include "resourceHandler.h"
#include "sceneGraph.h"
#include "logic.h"



class Core : public Module{
public:
    Core();
    ~Core(){};
    std::pair<int,int> getCurrentWindowSize();
    std::shared_ptr<Renderer> getRenderer();
    void run();

protected:

private:
    std::shared_ptr<WindowHandler> windowHandler;
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<ResourceHandler> resourceHandler;
	std::shared_ptr<Renderable> item;
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
    std::shared_ptr<SceneGraph> sceneGraph;
    std::shared_ptr<MessageHandler> msgHandler;
    std::shared_ptr<Logic> logic;
    boost::thread* message_thread;
    void receiveMessage() override;
};

#endif