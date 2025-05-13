/*
 * Copyright (C) 2022 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

#include "ExampleApp.h"

using namespace std;

/*
 * Shared state for the app. This will be accessed within lifecycle callbacks
 * such as APP_CMD_START or APP_CMD_INIT_WINDOW.
 *
 * We store:
 * struct android_app - a pointer to the Android application handle
 *
 * vkt::HelloVK - a pointer to our (this) Vulkan application in order to call
 *  the rendering logic
 *
 * bool canRender - a flag which signals that we are ready to call the vulkan
 * rendering logic
 *
 */

/*
 * Key events filter to GameActivity's android_native_app_glue. This sample does
 * not use/process any input events, return false for all input events so system
 * can still process them.
 */
//extern "C" bool VulkanKeyEventFilter(const GameActivityKeyEvent *event) {
//  return false;
//}
//extern "C" bool VulkanMotionEventFilter(const GameActivityMotionEvent *event) {
//  return false;
//}

/*
 * Entry point required by the Android Glue library.
 * This can also be achieved more verbosely by manually declaring JNI functions
 * and calling them from the Android application layer.
 */
void android_main(struct android_app *state) {
    cube::ExampleApp exampleApp(state);

    exampleApp.Run();
}