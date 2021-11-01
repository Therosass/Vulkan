#ifndef LIGHT_H
#define LIGHT_H

struct LightObject {
    alignas(16) glm::vec3 position;

    alignas(16) glm::vec3 color;

    alignas(16) glm::vec3 diffuse;
    alignas(16) glm::vec3 specular;
    
    alignas(4) float constant;
    alignas(4) float linear;
    alignas(4) float quadratic;
};


#endif