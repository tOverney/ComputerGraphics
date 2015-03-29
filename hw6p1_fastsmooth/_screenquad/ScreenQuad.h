#pragma once
#include "icg_common.h"

class ScreenQuad{
protected:
    GLuint _vao; ///< vertex array object
    GLuint _pid; ///< GLSL shader program ID 
    GLuint _vbo; ///< memory buffer
    GLuint _tex; ///< Texture ID
    GLuint _mode;
    float sigma;

private: 
    void gaussian1D(float sig) {

        GLfloat stdev = sqrt(sig);
        //GLuint size = 1 + 2 * 3 * int( ceil(stdev) );
        int size = 5;
        float filter1D[size];
        float weight = 0;
        
        for (int i = -size; i <= size; i++) {
            float val = exp(-(i*i)/(2.0*sig));
            filter1D[i +size] = val;
            weight += val;
        }

        for(int i = 0; i <= 2*size + 1; i++) {
            filter1D[i] /= weight;
        }

        //GLint size_id = glGetUniformLocation(_pid, "size");
        //glUniform1ui(size_id, size);

        GLint filter_id = glGetUniformLocation(_pid, "filter1D");
        glUniform1fv(filter_id, size, filter1D);
    }

public:
    void init(GLuint texture){ 
        
        ///--- Compile the shaders
        _pid = opengp::load_shaders("_screenquad/ScreenQuad_vshader.glsl", "_screenquad/ScreenQuad_fshader.glsl");
        if(!_pid) exit(EXIT_FAILURE);       
        glUseProgram(_pid);
        
        ///--- Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
     
        ///--- Vertex coordinates
        {
            const GLfloat vpoint[] = { /*V1*/ -1.0f, -1.0f, 0.0f, 
                                       /*V2*/ +1.0f, -1.0f, 0.0f, 
                                       /*V3*/ -1.0f, +1.0f, 0.0f,
                                       /*V4*/ +1.0f, +1.0f, 0.0f };        
            ///--- Buffer
            glGenBuffers(1, &_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vpoint), vpoint, GL_STATIC_DRAW);
        
            ///--- Attribute
            GLuint vpoint_id = glGetAttribLocation(_pid, "vpoint");
            glEnableVertexAttribArray(vpoint_id);
            glVertexAttribPointer(vpoint_id, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }
        
        ///--- Texture coordinates
        {
            const GLfloat vtexcoord[] = { /*V1*/ 0.0f, 0.0f, 
                                          /*V2*/ 1.0f, 0.0f, 
                                          /*V3*/ 0.0f, 1.0f,
                                          /*V4*/ 1.0f, 1.0f}; 
            
            ///--- Buffer
            glGenBuffers(1, &_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vtexcoord), vtexcoord, GL_STATIC_DRAW);
        
            ///--- Attribute
            GLuint vtexcoord_id = glGetAttribLocation(_pid, "vtexcoord");
            glEnableVertexAttribArray(vtexcoord_id);
            glVertexAttribPointer(vtexcoord_id, 2, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }
        
        ///--- Load/Assign texture
        this->_tex = texture;
        glBindTexture(GL_TEXTURE_2D, _tex);
        GLuint tex_id = glGetUniformLocation(_pid, "tex");
        glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);
    
        
        ///--- to avoid the current object being polluted
        glBindVertexArray(0);
        glUseProgram(0);

        ///--- Initialization common variable for control
        sigma = 2.0f;
    }
       
    void cleanup(){
        // TODO cleanup
    }

    void blurMethod() {
        GLuint mode_id = glGetUniformLocation(_pid, "mode");
        glUniform1ui(mode_id,_mode);

        if(_mode == 1) {
            gaussian1D(sigma);
        }
    }

    
    void draw(){
        glUseProgram(_pid);
        glBindVertexArray(_vao);      
            glUniform1f(glGetUniformLocation(_pid, "tex_width"), _width);
            glUniform1f(glGetUniformLocation(_pid, "tex_height"), _height); 
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _tex);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);        
        glBindVertexArray(0);        
        glUseProgram(0);
    }

    void selectMode(int mode_id) {
        _mode = mode_id;
    }

};
