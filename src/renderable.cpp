#include "resourceHandler.h"
#include <iostream>
const std::string MODEL_PATH = "models/viking.obj";
const std::string MODEL_PATH2 = "models/BeeModel.obj";
const std::string TEXTURE_PATH = "textures/viking.png";
const std::string TEXTURE_PATH2 = "textures/black_and_yellow.png";
//TODO: read texture from model object file!!!

unsigned int Renderable::objectCount = 0;

void Renderable::init(std::string modelPath, std::shared_ptr<ResourceHandler> handler){
    std::string texturePath;
    if(modelPath == MODEL_PATH){
        texturePath = TEXTURE_PATH;
    }
    else if(modelPath == MODEL_PATH2){
        texturePath = TEXTURE_PATH2;
    }
    else{
        std::cout << "INVALID MODEL PATH GIVEN, PLEASE FIX THIS FUNCTION SOMETIME" << std::endl;
    }
    this->texturePath = texturePath;
    this->modelPath = modelPath;
    this->handler = handler;
    modelID = handler->loadModel(modelPath, objectCount);
    this->modelHandle = handler->getModelHandle(modelID);
    textureID = handler->loadTexture(texturePath);
    textureViewID = textureID;
    this->textureHandle = handler->getTextureHandle(textureID);
    descriptorSetID = handler->createDescriptorSet(1,textureID);
    objectCount++;
}

void Renderable::updateBufferValue(){
    handler->updateUniformBuffer(modelHandle->TRMatrix.get(), sizeof(TransformationMatrix), modelHandle->TRMatrixBufferOffset);
}

void Renderable::setTRMatrix(std::shared_ptr<TransformationMatrix> newMatrix){
    modelHandle->TRMatrix = newMatrix;
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

int Renderable::getTRBufferID(){
    return modelHandle->TRMatrixID;
}

int Renderable::getTRBufferOffset(){
    return modelHandle->TRMatrixBufferOffset;
}

