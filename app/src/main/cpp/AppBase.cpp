//
// Created by 양희찬 on 2025. 5. 11..
//

#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>

#include "AppBase.h"

namespace cube {

    AppBase *g_appBase = nullptr;
    const int MAX_FRAMES_IN_FLIGHT = 2;

    static VkResult CreateDebugUtilsMessengerEXT(
            VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
            const VkAllocationCallbacks *pAllocator,
            VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void HandleInputEvents(struct android_app *app) {
        auto inputBuf = android_app_swap_input_buffers(app);
        if (inputBuf == nullptr) {
            return;
        }

        // For the minimum, apps need to process the exit event (for example,
        // listening to AKEYCODE_BACK). This sample has done that in the Kotlin side
        // and not processing other input events, we just reset the event counter
        // inside the android_input_buffer to keep app glue code in a working state.
        android_app_clear_motion_events(inputBuf);
        android_app_clear_motion_events(inputBuf);
    }

    /**
* Called by the Android runtime whenever events happen so the
* app can react to it.
*/
    void HandleCmd(android_app *app, int32_t cmd) {
//        auto *engine = (VulkanEngine *) app->userData;
        switch (cmd) {
            case APP_CMD_START:
                if (app->window != nullptr) {
                    g_appBase->Initialize();
                }
            case APP_CMD_INIT_WINDOW:
                // The window is being shown, get it ready.
                LOGI("Called - APP_CMD_INIT_WINDOW");
                if (app->window != nullptr) {
                    g_appBase->Initialize();
                }
                break;
            case APP_CMD_TERM_WINDOW:
                // The window is being hidden or closed, clean it up.
                break;
            case APP_CMD_DESTROY:
                LOGI("Destroying");
            default:
                break;
        }
    }

    AppBase::AppBase(struct android_app *state)
            : app{state} {
        app->onAppCmd = HandleCmd;
        g_appBase = this;
    }

    bool AppBase::InitGUI() {
//        IMGUI_CHECKVERSION();
//        ImGui::CreateContext();
//        ImGuiIO &io = ImGui::GetIO();
//        (void) io;

//        if (!ImGui_ImplVulkan_Init()) {
//            return false;
//        }
//
//        if (!ImGui_ImplAndroid_Init()) {
//            return false;
//        }

        return true;
    }

    int AppBase::Run() {
        do {
            int events;
            android_poll_source *source;

            while (ALooper_pollAll(0, nullptr, &events, (void **) &source) >= 0) {
                if (source != nullptr) {
                    source->process(source->app, source);
                }
            }
            // render sth
        } while (app->destroyRequested == 0);

//            ImGui_ImplVulkan_NewFrame();
//            ImGui_ImplAndroid_NewFrame();

//            ImGui::NewFrame();
//            ImGui::Begin("Scene Control");

        // ImGui가 측정해주는 Framerate 출력
//            ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
//                        ImGui::GetIO().Framerate);

//            UpdateGUI(); // 추가적으로 사용할 GUI

//            ImGui::End();
//            ImGui::Render(); // 렌더링할 것들 기록 끝

//            Update(ImGui::GetIO().DeltaTime); // 애니메이션 같은 변화

//            Render(); // 우리가 구현한 렌더링

//            ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData()); // GUI 렌더링

        // Switch the back buffer and the front buffer
        //            // 주의: ImGui RenderDrawData() 다음에 Present() 호출
        //            m_swapChain->Present(1, 0);

    }

    bool AppBase::Initialize() {
        if (!InitVulkan())
            return false;
        return true;
    }

    AppBase::~AppBase() {
        g_appBase = nullptr;
    }

    bool AppBase::InitVulkan() {
        CreateVulkanDevice(app->window);
        CreateSwapChain();
        return true;
    }

    void AppBase::CreateVulkanDevice(ANativeWindow *platformWindow) {
        LOGI("->CreateVulkanDevice");

        assert(!m_device.enableValidationLayers ||
               checkValidationLayerSupport());  // validation layers requested, but not available!
        auto requiredExtensions = getRequiredExtensions();

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();
        createInfo.pApplicationInfo = &appInfo;

        if (m_device.enableValidationLayers) {
            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
            createInfo.enabledLayerCount =
                    static_cast<uint32_t>(m_device.validationLayers.size());
            createInfo.ppEnabledLayerNames = m_device.validationLayers.data();
            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        VK_CHECK(vkCreateInstance(&createInfo, nullptr, &m_device.instance_));

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                               extensions.data());
        LOGI("available extensions");

        for (const auto &extension: extensions) {
            LOGI("\t %s", extension.extensionName);
        }

        // create surface
        assert(platformWindow != nullptr);  // window not initialized
        const VkAndroidSurfaceCreateInfoKHR create_info{
                .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
                .pNext = nullptr,
                .flags = 0,
                .window = platformWindow};

        VK_CHECK(vkCreateAndroidSurfaceKHR(m_device.instance_, &create_info,
                                           nullptr /* pAllocator */, &m_device.surface_));

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_device.instance_, &deviceCount, nullptr);

        assert(deviceCount > 0);  // failed to find GPUs with Vulkan support!

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_device.instance_, &deviceCount, devices.data());

