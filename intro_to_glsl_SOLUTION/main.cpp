#include "icg_common.h"

#include "Triangle.h"
#include "Quad.h"
Triangle triangle;
Quad quad;

void init(){
    ///--- Sets background color
    glClearColor(/*gray*/ .937,.937,.937, /*solid*/1.0 );
    
    triangle.init();
    quad.init();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    quad.draw();
    triangle.draw();
}

int main(int, char**){
    glfwInitWindowSize(512, 512);
    glfwCreateWindow("intro_to_glsl_SOLUTION");
    glfwDisplayFunc(display);
    init();
    glfwMainLoop();
    triangle.cleanup();
    quad.cleanup();
    return EXIT_SUCCESS;
}
