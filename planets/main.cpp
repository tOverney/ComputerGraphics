#include "icg_common.h"
#include "Quad.h"

    
Quad sun;
Quad earth;
Quad moon;

void init(){
    glClearColor(/*white*/ 1.0f,1.0f,1.0f, /*solid*/1.0 );
    /// {stuff}.init(...);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    float time_s = glfwGetTime();

    /// {stuff}.draw(...);
}

int main(int, char**){
    glfwInitWindowSize(800, 800);
    glfwCreateWindow("planets");
    glfwDisplayFunc(display);
    init();
    glfwMainLoop();

    //// {stuff}.cleanup()
    return EXIT_SUCCESS;
}
