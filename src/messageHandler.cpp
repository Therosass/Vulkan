#include "messageHandler.h"
#include <iostream>

void MessageHandler::registerModule(Module* moduleToRegister){
    // TODO: add compile time enum max
    enum MODULES roleValue = moduleToRegister->getModuleRole();
    if(roleValue > 3 || roleValue < 0){
        std::cout << this << " " << moduleRole << std::endl;
        throw std::runtime_error("moduleRole not set properly");
    }
    boost::lock_guard<boost::mutex> guard(this->registerLock);
    modulesToListen.insert({moduleToRegister->getModuleRole(), moduleToRegister});
    moduleToRegister->setWaitPointer(&cv);
}

void MessageHandler::moveMessages(){
    boost::lock_guard<boost::mutex> guard(this->registerLock); //cannot register new modules while reading messages
    boost::lock_guard<boost::mutex> guard2(this->readGuard); //protects new events flag until reading is done
    auto it = modulesToListen.begin();
    while(it != modulesToListen.end()){
        while(auto msg = it->second->getNextMesage()){
            //printMessage(*msg);
            enum MODULES receivingModule = msg->dstModule;
            auto receivingModuleptr = modulesToListen.find(receivingModule)->second;
            //std::cout << "Sent Message to: " << receivingModuleptr << std::endl;
            //std::cout << "Sent Packet Data ptr: " << msg->dataPacket->data << std::endl;
            //std::cout << "Data packet created" << std::endl <<
            //            "Data type: " << msg->dataPacket->dataType << std::endl <<
            //            "Data size: " << msg->dataPacket->length << std::endl <<
            //            "Data: " << static_cast<std::pair<int,int>*>(msg->dataPacket->data)->first << ":" << static_cast<std::pair<int,int>*>(msg->dataPacket->data)->second << std::endl <<
            //            "Data pointer: " << msg->dataPacket->data << std::endl;

            receivingModuleptr->receiveMessage(msg);
        }
        it++;
    }
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
        moveMessages();
    }
}

