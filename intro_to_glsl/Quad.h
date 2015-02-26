#pragma once
#include "icg_common.h"

class Quad{
private:
    GLuint _vao; ///< vertex array object
    GLuint _pid; ///< GLSL shader program ID 
    GLuint _vbo_vpoint; ///< memory buffer
    GLuint _vbo_vtexcoord; ///< memory buffer
    GLuint _tex; ///< Texture ID
public:
    void init(){
        //...
    }
       
    void cleanup(){
        //... 
    }
    
    void draw(){
        //... 
    }
};
