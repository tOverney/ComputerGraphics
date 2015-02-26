#pragma once
#include "icg_common.h"

class Triangle{
private:
    GLuint _vao; ///< vertex array object
    GLuint _pid; ///< GLSL shader program ID 
    GLuint _vbo; ///< memory buffer
public:
    void init(){
        // glGenVertexArrays            <--- create vertex array
        // glBindVertexArray            <--- and bind it
        //---
        // compile_shaders              <--- compile the shaders
        // glUseProgram                 <--- bind the shader (for attributes)
        //---
        // opengp::load_texture         <--- load texture from file
        // glGenBuffers                 <--- generate buffer pointer
        // glBindBuffers                <--- make it current
        // glBufferData                 <--- tell it where to find data
        //---
        // glGetAttribLocation          <--- fetch attribute ID
        // glEnableVertexAttribArray    <--- make it the current
        // glVertexAttribPointer        <--- specify layout of attribute
        //--- To avoid resource pollution, unload resources
        // glUseProgram(0)              <--- unbind program (safety!)
        // glBindVertexArray(0)         <--- unbind array (safety!)
    }
       
    void cleanup(){
        // glDelete*(...);
    }
        
    void draw(){
        // glUseProgram(_pid);
        // glBindVertexArray(_vao);  
            // glActiveTexture          <--- enable certain tex-unit
            // glBindTexture            <--- bind it to CPU memory
            //--- 
            // glGetUniformLocation     <--- fetch IDs from shader
            // glUniform*               <--- set their values
            // glDraw*(...)             <--- draws the buffers
        // glBindVertexArray(0);        
        // glUseProgram(0);  
    }
};
