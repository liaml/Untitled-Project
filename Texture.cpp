#include "Texture.hpp"
#include <iostream>
#include <assert.h>
#include <SOIL2.h>
#include <glew.h>

namespace texLdr {
    
    bool loadTexture(const char* filePath, unsigned int* genTexID) {
        
        *genTexID = SOIL_load_OGL_texture(filePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
        
        if (*genTexID == 0) {
            std::cout << "Loading textures path failed: " << filePath << std::endl;
        }
        
        return* genTexID != 0;
    }
}