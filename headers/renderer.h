#ifndef RENDERER_H
#define RENDERER_H

#include <vulkan/vulkan.hpp>
#include "GLFW/glfw3.h"
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
#include "renderable.h"

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
    void beginRenderPass();
    void endRenderPass();
    Renderer();
    ~Renderer();

private:

    size_t currentFrame = 0;
    size_t currentFramebuffer = 0;
    bool framebufferResized = false;

    void initVulkan();
    void cleanup();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    bool hasStencilComponent(VkFormat format);
    void createLogicalDevice();
    void createSurface();
    void drawFrame();

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
    VkPipeline graphicsPipeline;

    VkShaderModule createShaderModule(const std::vector<char>& code);   
    void createGraphicsPipeline();

/****
 * 
 * Buffer uploading and updating
 * 
 ****/

    std::vector<VkBuffer> bufferObjects;
    std::vector<VmaAllocation> bufferAllocations;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    int createBufferObject(void* data, unsigned int size, VkBufferUsageFlags usage);
    int createUniformBuffer(uint32_t size);
    void updateUniformBuffer(unsigned int bufferToUpdate);
    std::pair<int,int> uploadVertexData(std::vector<Vertex> vertices);

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
    VkDeviceMemory depthImageMemory;
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
    VkDescriptorSetLayout descriptorSetLayout;
    size_t boundDescriptorSets = 0;

    void createDescriptorSetLayout();
    void createDescriptorPool();
    int createDescriptorSet(unsigned int uboBufferID, unsigned int textureViewID);

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

    void createRenderPass();
    void beginRenderPassFunc();
    void endRenderPassFunc();

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

    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    int loadImageData(void* data, unsigned int size, VkImageCreateInfo imageInfo);
    int createImageDescriptorSet(int imageIndex);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    void createTextureSampler();
    int uploadTextureData();

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
    void loadModel();

/*****
 * 
 * Module function implementation
 * 
 ****/

    void receiveMessage() override;

/*
    Engine core
*/
    Core* engineCore;

/*
    GLFW window handling
*/  
    GLFWwindow* window = nullptr;

};

#endif