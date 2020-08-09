#include "module.h"

void Module::sendMessage(EVENTS event, MODULES targetModule, std::string message){
    boost::lock_guard<boost::mutex> lock(readGuard);
    Message* messageToSend = new Message;
    messageToSend->srcModule = moduleRole;
    messageToSend->dstModule = targetModule;
    messageToSend->relatedEvent = event;
    messageToSend->messageText = message;
    printMessage(*messageToSend);
    sendQueue.push(messageToSend);
    cv->notify_one();
}

#ifdef DEBUG_MODE_MODULE
#include <iostream>
void Module::printMessage(Message msg){
    std::cout   << "Message source: " << msg.srcModule << std::endl
                << "Message dest: " << msg.dstModule << std::endl
                << "Message event: " << msg.relatedEvent << std::endl
                << "Message text: " << msg.messageText << std::endl
                << "Is data: " << (msg.dataPacket.has_value()?"True":"False") << std::endl
                << "Is error: " << (msg.errorCode.has_value()?"True":"False") << std::endl;
}

    
#endif