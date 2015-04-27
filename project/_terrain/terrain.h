#pragma once
#include "icg_common.h"

class Terrain{
protected:
    GLuint _vao;          ///< vertex array object
    GLuint _vbo_position; ///< memory buffer for positions
    GLuint _vbo_index;    ///< memory buffer for indice
    GLuint _pid;          ///< GLSL shader program ID
    GLuint _tex;          ///< Texture ID
    GLuint _height_map;   ///< Texture ID of the height map
    GLuint _num_indices;  ///< number of vertices to render
    
public:    
    void init(GLuint height_map = -1){
        // Compile the shaders
        _pid = opengp::load_shaders("_terrain/terrain_vshader.glsl", "_terrain/terrain_fshader.glsl");
        if(!_pid) exit(EXIT_FAILURE);       
        glUseProgram(_pid);
        
        // Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        // Vertex coordinates and indices
        {
            std::vector<GLfloat> vertices;
            std::vector<GLuint> indices;
            // TODO 5: Make a triangle grid with dimension 100x100.
            // Always two subsequent entries in 'vertices' form a 2D vertex position.
            int grid_dim = 512;

            /* e.g. a small grid will be numbered as such in vertices:
                2 1 0
                5 4 3
                8 7 6

                so we'll have to take the indexes in the following order:
                0 3 1 / 1 3 4 / 2 4 5

                and we have to repeat the begining / end of lines to avoid "border"
                effects.
             */

            // indexes
            for (int y = 0; y < grid_dim - 1; ++y) {
                indices.push_back(y * grid_dim);
                for (int x = 0; x < grid_dim; ++x) {
                    indices.push_back(y * grid_dim + x);
                    indices.push_back((y + 1) * grid_dim + x);
                }
                indices.push_back((y + 1) * grid_dim + (grid_dim - 1));
            }

            // positions
            for(int y = 0; y < grid_dim; y++) {
                for(int x = 0; x < grid_dim; x++) {
                    vertices.push_back(1.0f - (2.0 * x)/grid_dim);
                    vertices.push_back(1.0f - (2.0 * y)/grid_dim);
                }
            }

            // The given code below are the vertices for a simple quad.
            // Your grid should have the same dimension as that quad, i.e.,
            // reach from [-1, -1] to [1, 1].

            // Vertex position of the triangles.
            // vertices.push_back(-1.0f); vertices.push_back( 1.0f);
            // vertices.push_back( 1.0f); vertices.push_back( 1.0f);
            // vertices.push_back( 1.0f); vertices.push_back(-1.0f);
            // vertices.push_back(-1.0f); vertices.push_back(-1.0f);

            // And indices.
            // indices.push_back(0);
            // indices.push_back(1);
            // indices.push_back(3);
            // indices.push_back(2);

            _num_indices = indices.size();

            // position buffer
            glGenBuffers(1, &_vbo_position);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo_position);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

            // vertex indices
            glGenBuffers(1, &_vbo_index);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo_index);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

            // position shader attribute
            GLuint loc_position = glGetAttribLocation(_pid, "position");
            glEnableVertexAttribArray(loc_position);
            glVertexAttribPointer(loc_position, 2, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }
        
        // Load texture
        glGenTextures(1, &_tex);
        glBindTexture(GL_TEXTURE_2D, _tex);
        glfwLoadTexture2D("_terrain/terrain_texture.tga", 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        _height_map = (height_map == -1) ? _tex : height_map;
                
        // Texture uniforms
        GLuint tex_id = glGetUniformLocation(_pid, "tex");
        glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);
        GLuint height_map_id = glGetUniformLocation(_pid, "height_map");
        glUniform1i(height_map_id, 1 /*GL_TEXTURE1*/);
        
        // to avoid the current object being polluted
        glBindVertexArray(0);
        glUseProgram(0);
    }
           
    void cleanup(){
        glDeleteBuffers(1, &_vbo_position);
        glDeleteBuffers(1, &_vbo_index);
        glDeleteVertexArrays(1, &_vao);
        glDeleteProgram(_pid);
        glDeleteTextures(1, &_tex);
    }
    
    void draw(const mat4& model, const mat4& view, const mat4& projection, float time){
        glUseProgram(_pid);
        glBindVertexArray(_vao);
        // Bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _tex);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _height_map);

        // Setup MVP
        mat4 MVP = projection*view*model;
        GLuint MVP_id = glGetUniformLocation(_pid, "mvp");
        glUniformMatrix4fv(MVP_id, 1, GL_FALSE, MVP.data());

        // Pass the current time stamp to the shader.
        glUniform1f(glGetUniformLocation(_pid, "time"), time);

        // Draw
        // TODO 5: For debugging it can be helpful to draw only the wireframe.
        // You can do that by uncommenting the next line.
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // TODO 5: Depending on how you set up your vertex index buffer, you
        // might have to change GL_TRIANGLE_STRIP to GL_TRIANGLES.
        glDrawElements(GL_TRIANGLE_STRIP, _num_indices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);        
        glUseProgram(0);
    }
};