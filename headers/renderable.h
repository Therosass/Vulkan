#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <string>
#include <iostream>

class ResourceHandler;
struct ModelHandle;
struct TextureHandle;

struct TransformationMatrix {
    glm::mat4x4 rotate;
    glm::mat4x4 scale;
    glm::mat4x4 translate;
    glm::mat4x4 buffer1;
    void forward(){
        translate[3][0]+=0.01f;
        translate[3][1]+=0.01f;
        translate[3][2]+=0.01f;
    };
    void backward(){
        translate[3][0]-=0.01f;
        translate[3][1]-=0.01f;
        translate[3][2]-=0.01f;
    };
};

struct  Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }   

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                   (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

struct LightObject{
    alignas(16) glm::mat3 lightcoord;
};

class Renderable{
public:
    void init(std::string modelPath, std::shared_ptr<ResourceHandler> handler);
    void createTextureView();
    void createDescriptorSet();
    void updateBufferValue();
    void setTRMatrix(std::shared_ptr<TransformationMatrix> newMatrix);
    int getTRBufferOffset();
    int getTRBufferID();
    int getDescriptorSet();
    int getVertexBuffer();
    int getIndexBuffer();
    int getIndiceAmount();

    
protected:
    std::shared_ptr<ResourceHandler> handler;
    bool isLoaded = false;
    std::string texturePath;
    std::string modelPath;
    unsigned int modelID = 0;
    std::shared_ptr<ModelHandle> modelHandle;
    unsigned int textureID = 0;
    unsigned int textureViewID = 0;
    std::shared_ptr<TextureHandle> textureHandle;
    unsigned int descriptorSetID = 0;
    
private:
    static unsigned int objectCount;

};

#endif