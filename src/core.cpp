#include "core.h"
#include <iostream>

Core::Core(){
    this->moduleRole == MODULES::CORE;
}

void Core::run(){
    startModule(WINDOW);
    startModule(RENDERER);
	while (!glfwWindowShouldClose(windowHandler->getWindow()))
    {
        getWindowEvents();
        receiveMessage();
        renderer->renderFrame();
	}
}

std::pair<int,int> Core::getCurrentWindowSize(){
    return {currentWindowWidth,currentWindowHeight};
}

void Core::getWindowEvents(){
    windowHandler->getWindowEvents();
}

void Core::startModule(MODULES module){
    switch(module){
        case MODULES::WINDOW:
            windowHandler = new WindowHandler();
            windowHandler->start(this);
            break;
        case MODULES::RENDERER:
            renderer = new Renderer();
            renderer->start(this,windowHandler->getWindow());
            break;
        default:
            break;
    }
}

void Core::receiveMessage(){
    std::list<Message>::iterator message = messageQueue.begin();
    while(message != messageQueue.end()){
        printMessage(*message);
        switch(message->srcModule){
            case MODULES::WINDOW:
            switch(message->relatedEvent){
                case EVENTS::WINDOW_RESIZE:
                    if(renderer){
                        std::cout << "window resize";
                        sendMessageSync(EVENTS::WINDOW_RESIZE, MODULES::RENDERER, renderer);
                    }
                break;

                default:
                break;
            }
            break;

            default:
            break;
        }
        message = messageQueue.erase(message);
    }
}