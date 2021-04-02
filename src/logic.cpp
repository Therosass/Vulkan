#include "logic.h"
#include "renderable.h"
#include "scenegraph.h"

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

void Logic::setSceneGraph(std::shared_ptr<SceneGraph> newSceneGraph){
    sceneGraph = newSceneGraph;
}

void Logic::uploadModelToGPU(int modelID){

}

void Logic::receiveMessage(){
    while(auto message = readNextMessage()){
        switch(message->srcModule){
            case MODULES::WINDOW:
                switch(message->relatedEvent){
                    case EVENTS::SELECT_NEXT_OBJECT:
                        sceneGraph->selectNextNode();
                    break;

                    case EVENTS::MOVE_OBJECT_FORWARD:
                    {
                        auto node = sceneGraph->getSelectedNode();
                        if(node){
                            node->TRMatrix->forward();
                            node->setUpdated();
                        }
                    }
                    break;

                    case EVENTS::MOVE_OBJECT_BACKWARD:
                    {
                        auto node = sceneGraph->getSelectedNode();
                        if(node){
                            node->TRMatrix->backward();
                            node->setUpdated();
                        }
                    }
                    break;

                    case EVENTS::LOAD_MODEL:
                        switch(test){
                            case 0:
                                loadModel(MODEL_PATH);
                                test++;
                                break;
                            case 1:
                                loadModel(MODEL_PATH2);
                                test++;
                                break;
                            case 2:
                                loadModel(MODEL_PATH);
                                test++;
                                break;
                            default:
                                break;
                        }
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