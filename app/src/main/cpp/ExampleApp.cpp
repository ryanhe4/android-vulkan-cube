//
// Created by 양희찬 on 2025. 5. 11..
//

#include "ExampleApp.h"

namespace cube {
    using namespace std;

    auto MakeBox() {

        vector<glm::vec3> positions;
        vector<glm::vec3> colors;
        vector<glm::vec3> normals;

        const float scale = 1.0f;

        // 윗면
        positions.push_back(glm::vec3(-1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(glm::vec3(-1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(glm::vec3(1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(glm::vec3(1.0f, 1.0f, -1.0f) * scale);
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

        // 아랫면


        // 앞면


        // 뒷면


        // 왼쪽


        // 오른쪽

        vector<Vertex> vertices;
        for (size_t i = 0; i < positions.size(); i++) {
            Vertex v;
            v.position = positions[i];
            v.color = colors[i];
            vertices.push_back(v);
        }

        vector<uint16_t> indices = {
                0, 1, 2, 0, 2, 3,  // 윗면
        };

        return tuple{vertices, indices};
    }

    ExampleApp::ExampleApp(android_app *app) : AppBase(app) {}

    bool ExampleApp::Initialize() {
        if (!AppBase::Initialize()) {
            return false;
        }

        m_vertexShader = make_unique<VkShaderModule>(createShaderModule("shaders/shader.vert.spv"));
        m_pixelShader = make_unique<VkShaderModule>(createShaderModule("shaders/shader.frag.spv"));

        // Create VertexBindingDescription
        VkVertexInputBindingDescription bindingDesc{};
        bindingDesc.binding = 0; // 바인딩 인덱스
        bindingDesc.stride = sizeof(Vertex); // 버텍스 당 데이터 크기
        bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // 버텍스 단위 이동

        std::array<VkVertexInputAttributeDescription, 2> attrDesc{};

        // Position 속성 (location 0)
        attrDesc[0].binding = 0;
        attrDesc[0].location = 0;
        attrDesc[0].format = VK_FORMAT_R32G32B32_SFLOAT; // vec3에 해당
        attrDesc[0].offset = offsetof(Vertex, position);

        // Color 속성 (location 1)
        attrDesc[1].binding = 0;
        attrDesc[1].location = 1;
        attrDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT; // vec3에 해당
        attrDesc[1].offset = offsetof(Vertex, color);


        auto [vertices, indices] = MakeBox();

        createVertexBuffer(vertices, m_vertexBuffer, m_vertexBufferMemory);

        createUniformBuffers(uniformBuffers, uniformBuffersMemory);

        // descriptorSets: uniformBuffer, Sampler, 혹은 Texture 등 셰이더 리소스 바인딩에 필요
        createDescriptorSets(uniformBuffers);

        AppBase::createGraphicsPipeline(*m_vertexShader, *m_pixelShader, bindingDesc, attrDesc);

        return true;
    }

    /*
    * getPrerotationMatrix handles screen rotation with 3 hardcoded rotation
    * matrices (detailed below). We skip the 180 degrees rotation.
    */
    void getPrerotationMatrix(const VkSurfaceTransformFlagBitsKHR &pretransformFlag, glm::mat4 &mat,
                              float ratio) {
        // mat is initialized to the identity matrix
        mat = glm::mat4(1.0f);

        if (pretransformFlag & VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR) {
            // mat is set to a 90 deg rotation matrix around Z axis
            mat = glm::rotate(mat, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        } else if (pretransformFlag & VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR) {
            // mat is set to 270 deg rotation matrix around Z axis
            mat = glm::rotate(mat, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        }

        // scale by screen ratio
        mat = glm::scale(mat, glm::vec3(1.0f, ratio, 1.0f));

        // rotate 1 degree every function call.
        static float currentAngleDegrees = 0.0f;
        currentAngleDegrees += 1.0f;
        mat = glm::rotate(mat, glm::radians(currentAngleDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void ExampleApp::Update(float dt) {
        float ratio = (float) swapChainExtent.width / (float) swapChainExtent.height;

        UniformBufferObject ubo{};

        getPrerotationMatrix(pretransformFlag,
                             ubo.mvp, ratio);

        updateUniformBuffer(ubo, uniformBuffersMemory[currentFrame]);
        vkResetFences(device, 1, &inFlightFences[currentFrame]);
    }

    void ExampleApp::UpdateGUI() {
        ImGui::Checkbox("usePerspectiveProjection", &m_usePerspectiveProjection);
    }

    void ExampleApp::Render() {

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

        AppBase::beginRender(commandBuffers[currentFrame], imageIndex, clearColor);
        VkViewport viewport{};
        viewport.width = (float) swapChainExtent.width;
        viewport.height = (float) swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.extent = swapChainExtent;
        vkCmdSetScissor(commandBuffers[currentFrame], 0, 1, &scissor);

        vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS,
                          graphicsPipeline);
        vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipelineLayout, 0, 1, &descriptorSets[currentFrame],
                                0, nullptr);

        vkCmdDraw(commandBuffers[currentFrame], 3, 1, 0, 0);
    }

    void ExampleApp::cleanup() {
        AppBase::cleanup();

        // TODO: DELETE SHADER
        vkDestroyBuffer(device, m_vertexBuffer, nullptr);
        vkFreeMemory(device, m_vertexBufferMemory, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(device, uniformBuffers[i], nullptr);
            vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
        }
    }
}