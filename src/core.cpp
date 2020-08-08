#include "core.h"
#include <iostream>

Core::Core(){
    this->moduleRole = MODULES::CORE;
    this->coreModule = this;
    this->msgHandler = new MessageHandler;
    cv = msgHandler->getConditionVariablePtr();
    message_thread = new boost::thread(&MessageHandler::run, msgHandler);
}

void Core::run(){
    startModule(WINDOW);
    startModule(RENDERER);
    std::cout << "initalized with" << std::endl 
            << "core: " << this << std::endl 
            << "msgHandler: " << this->msgHandler << std::endl 
            << "windowHandler: " << this->windowHandler << std::endl 
            << "renderer: " << this->renderer << std::endl; 
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
            windowHandler->init(this);
            std::cout << "window pointer: " << windowHandler << std::endl;
            msgHandler->registerModule(windowHandler);
            break;
        case MODULES::RENDERER:
            renderer = new Renderer();
            renderer->start(this,windowHandler->getWindow());
            std::cout << "renderer pointer: " << renderer << std::endl;
            msgHandler->registerModule(renderer);
            break;
        default:
            break;
    }
}

void Core::receiveMessage(){
    /*
    std::list<Message>::iterator message = receiveQueue.begin();
    while(message != receiveQueue.end()){
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

                case EVENTS::CURSOR_MOVE:
                    DataPacket<std::pair<int,int>> test = receiveData<std::pair<int,int>>(message->dataPacketID);
                    currentMousePos = DataPacket<std::pair<int,int>> 

                break;

                case EVENTS::KEYBOARD_EVENT:
                break;


                default:
                break;
            }
            break;

            default:
            break;
        }
        message = messageQueue.erase(message);
    }*/
}