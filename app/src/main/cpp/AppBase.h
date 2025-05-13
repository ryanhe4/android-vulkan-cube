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

#include <vulkan/vulkan.h>
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_android.h>

#include <vector>
#include <optional>
#include <set>
#include <string>

namespace cube {

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

    struct VulkanDeviceInfo {
        bool initialized_;
        bool enableValidationLayers = false;

        VkInstance instance_;
        VkPhysicalDevice gpuDevice_;
        VkDevice device_;

        VkSurfaceKHR surface_;
        VkQueue graphicsQueue_;
        VkQueue presentQueue_;

        const std::vector<const char *> validationLayers = {
                "VK_LAYER_KHRONOS_validation"};

        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct VulkanSwapchainInfo {
        VkSwapchainKHR swapchain_;
        uint32_t swapchainLength_;

        VkExtent2D displaySize_;
        VkFormat displayFormat_;
        VkSurfaceTransformFlagBitsKHR pretransformFlag;

        // array of frame buffers and views
        std::vector<VkImage> displayImages_;
        std::vector<VkImageView> displayViews_;
        std::vector<VkFramebuffer> framebuffers_;
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct VulkanRenderInfo {
        VkRenderPass renderPass_;
        VkCommandPool cmdPool_;
        VkCommandBuffer *cmdBuffer_;
        uint32_t cmdBufferLen_;
        VkSemaphore semaphore_;
        VkFence fence_;
    };

    struct VulkanGfxPipelineInfo {
        VkPipelineLayout layout_;
        VkPipelineCache cache_;
        VkPipeline pipeline_;
    };

    struct VulkanBufferInfo {
        VkBuffer vertexBuf_;
    };

    class AppBase {
    public:
        AppBase(android_app *state);

        virtual ~AppBase();

        int Run();

        virtual bool Initialize();

        virtual void UpdateGUI() = 0;

        virtual void Update(float dt) = 0;

        virtual void Render() = 0;

        bool checkValidationLayerSupport();

        static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    private:
        void CreateVulkanDevice(ANativeWindow *pWindow);
        void CreateSwapChain();

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;

    protected:
        bool InitGUI();

        bool InitVulkan();

    public:
        bool m_orientationChanged = false;
        android_app *app;

        VulkanDeviceInfo m_device;
        VulkanSwapchainInfo m_swapChain;
        VulkanRenderInfo m_render;
        VulkanBufferInfo buffers;
        VulkanGfxPipelineInfo gfxPipeline;
        VkDebugUtilsMessengerEXT debugMessenger;

        const std::vector<const char *> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        std::vector<const char *> getRequiredExtensions() const;

        bool isDeviceSuitable(VkPhysicalDevice device);

        void findQueueFamilies(VkPhysicalDevice device);

        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);


        void createImageViews();

        void createRenderPass();

        void createFramebuffers();
    };
}


#endif //CUBE_APPBASE_H
