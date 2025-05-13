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

    void ExampleApp::Render() {}

    void ExampleApp::UpdateGUI() {}
}