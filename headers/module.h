#ifndef MODULE_H
#define MODULE_H

#include <list>
#include <string>
#include <optional>

#define DEBUG_MODE_RENDERER
#define DEBUG_MODE_MODULE

enum MODULES{
    CORE = 0x00000000,
    WINDOW = 0x00000001,
    RENDERER = 0x00000002
};

enum EVENTS{
#ifdef DEBUG_MODE_RENDERER
    WINDOW_RESIZE_SUCCESSFUL,
#endif
    WINDOW_RESIZE
};

enum ERRORCODES{

};

struct Message{
    enum MODULES srcModule;
    enum MODULES dstModule;
    enum EVENTS relatedEvent;
    std::string messageText;
    std::optional<ERRORCODES> errorCode;
    std::optional<std::string> errorMessage;
};

class Module{
public:
    virtual ~Module(){};
    virtual void receiveMessage() = 0;
    virtual void sendMessageSync(EVENTS event, MODULES targetModule, Module* messageHandler, std::string message = "");
#ifdef DEBUG_MODE_MODULE
    virtual void printMessage(Message msg);
#endif

protected:
    enum MODULES moduleRole;
    std::list<Message> messageQueue;
    std::list<Message> processableEventQueue;

};

#endif