        for (const auto &device: devices) {
            if (isDeviceSuitable(device)) {
                m_device.gpuDevice_ = device;
                break;
            }
        }

        assert(m_device.gpuDevice_ != VK_NULL_HANDLE);  // failed to find a suitable GPU!

        // createLogicalDeviceAndQueue
        findQueueFamilies(m_device.gpuDevice_);
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {m_device.graphicsFamily.value(),
                                                  m_device.presentFamily.value()};
        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount =
                static_cast<uint32_t>(queueCreateInfos.size());
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
        deviceCreateInfo.enabledExtensionCount =
                static_cast<uint32_t>(deviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
        if (m_device.enableValidationLayers) {
            deviceCreateInfo.enabledLayerCount =
                    static_cast<uint32_t>(m_device.validationLayers.size());
            deviceCreateInfo.ppEnabledLayerNames = m_device.validationLayers.data();
        } else {
            deviceCreateInfo.enabledLayerCount = 0;
        }

        VK_CHECK(vkCreateDevice(m_device.gpuDevice_, &deviceCreateInfo, nullptr, &m_device.device_));

        vkGetDeviceQueue(m_device.device_, m_device.graphicsFamily.value(), 0, &m_device.graphicsQueue_);
        vkGetDeviceQueue(m_device.device_, m_device.presentFamily.value(), 0, &m_device.presentQueue_);

        if (!m_device.enableValidationLayers) {
            return;
        }

        VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo{};
        populateDebugMessengerCreateInfo(debugUtilsMessengerCreateInfo);

        VK_CHECK(CreateDebugUtilsMessengerEXT(m_device.instance_, &debugUtilsMessengerCreateInfo, nullptr,
                                              &debugMessenger));
    }

