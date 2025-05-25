//
// Created by 양희찬 on 2025. 5. 11..
//
#ifndef CUBE_APPBASE_H
#define CUBE_APPBASE_H

#include <android/asset_manager.h>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vulkan/vulkan.h>
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_android.h>

#include <vector>
#include <optional>
#include <set>
#include <string>
#include <array>
#include <memory>

namespace cube {

    using std::vector;

#define LOG_TAG "hellovkjni"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define VK_CHECK(x)                           \
  do {                                        \
    VkResult err = x;                         \
    if (err) {                                \
      LOGE("Detected Vulkan error: %d", err); \
      abort();                                \
    }                                         \
  } while (0)

    const int MAX_FRAMES_IN_FLIGHT = 2;

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

    struct ANativeWindowDeleter {
        void operator()(ANativeWindow *window) { ANativeWindow_release(window); }
    };

    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
    };


    class AppBase {
    public:
        bool initialized = false;
        uint32_t currentFrame = 0;
        bool orientationChanged = false;
    public:
        AppBase(android_app *state);

        virtual ~AppBase();

        int Run();

        virtual bool Initialize();

        virtual void UpdateGUI() = 0;

        virtual void Update(float dt) = 0;

        virtual void Render() = 0;

        void reset(ANativeWindow *newWindow, AAssetManager *newManager);

        virtual void cleanup();

    private:
        void initVulkan();

        void cleanupSwapChain();

        void createInstance();

        void createSurface();

        void setupDebugMessenger();

        void pickPhysicalDevice();

        void createLogicalDeviceAndQueue();

        void createSwapChain();

        void createImageViews();

        void createRenderPass();

        void createDescriptorSetLayout();

        void createFramebuffers();

        void createCommandPool();

        void createCommandBuffer();

        void createSyncObjects();

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        bool isDeviceSuitable(VkPhysicalDevice device);

        bool checkValidationLayerSupport();

        std::vector<const char *> getRequiredExtensions(bool enableValidation);

        uint32_t findMemoryType(uint32_t typeFilter,
                                VkMemoryPropertyFlags properties);

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                          VkMemoryPropertyFlags properties, VkBuffer &buffer,
                          VkDeviceMemory &bufferMemory);

        void createDescriptorPool();

        void establishDisplaySizeIdentity();
        void PreUpdate();
        void Present();

        android_app *app;

        bool enableValidationLayers = false;

        const std::vector<const char *> validationLayers = {
                "VK_LAYER_KHRONOS_validation"};
        const std::vector<const char *> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        std::unique_ptr<ANativeWindow, ANativeWindowDeleter> window;

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;

        VkSurfaceKHR surface;

        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D displaySizeIdentity;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkCommandPool commandPool;

        VkRenderPass renderPass;
        VkDescriptorSetLayout descriptorSetLayout;

        VkDescriptorPool descriptorPool;

    protected:
        bool InitGUI();

        void recreateSwapChain();

        void
        beginRender(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkClearValue clearColor, VkClearValue depthStencil);

        void onOrientationChange();

        void updateUniformBuffer(UniformBufferObject &ubo, VkDeviceMemory &uniformBufferMemory) const;

        void createDescriptorSets(vector<VkBuffer> uniformBuffers);

        AAssetManager *assetManager;

        VkShaderModule createShaderModule(const char *file_path);

        template<typename T_VERTEX>
        void createVertexBuffer(const vector<T_VERTEX> &vertices, VkBuffer &vertexBuffer,
                                VkDeviceMemory &vertexBufferMemory) {
            VkDeviceSize bufferSize = sizeof(T_VERTEX) * vertices.size();

            createBuffer(bufferSize,VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                                       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory );

            void* data;
            vkMapMemory(device, vertexBufferMemory, 0,bufferSize, 0, &data);
            memcpy(data, vertices.data(), (size_t) bufferSize);
            vkUnmapMemory(device, vertexBufferMemory);
        }

        void createIndexBuffer(const vector<uint16_t> &indices, VkBuffer &m_indexBuffer, VkDeviceMemory &indexBufferMemory);

        void createGraphicsPipeline(VkShaderModule vertexShader, VkShaderModule pixelShader,
                                    VkVertexInputBindingDescription vertextInputBinding,
                                    std::array<VkVertexInputAttributeDescription, 2> attrDesc
        );

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

        void createUniformBuffers(std::vector<VkBuffer> &uniformBuffers,
                                  std::vector<VkDeviceMemory> &uniformBuffersMemory);

        std::vector<VkDescriptorSet> descriptorSets;
    public:
        VkDevice device;
        VkSwapchainKHR swapChain;
        VkExtent2D swapChainExtent;
        std::vector<VkCommandBuffer> commandBuffers;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkSurfaceTransformFlagBitsKHR pretransformFlag;

        std::vector<VkFence> inFlightFences;
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;

        VkPipeline graphicsPipeline;
        VkPipelineLayout pipelineLayout;

        VkQueue graphicsQueue;
        VkQueue presentQueue;
        uint32_t imageIndex;
    };

}


#endif //CUBE_APPBASE_H
