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

        // 앞면
        positions.push_back(glm::vec3(-1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(glm::vec3(-1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(glm::vec3(1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(glm::vec3(1.0f, -1.0f, -1.0f) * scale);
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));

        // 왼쪽
        positions.push_back(glm::vec3(-1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(glm::vec3(-1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(glm::vec3(-1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(glm::vec3(-1.0f, -1.0f, -1.0f) * scale);
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
        normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
        normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
        normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));

        // 뒷면
        positions.push_back(glm::vec3(-1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(glm::vec3(-1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(glm::vec3(1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(glm::vec3(1.0f, -1.0f, 1.0f) * scale);
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

        // 아랫면
        positions.push_back(glm::vec3(-1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(glm::vec3(-1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(glm::vec3(1.0f, -1.0f, 1.0f) * scale);
        positions.push_back(glm::vec3(1.0f, -1.0f, -1.0f) * scale);
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));

        // 오른쪽
        positions.push_back(glm::vec3(1.0f, -1.0f, -1.0f) * scale);
        positions.push_back(glm::vec3(1.0f, 1.0f, -1.0f) * scale);
        positions.push_back(glm::vec3(1.0f, 1.0f, 1.0f) * scale);
        positions.push_back(glm::vec3(1.0f, -1.0f, 1.0f) * scale);

        colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        colors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
        colors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
        colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

        normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

        vector<Vertex> vertices;
        for (size_t i = 0; i < positions.size(); i++) {
            Vertex v;
            v.position = positions[i];
            v.color = colors[i];
            vertices.push_back(v);
        }

        vector<uint16_t> indices = {
                0, 1, 2, 0, 2, 3,  // 윗면
                4, 5, 6, 4, 6, 7,   // 앞면
                8, 9, 10, 8, 10, 11,     // 옆면
                12, 14, 13, 12, 15, 14, // 뒷면
                16, 18, 17, 16, 19, 18,  // 아랫면
                20, 21, 22, 20, 22, 23, // 오른면
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

        m_indexCount = indices.size();

        createIndexBuffer(indices, m_indexBuffer, m_indexBufferMemory);

        createUniformBuffers(uniformBuffers, uniformBuffersMemory);

        // descriptorSets: uniformBuffer, Sampler, 혹은 Texture 등 셰이더 리소스 바인딩에 필요
        createDescriptorSets(uniformBuffers);

        AppBase::createGraphicsPipeline(*m_vertexShader, *m_pixelShader, bindingDesc, attrDesc);

        return true;
    }

    void ExampleApp::Update(float dt) {

        static float rot = 0.0f;
        rot += dt;

        const float aspect = (float) swapChainExtent.width / (float) swapChainExtent.height;
        UniformBufferObject ubo{};

        ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, -1.0f)) *
                    glm::rotate(glm::mat4(1.0f), rot, glm::vec3(0.0f, 1.0f, 0.0f)) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(0.25f));

        ubo.view = glm::lookAtLH(
                glm::vec3(0.0f, 0.0f, 1.0f), // EyePosition
                glm::vec3(0.0f, 0.0f, -1.0f),  // FocusPosition
                glm::vec3(0.0f, 1.0f, 0.0f)   // UpDirection
        );

        // 프로젝션
        if (m_usePerspectiveProjection) {
            const float fovAngleY = glm::radians(70.0f);
            ubo.projection =
                    glm::perspectiveLH(fovAngleY, aspect, 0.01f, 100.0f);
        } else {
            ubo.projection =
                    glm::orthoLH(-aspect, aspect, -1.0f, 1.0f, 0.1f, 10.0f);
        }

//        getPrerotationMatrix(pretransformFlag,
//                             ubo.mvp, ratio);

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

        VkBuffer vertexBuffers[] = {m_vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffers[currentFrame], m_indexBuffer, 0,
                             VkIndexType::VK_INDEX_TYPE_UINT16);

//        vkCmdDraw(commandBuffers[currentFrame], 3,1,0,0);
        vkCmdDrawIndexed(commandBuffers[currentFrame], static_cast<uint32_t>(m_indexCount), 1, 0, 0,
                         0);
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