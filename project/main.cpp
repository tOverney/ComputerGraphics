#include "icg_common.h"
#include "FrameBuffer.h"
#include "_terrain/terrain.h"
#include "_quad/quad.h"

typedef Eigen::Transform<float,3,Eigen::Affine> Transform;

int WIDTH = 1680;
int HEIGHT = 1001;

mat4 projection_matrix;
mat4 view_matrix;
double y_0;


FrameBuffer fb(WIDTH, HEIGHT);
Terrain terrain;
Quad quad;

vec3 eye = vec3(0.9f, 0.9f, 1.8f);
vec3 center = vec3(0.0f, 0.0f, 0.0f);
vec3 up = vec3(0.0f, 1.0f, 0.0f);
float step = 0.1f;


// Gets called when the windows is resized.
void resize_callback(int width, int height) {
    WIDTH = width;
    HEIGHT = height;

    std::cout << "Window has been resized to " << WIDTH << "x" << HEIGHT << "." << std::endl;
    glViewport(0, 0, WIDTH, HEIGHT);

    projection_matrix = Eigen::perspective(45.0f, (GLfloat)WIDTH / HEIGHT, 0.1f, 100.0f);

}

void init(){
    // Sets background color.
    glClearColor(/*gray*/ .937,.937,.937, /*solid*/1.0);
 
    // Enable depth test.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    view_matrix = Eigen::lookAt(eye, center, up);

    // TODO: initialize framebuffer
    GLuint fb_tex = fb.init();
    quad.init();

    fb.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        quad.draw();
    fb.unbind();


    terrain.init(fb_tex);
    check_error_gl();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const float time = glfwGetTime();

    // Draw a quad on the ground.
    mat4 quad_model_matrix = Eigen::Affine3f(
        Eigen::Translation3f(vec3(0.0f, -0.25f, 0.0f))).matrix();
    terrain.draw(quad_model_matrix, view_matrix, projection_matrix, time);
    

    check_error_gl();

}

// Transforms glfw screen coordinates into normalized OpenGL coordinates.
vec2 transform_screen_coords(int x, int y) {
    return vec2(2.0f * (float)x / WIDTH - 1.0f,
                1.0f - 2.0f * (float)y / HEIGHT);
}

void mouse_button(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        int x_i, y_i;
        glfwGetMousePos(&x_i, &y_i);
        vec2 p = transform_screen_coords(x_i, y_i);
        y_0 = p.y();
    }
}

void mouse_pos(int x, int y) {

    // Zoom
    if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        // TODO 4: Implement zooming. When the right mouse button is pressed,
        // moving the mouse cursor up and down (along the screen's y axis)
        // should zoom out and it. For that you have to update the current
        // 'view_matrix' with a translation along the z axis.
        vec2 p = transform_screen_coords(x, y);

        Transform _M = Transform::Identity();
        _M *= Eigen::Translation3f(0, 0, p.y() - y_0);
        y_0 = p.y();
        mat4 trans_z_mat = _M.matrix();
        view_matrix = trans_z_mat*view_matrix ;
    }
}

void keyboard(int key, int action){
    if(action != GLFW_RELEASE) return; ///< only act on release

    switch(key){
        // up
        case 283:
        eye.z() = eye.z() + step;
        center.z() = center.z() + step;
        break;
        //left
        case 286:
            eye.x() = eye.x() + step;

            center.x() = center.x() + step;
            break;
        //down
        case 284:
        eye.z() = eye.z() - step;

        center.z() = center.z() - step;
            break;

        case 285:
        //right
        eye.x() = eye.x() - step;

        center.x() = center.x() - step;
            break;

        default:
            break;
    }
    view_matrix = Eigen::lookAt(eye, center, up);
}



int main(int, char**){
    glfwInitWindowSize(WIDTH, HEIGHT);
    glfwCreateWindow("MyLittleTerrain");
    glfwDisplayFunc(display);
    glfwSetWindowSizeCallback(&resize_callback);
    glfwSetMouseButtonCallback(mouse_button);
    glfwSetMousePosCallback(mouse_pos);
    glfwSetKeyCallback(keyboard);
    init();
    glfwMainLoop();
    return EXIT_SUCCESS;
}
