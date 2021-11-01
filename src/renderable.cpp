#include "resourceHandler.h"
#include <iostream>
const std::string MODEL_PATH = "models/UI.obj";
const std::string MODEL_PATH2 = "modeBls/BeeModel.obj";
const std::string TEXTURE_PATH = "textures/UI.png";
const std::string TEXTURE_PATH2 = "textures/black_and_yellow.png";
const std::string MODEL_PATH3 = "models/textured_punk.obj";
const std::string TEXTURE_PATH3 = "textures/untitled.png";
//TODO: read texture from model object file!!!

unsigned int Renderable::objectCount = 0;

void Renderable::init(std::string modelPath, std::shared_ptr<ResourceHandler> handler){
    std::string texturePath;
    if(modelPath == MODEL_PATH){
        texturePath = TEXTURE_PATH;
    }
    else if(modelPath == MODEL_PATH3){
        texturePath = TEXTURE_PATH3;
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
    descriptorSetID = handler->createDescriptorSet(2,textureID);
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

