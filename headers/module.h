#ifndef MODULE_H
#define MODULE_H

#include <list>
#include <string>
#include <optional>
#include <memory>
#include <queue>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/condition_variable.hpp>

#define DEBUG_MODE_RENDERER
#define DEBUG_MODE_MODULE

enum DATATYPES{
    UNKNOWN_TYPE,
    CHAR,
    FLOAT,
    DOUBLE,
    INT,
    BYTES,
    STDSTRING

};

namespace CONTAINERS{

    constexpr int32_t is_array = 0x10000000;
    constexpr int32_t is_list = 0x20000000;

}

enum ERRORCODES{
    INVALID_ERROR
};

enum MODULES{
    CORE = 0x00000000,
    WINDOW = 0x00000001,
    RENDERER = 0x00000002
};

enum EVENTS{
#ifdef DEBUG_MODE_RENDERER
    WINDOW_RESIZE_SUCCESSFUL,
#endif
    WINDOW_RESIZE,
    CURSOR_MOVE,
    KEYBOARD_EVENT
};

struct DataPacket{
    enum DATATYPES dataType;
    int length;
    void* data;
};

struct Message{ 
    enum MODULES srcModule;
    enum MODULES dstModule;
    enum EVENTS relatedEvent;
    std::string messageText;
    std::optional<DataPacket> dataPacket;
    std::optional<ERRORCODES> errorCode;
    std::optional<std::string> errorMessage;
};

class Module{
public:

    template <class T>
    DataPacket createDataPacket(T* data, enum DATATYPES dataType = DATATYPES::UNKNOWN_TYPE){
        //static::assert((boost::is_pointer<T>,"createDataPacket can only be instantiated with a pointer type!"));
        auto copyOfData = new T();
        copyOfData = data;
        
        DataPacket newPacket;
        newPacket.dataType = dataType;
        newPacket.length = sizeof(*data)/sizeof(data);
        newPacket.data = (void*)copyOfData; 

        return newPacket;
    };

    virtual ~Module(){};
    virtual void receiveMessage() = 0;
    
    template <class T>
    void sendDataPacket(EVENTS event, MODULES targetModule, T dataToSend, std::string message = "", enum DATATYPES dataType = DATATYPES::UNKNOWN_TYPE){
        DataPacket newPacket = createDataPacket(std::addressof(dataToSend),dataType);
        Message newMessage = Message();
        newMessage.srcModule = moduleRole;
        newMessage.dstModule = targetModule;
        newMessage.dataPacket = newPacket;
        newMessage.relatedEvent = event;
    };

    virtual void sendMessage(EVENTS event, MODULES targetModule, std::string message = "");

    Message* getNextMesage(){
        boost::lock_guard<boost::mutex> lock(readGuard);
        if(sendQueue.empty()){
            return nullptr;
        }
        Message* nextMessage = sendQueue.front();
        return nextMessage;
    }

    void receiveMessage(Message* message){
        boost::lock_guard<boost::mutex> lock(writeGuard);
        receiveQueue.push(message);
    }

    enum MODULES getModuleRole(){
        return moduleRole;
    }

    void setWaitPointer(boost::condition_variable* cv_ptr){
        cv = cv_ptr;
    }

    bool checkNewMessage(){
        boost::lock_guard<boost::mutex> lock(readGuard);
        if(sendQueue.size() > 0){
            return true;
        }
        return false;
    }
    
#ifdef DEBUG_MODE_MODULE
    virtual void printMessage(Message msg);
#endif

protected:

    boost::condition_variable* cv;
    int currentPacketID = 0;
    enum MODULES moduleRole;
    Module* coreModule = nullptr;

    boost::mutex PacketIDLock;
    boost::mutex readGuard;
    boost::mutex writeGuard;

    std::queue<Message*> sendQueue;
    std::queue<Message*> receiveQueue;

    int getNewPacketID(){
        boost::lock_guard<boost::mutex> lock(PacketIDLock);
        currentPacketID++;
        return currentPacketID;
    };

};




/*

Implementation of generic data packet
This is all horribly C and will probably be removed

template <class T>
enum DATATYPES getDataType(T dataToCheck){
    typedef typename std::remove_all_extents<T>::type Type;
    if(typeid(Type) == typeid(char)){
        return DATATYPES::CHAR;
    }
    if(typeid(Type) == typeid(int)){
        return DATATYPES::INT;
    }
    if(typeid(Type) == typeid(float)){
        return DATATYPES::FLOAT;
    }
    if(typeid(Type) == typeid(double)){
        return DATATYPES::DOUBLE;
    }
    if(typeid(Type) == typeid(void*)){
        return DATATYPES::BYTES;
    }
    if(typeid(Type) == typeid(std::string)){
        return DATATYPES::STDSTRING;
    }
    else{
        return DATATYPES::INVALID_TYPE;
    }
}

template <typename T>
MessageData convertData(T dataToConvert, DATATYPES TYPE){
    typedef typename std::remove_all_extents<T>::type Type;
    std::shared_ptr<void*> ptr = nullptr;
    int32_t size = sizeof(dataToConvert);
    int32_t length = sizeof(dataToConvert) / sizeof(*dataToConvert);
    ptr = (void*)new T[length];
    if(std::is_same<T,std::array<Type>){
        TYPE msg.dataType &= CONTAINERS::is_array;
    }
    if(std::is_same<T,std::list<Type>){
        TYPE &= CONTAINERS::is_list;
    }
    MessageData msg;
    msg.dataLocation = ptr;
    msg.dataLengthInType = length;
    msg.dataType = TYPE;
}

template <class T>
MessageData convertData(T dataToConvert){
    enum DATATYPES type = getDataType<T>;
    std::shared_ptr<void*> ptr = nullptr;
    int32_t size = sizeof(dataToConvert);
    int32_t length = sizeof(dataToConvert) / sizeof(*dataToConvert);
    ptr = (void*)new T[length];
    memcpy(ptr,&dataToConvert,length);
    if(typeid(T) == typeid(std::array)){
        type msg.dataType &= DATATYPES::is_array;
    }
    if(typeid(T) == typeid(std::list)){
        type &= DATATYPES::is_list;
    }
    MessageData msg;
    msg.dataLocation = ptr;
    msg.dataLengthInType = length;
    msg.dataType = type;
}
*/

#endif
