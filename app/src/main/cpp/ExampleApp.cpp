//
// Created by 양희찬 on 2025. 5. 11..
//

#include "ExampleApp.h"

namespace cube {
    using namespace std;

    ExampleApp::ExampleApp(android_app *app) : AppBase(app) {}

    bool ExampleApp::Initialize() {
        if(!AppBase:: Initialize()) {
            return false;
        }

        m_vertexShader = make_unique<VkShaderModule>(createShaderModule("shaders/shader.vert.spv"));
        m_pixelShader = make_unique<VkShaderModule>(createShaderModule("shaders/shader.frag.spv"));

        // TODO: DELETE SHADER
        AppBase::createGraphicsPipeline(*m_vertexShader, *m_pixelShader);

        return true;
    }

    void ExampleApp::Update(float dt) {
        if (orientationChanged) {
            onOrientationChange();
        }

        vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE,
                        UINT64_MAX);
        VkResult result = vkAcquireNextImageKHR(
                device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame],
                VK_NULL_HANDLE, &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }
        assert(result == VK_SUCCESS ||
               result == VK_SUBOPTIMAL_KHR);  // failed to acquire swap chain image

        updateUniformBuffer(currentFrame);
        vkResetFences(device, 1, &inFlightFences[currentFrame]);
    }

    void ExampleApp::UpdateGUI() {
        ImGui::Checkbox("usePerspectiveProjection", &m_usePerspectiveProjection);
    }

    void ExampleApp::Render() {
        vkResetCommandBuffer(commandBuffers[currentFrame], 0);
        recordCommandBuffer(commandBuffers[currentFrame], imageIndex);
    }
}