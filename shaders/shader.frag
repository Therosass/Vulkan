#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 vertPos;
layout(location = 3) in vec3 vertNormal;
layout(location = 4) in vec3 cameraPos;

layout(location = 0) out vec4 outColor;
layout(set = 0, binding = 0) uniform lightData{
    vec3 lightCoord;

    vec3 color;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
} light;

layout(set = 1, binding = 1) uniform sampler2D texSampler;

#define NR_POINT_LIGHTS 1

vec3 CalcPointLight(vec3 normal, vec3 fragPos, vec3 viewPos) 
{
    float ambientLight = 0.0f;
    vec3 lightDir = normalize(light.lightCoord - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    float distance    = length(light.lightCoord - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
            light.quadratic * (distance * distance));
    vec3 ambient  = ambientLight  * vec3(texture(texSampler, fragTexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texSampler, fragTexCoord));
    vec3 specular = light.specular * spec * vec3(texture(texSampler, fragTexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main() {
    
    outColor = texture(texSampler, fragTexCoord);
    vec3 normal = normalize(vertNormal);
    vec3 result = vec3(0.0,0.0,0.0);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        //result = vertNormal;
        result += CalcPointLight(vertNormal, vertPos, cameraPos);
        //result = vec3(abs(cameraPos.z), 0.2f, 0.2f);
    outColor  = vec4(result,1.0);
}