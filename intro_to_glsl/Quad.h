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
        const GLfloat vpoint[] = {
            -1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f, 0.0f};

        const GLfloat vtexcoord[] = { /*V1*/ 0.0f, 0.0f,
                                      /*V2*/ 1.0f, 0.0f,
                                      /*V3*/ 0.0f, 1.0f,
                                      /*V4*/ 1.0f, 1.0f};
        
        ///--- Vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        //---
        _pid = opengp::load_shaders("quad_vshader.glsl", "quad_fshader.glsl");
        if(!_pid) {
            exit(EXIT_FAILURE);
        }

        glUseProgram(_pid);
        //---
        
        // Coordinate buffer
        glGenBuffers(1, &_vbo_vpoint);             
        glBindBuffer(GL_ARRAY_BUFFER, _vbo_vpoint);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vpoint), vpoint, GL_STATIC_DRAW);

        //--- Coordinate loading
        GLuint position = glGetAttribLocation(_pid, "vpoint"); ///< Fetch Attribute ID for Vertex Positions
        glEnableVertexAttribArray(position); /// Enable it
        glVertexAttribPointer(position, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);

        // Texture coordinate buffer
        glGenBuffers(1, &_vbo_vtexcoord);             
        glBindBuffer(GL_ARRAY_BUFFER, _vbo_vtexcoord);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vtexcoord), vtexcoord, GL_STATIC_DRAW);
        

        //--- Texture coordinate loading
        GLuint texture_pos = glGetAttribLocation(_pid, "vtexcoord"); ///< Fetch Attribute ID for Vertex Positions
        glEnableVertexAttribArray(texture_pos); /// Enable it
        glVertexAttribPointer(texture_pos, 2, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);


        // opengp::load_texture         <--- load texture from file
        glGenTextures(1, &_tex);
        glBindTexture(GL_TEXTURE_2D, _tex);
        glfwLoadTexture2D("mrt.tga", 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GLuint tex_id = glGetUniformLocation(_pid, "tex");
        glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);

        //--- To avoid resource pollution, unload resources
        glUseProgram(0);
        glBindVertexArray(0);
    }
       
    void cleanup(){
        glBindVertexArray(0);
        glUseProgram(0);
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo_vpoint);
        glDeleteBuffers(1, &_vbo_vtexcoord);
        glDeleteProgram(_pid);
        glDeleteTextures(1, &_tex);
    }
    
    void draw(){
        glUseProgram(_pid);
        glBindVertexArray(_vao);  
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _tex);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);        
        glUseProgram(0);
    }
};
