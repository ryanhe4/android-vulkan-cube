# ANDROID CUBE GAME

안드로이드 vulkan을 활용한 큐브 게임개발

## 환경변수 설정

1. ANDROID_NDK_HOME: ndk home root 설정
2. VCPKG_ROOT: vcpkg hoom root 설정

## VCPKG 설치

- imgui 설치

~~~shell
$ vcpkg install 'imgui[android-binding,vulkan-binding]:arm64-android'
~~~

- stb 설치

~~~shell
$ vcpkg install 'stb:arm64-android'
~~~