#include "module.h"

void Module::sendMessageSync(EVENTS event, MODULES targetModule, Module* messageHandler, std::string message){
    Message messageToSend;
    messageToSend.srcModule = this->moduleRole;
    messageToSend.dstModule = targetModule;
    messageToSend.relatedEvent = event;
    messageToSend.messageText = message;
    messageHandler -> messageQueue.push_back(messageToSend);
}

#ifdef DEBUG_MODE_MODULE
#include <iostream>
void Module::printMessage(Message msg){
    std::cout   << "Message source: " << msg.srcModule << std::endl
                << "Message dest: " << msg.dstModule << std::endl
                << "Message event: " << msg.relatedEvent << std::endl
                << "Message text: " << msg.messageText << std::endl
                << "Is error: " << (msg.errorCode.has_value()?"True":"False") << std::endl;
}
#endif