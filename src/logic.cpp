#include "logic.h"
#include "renderable.h"

const std::string MODEL_PATH = "models/viking.obj";
const std::string MODEL_PATH2 = "models/BeeModel.obj";

Logic::Logic(){
    this->moduleRole = MODULES::LOGIC;
};

Logic::~Logic(){

};

//void Logic::init(std::shared_ptr<Core> engineCore){
//    this->engineCore = engineCore;
//}

void Logic::update(){
    receiveMessage();
}

void Logic::loadModel(std::string path){
    sendMessage(EVENTS::LOAD_MODEL, MODULES::CORE, path);
}

void Logic::uploadModelToGPU(int modelID){

}

void Logic::receiveMessage(){
    while(auto message = readNextMessage()){
        switch(message->srcModule){
            case MODULES::WINDOW:
                switch(test){
                    case 0:
                        loadModel(MODEL_PATH);
                        test++;
                        break;
                    case 1:
                        loadModel(MODEL_PATH2);
                        test++;
                        break;
                    default:
                        break;
                }
            
            default:
            break;
        }
        delete message;
    }
}