#include "icg_common.h"
#include "Triangle.h"
Triangle triangle;

void init(){
    glClearColor(/*gray*/ .937,.937,.937, /*solid*/1.0 );
    triangle.init();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    triangle.draw(mat4::Identity());
}

int main(int, char**){
    glfwInitWindowSize(512, 512);
    glfwCreateWindow("spiral");
    glfwDisplayFunc(display);
    init();
    glfwMainLoop();
    triangle.cleanup();
    return EXIT_SUCCESS;
}

