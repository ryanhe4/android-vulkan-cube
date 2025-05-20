#version 450

// Uniform buffer containing an MVP matrix.
// Currently the vulkan backend only sets the rotation matix
// required to handle device rotation.
layout(set = 0, binding = 0) uniform UniformBufferObject {
   mat4 model;
   mat4 view;
   mat4 projection;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 0) out vec3 fragColor;

void main() {
//   gl_Position = ubo.MVP * vec4(positions[gl_VertexIndex], 0.0, 1.0);
//   gl_Position = vec4(positions[gl_VertexIndex], 1.0);
   vec4 pos = vec4(inPosition, 1.0);
   pos = ubo.model * pos;
   pos = ubo.view * pos;
   pos = ubo.projection * pos;

   gl_Position = pos;
   fragColor = inColor;
}