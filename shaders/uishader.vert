#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inColor;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
	//const vec3 positions[4] = vec3[4](
	//	vec3(-1.0f, -1.0f, 0.0f),
	//	vec3(-1.0f, 1.0f, 0.0f),
	//	vec3(1.0f, 0.2f, 0.0f),
	//	vec3(1.0f, 0.0f, 0.0f)
	//);
	//gl_Position = vec4(positions[gl_VertexIndex], 1.0f);
    gl_Position =  vec4(inPosition, 1.0f);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}

