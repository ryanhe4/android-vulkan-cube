#[[
 Copyright (C) 2022 The Android Open Source Project

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
#]]

cmake_minimum_required(VERSION 3.18.1)
project(hellovkjni)

# Include the GameActivity static lib to the project.
find_package(game-activity REQUIRED CONFIG)
find_package(imgui CONFIG REQUIRED)
find_package(Stb REQUIRED)

set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u \
    Java_com_google_androidgamesdk_GameActivity_initializeNativeCode")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
set(THIRD_PARTY_DIR ../../../../third_party)

add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR=1)

add_library(${PROJECT_NAME} SHARED
    vk_main.cpp
    ExampleApp.cpp
    AppBase.cpp
)

# Import the CMakeLists.txt for the glm library
add_subdirectory(${THIRD_PARTY_DIR}/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
     ${Stb_INCLUDE_DIR}
    ${THIRD_PARTY_DIR}/glm/glm)

# add lib dependencies
target_link_libraries(${PROJECT_NAME} PUBLIC
        vulkan
        game-activity::game-activity_static
        android
        glm
        imgui::imgui
        log)
