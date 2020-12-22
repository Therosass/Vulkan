#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "module.h"
#include <unordered_map>

static bool newMessageEvent = false;

class MessageHandler : public Module{
public:

    MessageHandler(){
        moduleRole = MODULES::MESSAGEHANDLER;
    };
    void receiveMessage(){};
    void registerModule(std::shared_ptr<Module> moduleToRegister);
    void moveMessages();
    void checkNewMessage();
    boost::condition_variable* getConditionVariablePtr();

    void run();

protected:

    boost::condition_variable cv;
    boost::mutex messageProcessingLock;
    boost::mutex registerLock;
    std::unordered_map<enum MODULES, std::shared_ptr<Module>> modulesToListen;
    std::queue<Message*> messageTransferQueue;


};

#endif //(MESSAGEHANDLER_H)
