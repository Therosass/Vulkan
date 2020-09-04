#include "resourceHandler.h"

void Renderable::init(std::string texturePath, std::string modelPath, ResourceHandler* handler){
    this->texturePath = texturePath;
    this->modelPath = modelPath;
    this->handler = handler;
    modelID = handler->loadModel(modelPath);
    this->modelHandle = handler->getModelHandle(modelID);
    textureID = handler->loadTexture(texturePath);
    textureViewID = textureID;
    this->textureHandle = handler->getTextureHandle(textureID);
    descriptorSetID = handler->createDescriptorSet(0,textureID);

}

int Renderable::getDescriptorSet(){
    return descriptorSetID;
}

int Renderable::getVertexBuffer(){
    return modelHandle->vertexBufferID;
}

int Renderable::getIndexBuffer(){
    return modelHandle->indexBufferID;
}

int Renderable::getIndiceAmount(){
    return modelHandle->indices.size();
}
