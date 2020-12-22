#ifndef RESOURCEHANDLER_H
#define RESOURCEHANDLER_H

#include <string>
#include <vulkan/vulkan.hpp>
#include <unordered_map>
#include <vector>
#include <boost/functional/hash/hash.hpp>

#include "renderable.h"

class Renderer;

struct TextureHandle{
    int textureID;
    std::string textureName;
    VkImageCreateInfo imageInfo;
};

struct ModelHandle{
    int modelID;
    std::string modelName;
    std::vector<Vertex> vertices;
    int vertexBufferID;
    std::vector<uint32_t> indices;
    int indexBufferID;
};

struct UBOHandle{
    int BufferID;
    int size;
};

class ResourceHandler{
public:
    ResourceHandler(std::shared_ptr<Renderer> renderer);
    int loadTexture(std::string texturePath);
    int loadModel(std::string modelPath);
    int createUniformBuffer(int size);
    int createDescriptorSet(int bufferID, int TextureID);
    std::shared_ptr<TextureHandle> getTextureHandle(int textureID);
    std::shared_ptr<ModelHandle> getModelHandle(int textureID);
    int getTextureID(std::string textureName);

private:
    std::unordered_map<int,std::shared_ptr<TextureHandle>> textureMap;
    std::unordered_map<std::string,int> textureStringsMap;
    int nextTextureID = 0;

    std::unordered_map<int,std::shared_ptr<ModelHandle>> modelMap;
    std::unordered_map<std::string,int> modelStringsMap;
    int nextModelID = 0;

    std::vector<UBOHandle> bufferObjects;

    std::unordered_map<std::pair<int,int>,int,boost::hash<std::pair<int,int>>> descriptorSetMap;

    std::shared_ptr<Renderer> renderer;

};

#endif