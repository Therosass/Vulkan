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

#include "module_enums.h"

#include <iostream>

#define DEBUG_MODE_RENDERER
#define DEBUG_MODE_MODULE

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
        auto copyOfData = new T(*data);
        
        DataPacket newPacket;
        newPacket.dataType = dataType;
        newPacket.length = sizeof(*data)/sizeof(data);
        newPacket.data = (void*)copyOfData; 

        //std::cout << "Data packet created" << std::endl <<
        //            "Data type: " << newPacket.dataType << std::endl <<
        //            "Data size: " << newPacket.length << std::endl <<
        //            "Data: " << static_cast<std::pair<int,int>*>(newPacket.data)->first << ":" << static_cast<std::pair<int,int>*>(newPacket.data)->second << std::endl <<
        //            "Data pointer: " << newPacket.data << std::endl;

        return newPacket;
    };

    virtual ~Module(){};
    virtual void receiveMessage() = 0;
    
    /*
    *   sendDataPacket:
    *   Places a message with a DataPacket into the sendqueue
    *   The Datapacket contains a pointer to the copy of the data to be sent
    *
    */

    template <class T>
    void sendDataPacket(EVENTS event, MODULES targetModule, T dataToSend, std::string message = "", enum DATATYPES dataType = DATATYPES::UNKNOWN_TYPE){
        DataPacket newPacket = createDataPacket(std::addressof(dataToSend),dataType);
        Message* messageToSend = new Message;
        messageToSend->srcModule = moduleRole;
        messageToSend->dstModule = targetModule;
        messageToSend->dataPacket = newPacket;
        messageToSend->relatedEvent = event;
        messageToSend->messageText = message;
        sendQueue.push(messageToSend);
        cv->notify_one();
    };

    virtual void sendMessage(EVENTS event, MODULES targetModule, std::string message = "");

    Message* getNextMesage(){ //Used by MessageHandler to receive an element from sendQueue
        boost::lock_guard<boost::mutex> lock(readGuard);
        if(sendQueue.empty()){
            return nullptr;
        }
        Message* nextMessage = sendQueue.front();
        sendQueue.pop();
        return nextMessage;
    }

    void receiveMessage(Message* message){ //Used by MessageHandler to send a new message to receiveQueue
        boost::lock_guard<boost::mutex> lock(writeGuard);
        receiveQueue.push(message);
    }

    Message* readNextMessage(){ //Get next message from receiveQueue
        boost::lock_guard<boost::mutex> lock(writeGuard);
        if(receiveQueue.empty()){
            return nullptr;
        }
        Message* newMsg = receiveQueue.front();
        receiveQueue.pop();
        return newMsg;
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

    boost::mutex readGuard;
    boost::mutex writeGuard;

    std::queue<Message*> sendQueue;
    std::queue<Message*> receiveQueue;

    int getNewPacketID(){
        boost::lock_guard<boost::mutex> lock(writeGuard);
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
