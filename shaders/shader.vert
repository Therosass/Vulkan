#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inColor;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 outPos;
layout(location = 3) out vec3 outNormal;
layout(location = 4) out vec3 outCamera;

layout(set = 1, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 buffer0;
} ubo;

layout(set = 1, binding = 2) uniform UniformBufferObject2 {
    mat4 rotationMatrix;
    mat4 scaleMatrix;
    mat4 translationMatrix;
    mat4 buffer1;
} transformationMatrix;


void main() {
    gl_Position = transformationMatrix.rotationMatrix * transformationMatrix.scaleMatrix * transformationMatrix.translationMatrix * vec4(inPosition, 1.0);
    outPos = vec3(gl_Position[0], gl_Position[1], gl_Position[2]);
    outNormal = inNormal;
    gl_Position = ubo.proj * ubo.view * ubo.model * gl_Position;
    outCamera = vec3(ubo.buffer0[0][0], ubo.buffer0[0][1], ubo.buffer0[0][2]);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}

