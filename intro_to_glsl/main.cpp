#include "icg_common.h"

///--- Vertex position of the triangle
const GLfloat vpoint[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,};

void init(){
    ///--- Sets background color
    glClearColor(/*gray*/ .937,.937,.937, /*solid*/1.0 );
    
    ///--- Compile the shaders
    GLuint programID = opengp::load_shaders("triangle_vshader.glsl", "triangle_fshader.glsl");
    if(!programID) exit(EXIT_FAILURE);
    glUseProgram(programID);
    
    ///--- Vertex Array
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    ///--- Vertex Buffer
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vpoint), vpoint, GL_STATIC_DRAW);

    ///--- Position attribute
    GLuint position = glGetAttribLocation(programID, "vpoint"); ///< Fetch Attribute ID for Vertex Positions
    glEnableVertexAttribArray(position); /// Enable it
    glVertexAttribPointer(position, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int, char**){
    glfwInitWindowSize(512, 512);
    glfwCreateWindow("intro_to_glsl");
    glfwDisplayFunc(display);
    init();
    glfwMainLoop();
    return EXIT_SUCCESS;
}
