#include "module.h"

void Module::sendMessageSync(EVENTS event, MODULES targetModule, Module* messageHandler, std::string message){
    Message messageToSend;
    messageToSend.srcModule = this->moduleRole;
    messageToSend.dstModule = targetModule;
    messageToSend.relatedEvent = event;
    messageToSend.messageText = message;
    messageHandler -> messageQueue.push_back(messageToSend);
}