    bool AppBase::checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char *layerName: m_device.validationLayers) {
            bool layerFound = false;
            for (const auto &layerProperties: availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }
        return true;
    }

    const char *toStringMessageSeverity(VkDebugUtilsMessageSeverityFlagBitsEXT s) {
        switch (s) {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                return "VERBOSE";
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                return "ERROR";
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                return "WARNING";
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                return "INFO";
            default:
                return "UNKNOWN";
        }
    }

    const char *toStringMessageType(VkDebugUtilsMessageTypeFlagsEXT s) {
        if (s == (VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT))
            return "General | Validation | Performance";
        if (s == (VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT))
            return "Validation | Performance";
        if (s == (VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT))
            return "General | Performance";
        if (s == (VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT))
            return "Performance";
        if (s == (VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT))
            return "General | Validation";
        if (s == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) return "Validation";
        if (s == VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) return "General";
        return "Unknown";
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                  void * /* pUserData */) {
        auto ms = toStringMessageSeverity(messageSeverity);
        auto mt = toStringMessageType(messageType);
        printf("[%s: %s]\n%s\n", ms, mt, pCallbackData->pMessage);

        return VK_FALSE;
    }

    void AppBase::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    std::vector<const char *> AppBase::getRequiredExtensions() const {
        std::vector<const char *> extensions;
        extensions.push_back("VK_KHR_surface");
        extensions.push_back("VK_KHR_android_surface");
        if (m_device.enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        return extensions;
    }

    bool AppBase::isDeviceSuitable(VkPhysicalDevice device) {
        findQueueFamilies(device);
        bool extensionsSupported = checkDeviceExtensionSupport(device);
        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.formats.empty() &&
                                !swapChainSupport.presentModes.empty();
        }
        return m_device.isComplete() && extensionsSupported && swapChainAdequate;
    }

    void AppBase::findQueueFamilies(VkPhysicalDevice device) {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                                 queueFamilies.data());

        int i = 0;
        for (const auto &queueFamily: queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_device.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_device.surface_, &presentSupport);
            if (presentSupport) {
                m_device.presentFamily = i;
            }

            if (m_device.isComplete()) {
                break;
            }

            i++;
        }
    }

    bool AppBase::checkDeviceExtensionSupport(VkPhysicalDevice device) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                             nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                             availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(),
                                                 deviceExtensions.end());

        for (const auto &extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    SwapChainSupportDetails AppBase::querySwapChainSupport(VkPhysicalDevice device) {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_device.surface_,
                                                  &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_device.surface_, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_device.surface_, &formatCount,
                                                 details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_device.surface_, &presentModeCount,
                                                  nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                    device, m_device.surface_, &presentModeCount, details.presentModes.data());
        }
        return details;
    }

    void AppBase::CreateSwapChain() {
        LOGI("->createSwapChain");

        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device.gpuDevice_, m_device.surface_,
                                                  &capabilities);

        uint32_t width = capabilities.currentExtent.width;
        uint32_t height = capabilities.currentExtent.height;
        if (capabilities.currentTransform & VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR ||
            capabilities.currentTransform & VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR) {
            // Swap to get identity width and height
            capabilities.currentExtent.height = width;
            capabilities.currentExtent.width = height;
        }

        m_swapChain.displaySize_ = capabilities.currentExtent;

        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_device.gpuDevice_);

        auto chooseSwapSurfaceFormat =
                [](const std::vector<VkSurfaceFormatKHR> &availableFormats) {
                    for (const auto &availableFormat : availableFormats) {
                        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                            return availableFormat;
                        }
                    }
                    return availableFormats[0];
                };

        VkSurfaceFormatKHR surfaceFormat =
                chooseSwapSurfaceFormat(swapChainSupport.formats);

        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 &&
            imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }
        m_swapChain.pretransformFlag = swapChainSupport.capabilities.currentTransform;

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_device.surface_;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = m_swapChain.displaySize_;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.preTransform = m_swapChain.pretransformFlag;

        findQueueFamilies(m_device.gpuDevice_);
        uint32_t queueFamilyIndices[] = {m_device.graphicsFamily.value(),
                                         m_device.presentFamily.value()};

        if (m_device.graphicsFamily != m_device.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        VK_CHECK(vkCreateSwapchainKHR(m_device.device_, &createInfo, nullptr, &m_swapChain.swapchain_));

        vkGetSwapchainImagesKHR(m_device.device_, m_swapChain.swapchain_, &imageCount, nullptr);
        m_swapChain.displayImages_.resize(imageCount);
        vkGetSwapchainImagesKHR(m_device.device_, m_swapChain.swapchain_, &imageCount,
                                m_swapChain.displayImages_.data());

        m_swapChain.displayFormat_ = surfaceFormat.format;
//        swapChainExtent =m_swapChain.displaySize_;

        // create Sync Object

        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VK_CHECK(vkCreateSemaphore(m_device.device_, &semaphoreInfo, nullptr,
                                       &imageAvailableSemaphores[i]));

            VK_CHECK(vkCreateSemaphore(m_device.device_, &semaphoreInfo, nullptr,
                                       &renderFinishedSemaphores[i]));

            VK_CHECK(vkCreateFence(m_device.device_, &fenceInfo, nullptr, &inFlightFences[i]));
        }


        createImageViews();
        createRenderPass();
        createFramebuffers();

        LOGI("<-createSwapChain");
    }

    void AppBase::createImageViews() {
        m_swapChain.displayViews_.resize(m_swapChain.displayImages_.size());
        for (size_t i = 0; i < m_swapChain.displayImages_.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_swapChain.displayImages_[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = m_swapChain.displayFormat_;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
            VK_CHECK(vkCreateImageView(m_device.device_, &createInfo, nullptr,
                                       &m_swapChain.displayViews_[i]));
        }
    }

    void AppBase::createRenderPass() {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = m_swapChain.displayFormat_;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        VK_CHECK(vkCreateRenderPass(m_device.device_, &renderPassInfo, nullptr, &m_render.renderPass_));
    }

    void AppBase::createFramebuffers() {
        m_swapChain.framebuffers_.resize(m_swapChain.displayViews_.size());
        for (size_t i = 0; i < m_swapChain.displayImages_.size(); i++) {
            VkImageView attachments[] = {m_swapChain.displayViews_[i]};

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_render.renderPass_;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = m_swapChain.displaySize_.width;
            framebufferInfo.height = m_swapChain.displaySize_.height;
            framebufferInfo.layers = 1;

            VK_CHECK(vkCreateFramebuffer(m_device.device_, &framebufferInfo, nullptr,
                                         & m_swapChain.framebuffers_[i]));
        }
    }
}