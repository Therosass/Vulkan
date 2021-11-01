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
    std::shared_ptr<TransformationMatrix> TRMatrix;
    unsigned int TRMatrixID;
    unsigned int TRMatrixBufferOffset;
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
    int loadModel(std::string modelPath, int objectCount);
    void updateUniformBuffer(unsigned int bufferToUpdate, void* data, unsigned int size, unsigned int offset);
    void updateUniformBuffer(void* data, unsigned int size, unsigned int offset);
    std::shared_ptr<TextureHandle> getTextureHandle(int textureID);
    std::shared_ptr<ModelHandle> getModelHandle(int textureID);
    int getTextureID(std::string textureName);

private:
    std::unordered_map<int,std::shared_ptr<TextureHandle>> textureMap;
    std::unordered_map<std::string,int> textureStringsMap;
    static int nextTextureID;

    std::unordered_map<int,std::shared_ptr<ModelHandle>> modelMap;
    std::unordered_map<std::string,int> modelStringsMap;
    static int nextModelID;

    static int TRMatrixBufferID;

    std::vector<UBOHandle> bufferObjects;

    std::unordered_map<std::pair<int,int>,int,boost::hash<std::pair<int,int>>> descriptorSetMap;

    std::shared_ptr<Renderer> renderer;

};

#endif