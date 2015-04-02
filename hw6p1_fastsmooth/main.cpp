#include "icg_common.h"
#include "FrameBuffer.h"
#include "_cube/Cube.h"
#include "_quad/Quad.h"
#include "_screenquad/ScreenQuad.h"

int width=1280, height=720;

Cube cube;
Quad quad;

FrameBuffer fb(width, height);
ScreenQuad squad;

void init(){
    glClearColor(1,1,1, /*solid*/1.0 );    
    glEnable(GL_DEPTH_TEST);
    cube.init();
    quad.init();
    GLuint fb_tex = fb.init();
    squad.init(fb_tex);
}

void display(){ 
    opengp::update_title_fps("FrameBuffer");   
    glViewport(0,0,width,height);
    
    ///--- Setup view-projection matrix
    float ratio = width / (float) height;
    static mat4 projection = Eigen::perspective(45.0f, ratio, 0.1f, 10.0f);
    vec3 cam_pos(2.0f, 2.0f, 2.0f);
    vec3 cam_look(0.0f, 0.0f, 0.0f);
    vec3 cam_up(0.0f, 0.0f, 1.0f);
    mat4 view = Eigen::lookAt(cam_pos, cam_look, cam_up);
    mat4 VP = projection * view;
    
    ///--- Render to FB
    fb.clear();
    fb.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube.draw(VP, glfwGetTime());
        quad.draw(VP);
    fb.unbind();

    //fb.display_color_attachment("FB - Color"); ///< debug

    ///--- Render to Window
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    squad.draw();
}

void processKeys(int key, int action) {
    if(action != GLFW_RELEASE) return; ///< only act on release
        if(key == '0') {
            squad.selectMode(0);
            std::cout << "non separable mode" << std::endl;
        }
        if(key == '1') {
            squad.selectMode(1);
            std::cout << "separable mode" << std::endl;
        }

        if(char(key) == 'Q') {
            squad.increase_sigma();
        }

        if(char(key) == 'W') {
            squad.decrease_sigma();
        }
}


int main(int, char**){
    glfwInitWindowSize(width, height);
    glfwCreateWindow();
    glfwDisplayFunc(display);
    glfwSetKeyCallback(processKeys);
    init();
    processKeys(GLFW_KEY_KP_1, 0);
    glfwSwapInterval(0); ///< disable VSYNC (allows framerate>30)
    glfwMainLoop();
    return EXIT_SUCCESS;
}
