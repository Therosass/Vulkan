#include "keyHandler.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <regex>
#include <boost/algorithm/string/trim.hpp>


static const std::regex key_set_regex("set_key: (\"(?:SHIFT\\+|ALT\\+|CTRL\\+)*.*?\") (\".*?\")*");
static const std::regex shift_regex("SHIFT+");
static const std::regex alt_regex("ALT+");
static const std::regex ctrl_regex("CTRL+");


keyHandler::keyHandler(){
    initKeys();
};

keyData* keyHandler::getKeyState(char16_t keyRequested){
    auto result = keyMap.find(keyRequested);
    if(result != keyMap.end()){
        return &result->second;
    }
    else{
        return nullptr;
    }
}

void keyHandler::initKeys(){

}

void keyHandler::readConfig(std::string filePath){
    std::filesystem::path currentPath = std::filesystem::current_path();
    currentPath.append("config/keys.ini");
    std::cout << "Current Path is: " << currentPath << std::endl;
    std::ifstream file(currentPath);
    std::string currentLine;

    if(file.is_open()){ 
        std::smatch matches;
        while(std::getline(file,currentLine)){
            boost::trim(currentLine);
            if(currentLine[0] != '#'){
                if(std::regex_match(currentLine, key_set_regex)){
                    std::regex_search(currentLine, matches, key_set_regex);
                    keyData currentData;
                    char16_t key = matches[1].str()[0];
                    std::string action;
                    if(matches.size() > 2){
                        action = matches[2];
                    }
                    else{
                        action = "";
                    }
                    check_modifiers(key,matches[1].str());
                    currentData.isHeld = false;
                    currentData.actionBound = action;
                    keyMap.insert({key,currentData});
                }
            }
        }
    }


}

void keyHandler::check_modifiers(char16_t &key, std::string text){
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