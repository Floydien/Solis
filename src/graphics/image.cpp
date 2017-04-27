#include "image.h"

#ifdef __linux__
    #include <SOIL/SOIL.h>
#else
	#ifndef STB_IMAGE_IMPLEMENTATION
    	#define STB_IMAGE_IMPLEMENTATION
	#endif
    #include "stb_image.h"
#endif

std::map<const std::string, std::shared_ptr<Image::ImageData>> Image::imageDataMap;

Image::Image(const std::string &filename) :
        name(std::move(filename)) 
{
    // Check if the image was already loaded into the memory
    auto it = imageDataMap.find(name);
    if(it != imageDataMap.end()) 
    {
        imageData = it->second;
        imageData->addReference();
    } 
    else 
    {

        int32_t width = 0;
        int32_t height = 0;
        int32_t channels = 0;
        uint8_t *img = 0;

        #ifdef __linux__
            img = SOIL_load_image(("res/textures/" + name).c_str(), &width, &height,
                            &channels, SOIL_LOAD_RGBA);
        #else
            img = stbi_load(("res/textures/" + name).c_str(), &width, &height,
                            &channels, 4);
        #endif

        if(img) 
        {
            std::vector<uint8_t> imgVec;
            
        	for(int32_t i = 0; i < width * height * 4; i++) 
        		imgVec.push_back(img[i]);
            
            #ifdef __linux__
                SOIL_free_image_data(img);
            #else
                stbi_image_free(img);
            #endif

            imageData = std::make_shared<Image::ImageData>(width, height, channels, imgVec);
            imageDataMap.insert(std::pair<std::string, std::shared_ptr<Image::ImageData>>(filename, imageData));
        }
    }
}

Image::~Image() {
    auto it = imageDataMap.find(name);
    if (it != imageDataMap.end()) 
    {
        //remove from map if all references are gone
        if(it->second->removeReference()) 
            imageDataMap.erase(it);
    }
}