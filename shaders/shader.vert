#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;


layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 buffer0;
} ubo;

layout(binding = 2) uniform UniformBufferObject2 {
    mat4 rotationMatrix;
    mat4 scaleMatrix;
    mat4 translationMatrix;
    mat4 buffer1;
} transformationMatrix;


void main() {
    gl_Position = transformationMatrix.rotationMatrix * transformationMatrix.scaleMatrix * transformationMatrix.translationMatrix * vec4(inPosition, 1.0);
    gl_Position = ubo.proj * ubo.view * ubo.model *  gl_Position;
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}

