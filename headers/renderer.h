#ifndef RENDERER_H
#define RENDERER_H

#include <vulkan/vulkan.hpp>
#include "GLFW/glfw3.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <chrono>
#include <cstdint>
#include <optional>
#include <vector>
#include <array>
#include <fstream>
#include <unordered_map>

#include "module.h"

/****
 * 
 * FORWARD DECLARATIONS
 * 
****/

class Core;

/****
 * 
 * STRUCT DEFINITIONS   
 * NON VULKAN ONLY
 * 
****/

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


/****
 * 
 * CONSTANT DEFINES
 * 
 * 
****/


const std::string MODEL_PATH = "models/viking.obj";
const std::string TEXTURE_PATH = "textures/viking.png";

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};









class Renderer : public Module{
public:
    void start(Core* engineCore, GLFWwindow* window);
    void renderFrame();
    ~Renderer();

private:

/*
    Engine core functions
*/
    Core* engineCore;

/*
    GLFW window handling
*/  
    GLFWwindow* window = nullptr;
    bool windowResizedFlag = false;
    void cleanup();


/*
    Vulkan objects
*/
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    SwapChainSupportDetails details;
    VkDevice device; // Logical device
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSurfaceKHR surface; // Window surface object
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    std::vector<VkFence> transferFences;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Vertex> bg_vertices = {
        {{-1.0f, -1.0f, -1.0f},{1.0f, 1.0f, 1.0f},{0.0f, 0.0f}}, //top left behind 0
        {{-1.0f, -1.0f, +1.0f},{1.0f, 1.0f, 1.0f},{0.0f, 0.0f}}, //top left front 1
        {{-1.0f, +1.0f, -1.0f},{1.0f, 1.0f, 1.0f},{0.0f, 0.0f}}, //top right behind 2
        {{-1.0f, +1.0f, +1.0f},{1.0f, 1.0f, 1.0f},{0.0f, 0.0f}}, //top right front 3
        {{+1.0f, -1.0f, -1.0f},{1.0f, 1.0f, 1.0f},{0.0f, 0.0f}}, //bottom left behind 4
        {{+1.0f, -1.0f, +1.0f},{1.0f, 1.0f, 1.0f},{0.0f, 0.0f}}, //bottom left front 5
        {{+1.0f, +1.0f, -1.0f},{1.0f, 1.0f, 1.0f},{0.0f, 0.0f}}, //bottom right behind 6
        {{+1.0f, +1.0f, +1.0f},{1.0f, 1.0f, 1.0f},{0.0f, 0.0f}} //bottom right front 7
        };

    std::vector<uint32_t> bg_indices = {
        0,1,3,0,3,2, // top
        2,4,0,2,6,4, // behind
        0,4,5,0,5,1, // left
        5,4,6,5,6,7, // bottom
        3,7,6,3,6,2, // right
        1,5,7,1,7,3  // front
        };
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer bgBuffer;
    VkDeviceMemory bgBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    size_t currentFrame = 0;
    bool framebufferResized = false;




/*
    Vulkan functions
*/
    void initVulkan();
    void createInstance();
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    bool checkValidationLayerSupport();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();
    std::vector<const char*> getRequiredExtensions();
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    void selectPhysicalDevice();
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    void createSwapChain();
    void cleanupSwapChain();
    void recreateSwapChain();
    void createImageViews();
    VkShaderModule createShaderModule(const std::vector<char>& code);   
    void createGraphicsPipeline();
    void createRenderPass();
    void createFramebuffers();
    void createCommandPool();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createIndexBuffer();
    void createVertexBuffer();
    void createBgBuffer();
    void createUniformBuffers();
    void updateUniformBuffer(uint32_t currentImage);
    void createDescriptorPool();
    void createDescriptorSets();
    void createCommandBuffers();
    void createSyncObjects();
    void createDescriptorSetLayout();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    bool hasStencilComponent(VkFormat format);
    VkFormat findDepthFormat();
    void createDepthResources();

    void createLogicalDevice();
    void createSurface();

    void drawFrame();

    static std::vector<char> readFile(const std::string& filename);

/****
 * 
 *  Texture functions
 * 
****/
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    void createTextureImage();
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    void createTextureImageView();
    void createTextureSampler();

/*****
 * 
 * Model loading
 * 
 ****/


/*****
 * 
 * Camera functions
 * 
 ****/

    static glm::vec3 cameraPos;
    static glm::vec3 cameraDir;
    static glm::vec3 cameraRight;
    static glm::vec3 cameraUp;
    static glm::vec3 lookPos;

    void setCamera();
    static glm::vec3 readNewCameraPos(std::string posAsString);

    void loadModel();

/*****
 * 
 * Module function implementation
 * 
 ****/

    void receiveMessage() override;
};

#endif