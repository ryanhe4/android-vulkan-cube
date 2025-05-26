//
// Created by 양희찬 on 2025. 5. 11..
//

#ifndef CUBE_EXAMPLEAPP_H
#define CUBE_EXAMPLEAPP_H

#include <iostream>

#include "AppBase.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cube {

    // 이 예제에서 사용하는 Vertex 정의
    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;
    };

    class ExampleApp : public AppBase {
    public:
        ExampleApp(android_app *app);

        virtual bool Initialize() override;

        virtual void UpdateGUI() override;

        virtual void Update(float dt) override;

        virtual void Render() override;

        virtual void cleanup() override;
    protected:
        bool m_usePerspectiveProjection = true;
        std::unique_ptr<VkShaderModule> m_vertexShader;
        std::unique_ptr<VkShaderModule> m_pixelShader;

        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;

        VkBuffer m_vertexBuffer;
        VkDeviceMemory m_vertexBufferMemory;

        VkBuffer m_indexBuffer;
        VkDeviceMemory m_indexBufferMemory;
        int m_indexCount;

        glm::vec3 m_modelTranslation = glm::vec3(0.0f);
        glm::vec3 m_modelRotation = glm::vec3(0.0f);
        glm::vec3 m_modelScaling = glm::vec3(0.25f);
        glm::vec3 m_viewEyePos = {0.0f, 0.0f, 2.0f};
        glm::vec3 m_viewEyeDir = {0.0f, 0.0f, -1.0f};
        glm::vec3 m_viewUp = {0.0f, -1.0f, 0.0f};
        float m_projFovAngleY = 70.0f;
        float m_nearZ = 0.01f;
        float m_farZ = 100.0f;
    };

}


#endif //CUBE_EXAMPLEAPP_H
