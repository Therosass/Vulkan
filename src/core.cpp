#include "core.h"
#include <iostream>




Core::Core(){
    this->moduleRole = MODULES::CORE;
    this->coreModule = this;
    this->msgHandler = std::make_shared<MessageHandler>();
    this->sceneGraph = std::make_unique<SceneGraph>();
    cv = msgHandler->getConditionVariablePtr();
    message_thread = new boost::thread(MessageHandler::run, msgHandler);
    msgHandler->registerModule(std::shared_ptr<Core>(this));
}

void Core::run(){
    startModule(WINDOW);
    startModule(LOGIC);
    startModule(RENDERER);
    logic->setSceneGraph(sceneGraph);
    this->resourceHandler = std::make_shared<ResourceHandler>(renderer);

    std::cout << "initalized with" << std::endl 
            << "core: " << this << std::endl 
            << "msgHandler: " << this->msgHandler << std::endl 
            << "windowHandler: " << this->windowHandler << std::endl 
            << "renderer: " << this->renderer << std::endl
            << "logic: " << this->logic << std::endl; 
	while (!glfwWindowShouldClose(windowHandler->getWindow()))
    {
        getWindowEvents();
        receiveMessage();
        logic->update();
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
            windowHandler = std::shared_ptr<WindowHandler>(new WindowHandler());
            windowHandler->init(this);
            std::cout << "window pointer: " << windowHandler << std::endl;
            msgHandler->registerModule(windowHandler);
            break;
        case MODULES::RENDERER:
            renderer = std::shared_ptr<Renderer>(new Renderer());
            renderer->start(std::shared_ptr<Core>(this),windowHandler->getWindow());
            std::cout << "renderer pointer: " << renderer << std::endl;
            msgHandler->registerModule(renderer);
            break;
        case MODULES::LOGIC:
            logic = std::shared_ptr<Logic>(new Logic());
            std::cout << "logic pointer: " << logic << std::endl;
            msgHandler->registerModule(logic);
            break;
        default:
            break;
    }
}

void Core::receiveMessage(){
    while(auto message = readNextMessage())
    {
        switch(message->srcModule)
        {
            case MODULES::LOGIC:
            
            switch(message->relatedEvent)
            {
                case EVENTS::LOAD_MODEL:
                {
                    std::shared_ptr<Renderable> item = std::make_shared<Renderable>();
                    item->init(message->messageText,resourceHandler);
                    std::shared_ptr<TransformationMatrix> transformation = std::make_shared<TransformationMatrix>();
                    transformation->scale = {
                        0.2f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.2f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.2f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                    };
                    transformation->rotate = {
                        1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                    };
                    transformation->translate = {
                        1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                    };  
                    auto result = sceneGraph->addNode(transformation);
                    sceneGraph->addNode(item, result);
                break;
                }
    
                default:
                break;
            }
        }



        delete message;
    }
}