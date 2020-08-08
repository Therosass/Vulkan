#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "module.h"
#include <unordered_map>

static bool newMessageEvent = false;

class MessageHandler : public Module{
public:

    void receiveMessage(){};
    void registerModule(Module* moduleToRegister);
    void moveMessages();
    void checkNewMessage();
    boost::condition_variable* getConditionVariablePtr();

    void run();

protected:

    boost::condition_variable cv;
    boost::mutex messageProcessingLock;
    boost::mutex registerLock;
    std::unordered_map<enum MODULES, Module*> modulesToListen;
    std::queue<Message*> messageTransferQueue;


};

#endif //(MESSAGEHANDLER_H)
