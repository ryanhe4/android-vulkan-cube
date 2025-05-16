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

    class ExampleApp : public AppBase {
    public:
        ExampleApp(android_app *app);

        virtual bool Initialize() override;

        virtual void UpdateGUI() override;

        virtual void Update(float dt) override;

        virtual void Render() override;

    protected:
        bool m_usePerspectiveProjection = true;
        std::unique_ptr<VkShaderModule> m_vertexShader;
        std::unique_ptr<VkShaderModule> m_pixelShader;
    };

}


#endif //CUBE_EXAMPLEAPP_H
