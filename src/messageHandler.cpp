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
    int messagesMoved = 0;
    for(auto it : modulesToListen){
        while(auto msg = it.second->getNextMesage()){
            messagesMoved++;
            enum MODULES receivingModule = msg->dstModule;
            auto receivingModuleptr = modulesToListen.find(receivingModule)->second;
            receivingModuleptr->receiveMessage(msg);
        }
    }
    std::cout << "Messages moved: " << messagesMoved << std::endl;
    newMessageEvent = false;
}

void MessageHandler::checkNewMessage(){
    for(auto it : modulesToListen){
        if(it.second->checkNewMessage()){
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
            if(readGuard.try_lock()){
                checkNewMessage();
                readGuard.unlock();
            }
            else{
                return false;
            }

            return newMessageEvent;
            });
        moveMessages();
    }
}

