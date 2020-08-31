#ifndef RESOURCEHANDLER_H
#define RESOURCEHANDLER_H

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <string>
#include <vulkan/vulkan.hpp>
#include <boost/bimap.hpp>

struct textureHandle{
    int textureHandle;
    std::string textureName;
    int arrayLocation;
};

class ResourceHandler{
    void loadTexture(std::string texturePath);
    const textureHandle& getTexture(int textureID);
    const textureHandle& getTexture(std::string textureName);
    int getTextureID(std::string textureName);
private:
    boost::bimap<int,textureHandle> textureMap;

};

#endif