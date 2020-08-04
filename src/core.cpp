#include "core.h"

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
    for(auto message : messageQueue){
        switch(message.srcModule){
            case MODULES::WINDOW:
            switch(message.dstModule){
                case EVENTS::WINDOW_RESIZE:
                    if(renderer){
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
    }
}