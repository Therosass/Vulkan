#ifndef RENDERER_H
#define RENDERER_H

#include <vulkan/vulkan.hpp>
#include "GLFW/glfw3.h"
#include <boost/thread/shared_mutex.hpp>
#include <vk_mem_alloc.h>

#include <boost/thread/condition_variable.hpp>
#include <chrono>
#include <cstdint>
#include <optional>
#include <vector>
#include <array>
#include <fstream>
#include <unordered_map>

#include "module.h"
#include "camera.h"
#include "treeGraph.h"

/****
 * 
 * FORWARD DECLARATIONS
 * 
****/

class Core;

/****
 * 
 * CONSTANT DEFINES
 * 
 * 
****/

static LightObject light;

static bool initLight = false;
static unsigned int lightID = 0;

static std::vector<VkDescriptorSetLayoutBinding> createLayoutInfos(int location){
    std::vector<VkDescriptorSetLayoutBinding> layouts;
    if(location){
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers = nullptr; // Optional  

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutBinding transformLayoutBinding{};
        transformLayoutBinding.binding = 2;
        transformLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        transformLayoutBinding.descriptorCount = 1;
        transformLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        transformLayoutBinding.pImmutableSamplers = nullptr; // Optional
        layouts.push_back(uboLayoutBinding);  
        layouts.push_back(samplerLayoutBinding);  
        layouts.push_back(transformLayoutBinding);
    }
    else{
        VkDescriptorSetLayoutBinding lightLayoutBinding{};
        lightLayoutBinding.binding = 0;
        lightLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        lightLayoutBinding.descriptorCount = 1;
        lightLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        lightLayoutBinding.pImmutableSamplers = nullptr; // Optional
        layouts.push_back(lightLayoutBinding);  
    }
    return layouts;  
};

static unsigned int lightDescriptorsetID = -1;

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
    void start(std::shared_ptr<Core> engineCore, GLFWwindow* window);
    void render(Renderable object);
    void renderScene(const std::vector<TreeNode*>& items);
    void renderUI(const std::vector<TreeNode*>& items);
    Renderer();
    ~Renderer();
    LightObject getLights();

private:

    size_t currentFrame = 0;
    size_t currentFramebuffer = 0;
    bool framebufferResized = false;

public:
     void initVulkan();
private:
    void cleanup();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    bool hasStencilComponent(VkFormat format);
    void createLogicalDevice();
    void createSurface();

    static std::vector<char> readFile(const std::string& filename);

/****
 * 
 * Device and instance creation
 * 
 ****/

    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device; // Logical device
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSurfaceKHR surface; // Window surface object

    void selectPhysicalDevice();
    void createInstance();
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    void setupDebugMessenger();
    std::vector<const char*> getRequiredExtensions();
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

/****
 * 
 * Validation layer and messaging
 * 
 ****/

    VkDebugUtilsMessengerEXT debugMessenger;

    bool checkValidationLayerSupport();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

/****
 * 
 * Synchronization
 * 
 ****/

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    std::vector<VkFence> transferFences;

    void createSyncObjects();

/****
 * 
 * Graphics pipeline
 * 
 ****/

    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline[2];

    VkShaderModule createShaderModule(const std::vector<char>& code);   
    void createGraphicsPipeline(const std::string& vertShaderCode, const std::string& fragShaderCode, int pipelinePos);

/****
 * 
 * Buffer uploading and updating
 * 
 ****/

    std::vector<VkBuffer> bufferObjects;
    std::vector<VmaAllocation> bufferAllocations;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VmaAllocation> uniformBufferAllocations;

public:
    int createBufferObject(void* data, unsigned int size, VkBufferUsageFlags usage);
    int createUniformBuffer(uint32_t size);
    void updateUniformBuffer(unsigned int bufferToUpdate, void* data, unsigned int size, unsigned int offset);

private:

/****
 * 
 *  Image views
 * 
****/

    void createImageViews();

/****
 * 
 *  Framebuffers
 * 
****/

    void createFramebuffers();

/****
 * 
 *  Depth buffers
 * 
****/

    VkImage depthImage;
    VmaAllocation depthImageAllocation;
    VkImageView depthImageView;

    VkFormat findDepthFormat();
    void createDepthResources();

/****
 * 
 *  Descriptors
 * 
****/

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
    size_t boundDescriptorSets = 0;

public:
    int createDescriptorSet(unsigned int uboBufferID, unsigned int textureViewID, unsigned int TransformMatrixID);
    int createDescriptorSet(unsigned int lightBufferID);

private:
    void createDescriptorSetLayout(int location, std::vector<VkDescriptorSetLayoutBinding> bindings);
    void createDescriptorPool();

/****
 * 
 *  Swap chain
 * 
****/

    SwapChainSupportDetails details;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    void createSwapChain();
    void cleanupSwapChain();
    void recreateSwapChain();

/****
 * 
 *  Render Pass
 * 
****/

    VkRenderPass renderPass;

public:
    void beginRenderPass();
    void endRenderPass();
private:
    void createRenderPass();


/****
 * 
 *  Command buffers
 * 
****/

    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    void createCommandPool();
    void createCommandBuffers();
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

/****
 * 
 *  Texture management
 * 
****/

    std::vector<VkImage> textureImages;
    std::vector<VmaAllocation> textureAllocations;
    std::vector<VkImageView> textureImageViews;
    std::vector<VkSampler> textureSamplers;

public:
    int loadImageData(void* data, unsigned int size, VkImageCreateInfo imageInfo);

private:
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    int createImageDescriptorSet(int imageIndex);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    void createTextureSampler();

/****
 *
 *  Vulkan Memory Allocator
 *
****/

    void createVMA();
    VmaAllocator memoryAllocator;

/*****
 * 
 * Camera functions
 * 
 ****/

    Camera camera;

/*****
 * 
 * Module function implementation
 * 
 ****/

    void receiveMessage() override;

/*
    Engine core
*/
    std::shared_ptr<Core> engineCore;

/*
    GLFW window handling
*/  
    GLFWwindow* window = nullptr;

};

#endif