#include "KeyHandler.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <regex>
#include <boost/algorithm/string/trim.hpp>  


static const std::regex key_set_regex("set_key: \"((?:SHIFT\\+|ALT\\+|CTRL\\+)*.*?)\" \"(.*?)\"");
static const std::regex shift_regex("SHIFT+");
static const std::regex alt_regex("ALT+");
static const std::regex ctrl_regex("CTRL+");

KeyHandler::KeyHandler(){
    initKeys();
    readConfig();
};
/*
keyData KeyHandler::getKeyState(int32_t keyRequested){
    auto result = keyMap.find(keyRequested);
    if(result != keyMap.end()){
        return &result->second;
    }
    else{
        return nullptr;
  }
}
*/

void KeyHandler::initKeys(){

}

std::vector<keyData> KeyHandler::getHeldKeys(){
    std::vector<keyData> result;
    for(auto it : keyMap){
        if(it.second.isHeld){
            result.push_back(it.second);
        }
    }
    return result;
}



void KeyHandler::setKeyState(int32_t button, bool state){
    auto it = keyMap.find(button);
    if(it != keyMap.end()){
        it->second.isHeld = state;
    }
}


void KeyHandler::bindEventToKey(int32_t button, enum EVENTS eventToBind, std::error_code* ec){
   auto it = keyMap.find(button);
   if(it == keyMap.end()){
       keyData keyToBind = {};
       keyToBind.isHeld = false;
       keyToBind.actionBound = eventToBind;

       keyMap.insert({button, keyToBind});
   }
   else {
       if(ec){
            *ec = KeyBindErrors::KeyAlreadyBound;
            return;
       }
   }
}

void KeyHandler::unbindKey(int32_t button){
   auto it = keyMap.find(button);
   if(it != keyMap.end()){
       keyMap.erase(it);
   }
}

void KeyHandler::printKeyData(int32_t key){
    auto it = keyMap.find(key);
    if(it == keyMap.end()){
        std::cout << "Key: " << key << " is not mapped to anything" << std::endl;
    }
    else{
        std::cout << "Current key state of key " << key << " is: " << it->second.isHeld << std::endl <<
                     "Currently bound action is: " << ENUMMAP::reverseEventMap.find(it->second.actionBound)->second << std::endl;
    }
}

void KeyHandler::printKeyData(keyData key){
    std::cout << "Current key state of key " << key.keyNumber << " is: " << key.isHeld << std::endl <<
                 "Currently bound action is: " << ENUMMAP::reverseEventMap.find(key.actionBound)->second << std::endl;
}


void KeyHandler::readConfig(std::string filePath){
    std::filesystem::path currentPath;
    if(filePath == ""){
        currentPath = std::filesystem::current_path();
        currentPath.append("config\\keys.ini");
    }
    else{
        currentPath = filePath;
    }
    std::cout << "Current config Path is: " << currentPath << std::endl;
    std::ifstream file(currentPath);
    std::string currentLine;

    if(file.is_open()){
        std::cout<< "successfully opened config file" << std::endl;
        std::smatch matches;
        while(std::getline(file,currentLine)){
            boost::trim(currentLine);
            if(currentLine[0] != '#'){
                if(std::regex_match(currentLine, key_set_regex)){
                    std::regex_search(currentLine, matches, key_set_regex);
                    keyData currentData;

                    int32_t key = matches[1].str()[0];
                    check_modifiers(key,matches[1].str());
                    if(keyMap.find(key) != keyMap.end()){
                        std::cout << "Doubly assigned key in config file, ignoring all but first on key: " << key << std::endl;
                        break;
                    }

                    std::string action;
                    if(matches.size() > 2){
                        action = matches[2];
                    }
                    else{
                        action = "";
                    }
                    auto eventLookUp = ENUMMAP::eventMap.find(action);
                    if(eventLookUp == ENUMMAP::eventMap.end()){
                        if(action != ""){
                            std::cout << "invalid key_map event name in config file: " << action << std::endl;
                        }
                    }
                    else{
                        enum EVENTS eventBound = eventLookUp->second;

                        currentData.keyNumber = key;
                        currentData.isHeld = false;
                        currentData.actionBound = eventBound;


                        keyMap.insert({key,currentData});
                        printKeyData(key);
                    }
                }
            }
        }
    }
}

void KeyHandler::check_modifiers(int32_t &key, std::string text){
    if(std::regex_match(text,shift_regex)){
        key |= MODIFIER_SHIFT;
    }
    if(std::regex_match(text,ctrl_regex)){
        key |= MODIFIER_CTRL;
    }
    if(std::regex_match(text,alt_regex)){
        key |= MODIFIER_ALT;
    }
}