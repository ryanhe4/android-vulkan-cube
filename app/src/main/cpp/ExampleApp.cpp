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
        return true;
    }

    void ExampleApp::Update(float dt) {}

    void ExampleApp::UpdateGUI() {
        ImGui::Checkbox("usePerspectiveProjection", &m_usePerspectiveProjection);
    }

    void ExampleApp::Render() {
        if (orientationChanged) {
            onOrientationChange();
        }

        vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE,
                        UINT64_MAX);
        uint32_t imageIndex;
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
        vkResetCommandBuffer(commandBuffers[currentFrame], 0);

        recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

        // SwapChain
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[currentFrame];
        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        VK_CHECK(vkQueueSubmit(graphicsQueue, 1, &submitInfo,
                               inFlightFences[currentFrame]));

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr;

        result = vkQueuePresentKHR(presentQueue, &presentInfo);
        if (result == VK_SUBOPTIMAL_KHR) {
            orientationChanged = true;
        } else if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
        } else {
            assert(result == VK_SUCCESS);  // failed to present swap chain image!
        }
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }
}