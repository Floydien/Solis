#include "image.h"

#ifdef __linux__
    #include <SOIL/SOIL.h>
#else
	#ifndef STB_IMAGE_IMPLEMENTATION
    	#define STB_IMAGE_IMPLEMENTATION
	#endif
    #include "stb_image.h"
#endif

Image::Image(const std::string &filename) {
	name = filename;

    uint8_t *data = 0;

    #ifdef __linux__
        data = SOIL_load_image(("res/textures/" + name).c_str(), &width, &height,
                        &channels, SOIL_LOAD_RGBA);
    #else
        data = stbi_load(("res/textures/" + name).c_str(), &width, &height,
                        &channels, 4);
    #endif

    if(data) {
    	for(int64_t i = 0; i < width * height * channels; i++) {
    		imageData.push_back(data[i]);
    	}
    } else {

    }

    #ifdef __linux__
        SOIL_free_image_data(data);
    #else
        stbi_image_free(data);
    #endif
}

Image::~Image() {

}