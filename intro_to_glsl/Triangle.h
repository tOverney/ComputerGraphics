#pragma once
#include "icg_common.h"

const GLfloat vpoint[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,};

class Triangle{
private:
    GLuint _vao; ///< vertex array object
    GLuint _pid; ///< GLSL shader program ID 
    GLuint _vbo; ///< memory buffer

public:
    void init(){
        ///--- Vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        //---
        _pid = opengp::load_shaders("triangle_vshader.glsl", "triangle_fshader.glsl");
        if(!_pid) {
            exit(EXIT_FAILURE);
        }

        glUseProgram(_pid);
        //---
        // opengp::load_texture         <--- load texture from file

        glGenBuffers(1, &_vbo);             
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vpoint), vpoint, GL_STATIC_DRAW);
        //---
        GLuint position = glGetAttribLocation(_pid, "vpoint"); ///< Fetch Attribute ID for Vertex Positions
        glEnableVertexAttribArray(position); /// Enable it
        glVertexAttribPointer(position, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        //--- To avoid resource pollution, unload resources
        glUseProgram(0);
        glBindVertexArray(0);
    }
       
    void cleanup(){
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
    }
        
    void draw(){
        glUseProgram(_pid);
        glBindVertexArray(_vao);  
            // glActiveTexture          <--- enable certain tex-unit
            // glBindTexture            <--- bind it to CPU memory
            //--- 
        mat4 M = mat4::Identity();
        M(0, 0) = 0.7;

        GLuint M_id = glGetUniformLocation(_pid, "M");
        glUniformMatrix4fv(M_id, 1, DONT_TRANSPOSE, M.data());
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);        
        glUseProgram(0);  
    }
};
