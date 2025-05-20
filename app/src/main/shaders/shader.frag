#version 450

// Output colour for the fragment
layout(location = 0) out vec4 outColor;
layout(location = 0) in vec4 inColor;

void main() {
   outColor = inColor;
}