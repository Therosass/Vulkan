#include "core.h"
#include <iostream>


const std::string MODEL_PATH = "models/viking.obj";
const std::string MODEL_PATH2 = "models/BeeModel.obj";
const std::string TEXTURE_PATH = "textures/viking.png";
const std::string TEXTURE_PATH2 = "textures/black_and_yellow.png";

Core::Core(){
    this->moduleRole = MODULES::CORE;
    this->coreModule = this;
    this->msgHandler = std::make_shared<MessageHandler>();
    this->sceneGraph = std::make_unique<SceneGraph>();
    cv = msgHandler->getConditionVariablePtr();
    message_thread = new boost::thread(MessageHandler::run, msgHandler);
}

void Core::run(){
    startModule(WINDOW);
    startModule(RENDERER);
    this->resourceHandler = std::make_shared<ResourceHandler>(renderer);
    renderer->createUniformBuffer(sizeof(UniformBufferObject));
    std::shared_ptr<Renderable> item = std::make_shared<Renderable>() ;
    std::shared_ptr<Renderable> item2 = std::make_shared<Renderable>() ;
	item->init(TEXTURE_PATH2,MODEL_PATH2,resourceHandler);
    item2->init(TEXTURE_PATH,MODEL_PATH,resourceHandler);
    sceneGraph->addNode(item);
    sceneGraph->addNode(item2);
    std::cout << "initalized with" << std::endl 
            << "core: " << this << std::endl 
            << "msgHandler: " << this->msgHandler << std::endl 
            << "windowHandler: " << this->windowHandler << std::endl 
            << "renderer: " << this->renderer << std::endl; 

	while (!glfwWindowShouldClose(windowHandler->getWindow()))
    {
        getWindowEvents();
        receiveMessage();
        renderer->beginRenderPass();
        renderer->renderScene(sceneGraph->getSceneGraph());
        renderer->endRenderPass();
	}
}

std::shared_ptr<Renderer> Core::getRenderer(){
    return renderer;
}


std::pair<int,int> Core::getCurrentWindowSize(){
    return {currentWindowWidth,currentWindowHeight};
}

void Core::getWindowEvents(){
    windowHandler->getWindowEvents();
}

//TODO: figure out how shared_ptr_from_this works

void Core::startModule(MODULES module){
    switch(module){
        case MODULES::WINDOW:
            windowHandler = std::make_shared<WindowHandler>();
            windowHandler->init(this);
            std::cout << "window pointer: " << windowHandler << std::endl;
            msgHandler->registerModule(windowHandler);
            break;
        case MODULES::RENDERER:
            renderer = std::make_shared<Renderer>();
            renderer->start(std::shared_ptr<Core>(this),windowHandler->getWindow());
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