#include "resourceHandler.h"
#include "renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

ResourceHandler::ResourceHandler(Renderer* renderer){
    this->renderer = renderer;
}

int ResourceHandler::loadTexture(std::string texturePath){
    std::cout << "loading texture for: " << texturePath << std::endl;
    auto it = textureStringsMap.find(texturePath);
    if(it != textureStringsMap.end()){
        return it->second;
    }

    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(texturePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = texWidth;
    imageInfo.extent.height = texHeight;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    TextureHandle* handle = new TextureHandle{};
    handle->textureID = renderer->loadImageData(pixels,imageSize,imageInfo);
    handle->imageInfo = imageInfo;
    handle->textureName = texturePath;

    textureMap.insert({handle->textureID, handle});
    textureStringsMap.insert({texturePath,handle->textureID});


    return handle->textureID;
}

int ResourceHandler::loadModel(std::string modelPath) {
    std::cout << "loading model for: " << modelPath << std::endl;

    auto it = modelStringsMap.find(modelPath);
    if(it != modelStringsMap.end()){
        return it->second;
    }

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = {1.0f, 1.0f, 1.0f};

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
    
    ModelHandle* handle = new ModelHandle{};
    handle->modelID = nextModelID;
    handle->indices = indices;
    handle->modelName = modelPath;
    handle->vertices = vertices;
    handle->vertexBufferID = renderer->createBufferObject(vertices.data(), sizeof(vertices[0]) * vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    handle->indexBufferID = renderer->createBufferObject(indices.data(), sizeof(indices[0]) * indices.size(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

    modelMap.insert({nextModelID,handle});
    textureStringsMap.insert({modelPath,nextModelID});

    nextModelID++;
    
    return nextModelID - 1;
}

int ResourceHandler::createUniformBuffer(int size){
    UBOHandle handle = {};
    handle.size = size;
    handle.BufferID = renderer->createUniformBuffer(size);

    bufferObjects.push_back(handle);

    return handle.BufferID;
}

int ResourceHandler::createDescriptorSet(int bufferID, int TextureID){
    auto it = descriptorSetMap.find({bufferID,TextureID});
    if(it != descriptorSetMap.end()){
        return it->second;
    }
    else{
        int descID = renderer->createDescriptorSet(bufferID, TextureID);
        descriptorSetMap.insert({std::pair<int,int>(bufferID,TextureID), descID});
    }

    return 0;
}

TextureHandle* ResourceHandler::getTextureHandle(int textureID){
    auto result = textureMap.find(textureID);
    return result->second;
}

ModelHandle* ResourceHandler::getModelHandle(int modelID){
    auto result = modelMap.find(modelID);
    return result->second;
}
