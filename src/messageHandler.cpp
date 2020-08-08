#include "messageHandler.h"
#include <iostream>

void MessageHandler::registerModule(Module* moduleToRegister){
    boost::lock_guard<boost::mutex> guard(this->registerLock);
    modulesToListen.insert({moduleToRegister->getModuleRole(), moduleToRegister});
    moduleToRegister->setWaitPointer(&cv);
}

void MessageHandler::moveMessages(){
    boost::lock_guard<boost::mutex> guard(this->registerLock); //cannot register new modules while reading messages
    boost::lock_guard<boost::mutex> guard2(this->readGuard); //protects new events flag until reading is done
    std::cout << "Messages started to moveded" << std::endl;
    int messagesMoved = 0;
    auto it = modulesToListen.begin();
    while(it != modulesToListen.end()){
        while(auto msg = it->second->getNextMesage()){
            printMessage(*msg);
            std::cout << "Module role: " << it->second->getModuleRole() << std::endl;
            std::cout << "Messages gettened" << std::endl;
            messagesMoved++;
            enum MODULES receivingModule = msg->dstModule;
            auto receivingModuleptr = modulesToListen.find(receivingModule)->second;
            std::cout << "Messages created" << std::endl; 
            receivingModuleptr->receiveMessage(msg);
        }
        it++;
    }
    std::cout << "Messages moved: " << messagesMoved << std::endl;
    newMessageEvent = false;
}

void MessageHandler::checkNewMessage(){
    for(auto it : modulesToListen){
        if(it.second->checkNewMessage()){
            std::cout << "Message gottened" << std::endl;
            newMessageEvent = true;
        }
    }
}

boost::condition_variable* MessageHandler::getConditionVariablePtr(){
    return &cv;
}


void MessageHandler::run(){
    std::cout<<"MessageHandler successfully started\n\n\n\n";
    while(true){
        boost::unique_lock<boost::mutex> lock(messageProcessingLock);
        cv.wait(lock, [this]{
            std::cout << "Notified" << std::endl;
            if(readGuard.try_lock()){
                std::cout << "Locked" << std::endl;
                checkNewMessage();
                readGuard.unlock();
            }
            else{
                std::cout << "Failed to locked" << std::endl;
                return false;
            }

            return newMessageEvent;
            });
        std::cout << "Moving messages" << std::endl;
        moveMessages();
    }
}

