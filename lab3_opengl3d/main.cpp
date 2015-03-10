#include "icg_common.h"
#include "cube.h"
#include "_quad/Quad.h"
#include <Eigen/Geometry>

using namespace std;

Cube cube;
Quad quad;

int WIDTH = 800;
int HEIGHT = 600;

mat4 projection_matrix;
mat4 view_matrix;


mat4 LookAt(vec3 eye, vec3 center, vec3 up) {
    // We need a function that converts from world coordinates into camera coordiantes.
    //
    // Cam coords to world coords is given by:
    // X_world = R * X_cam + eye
    //
    // inverting it leads to:
    //
    // X_cam = R^T * X_world - R^T * eye
    //
    // Or as a homogeneous matrix:
    // [ r_00 r_10 r_20 -r_0*eye
    //   r_01 r_11 r_21 -r_1*eye
    //   r_02 r_12 r_22 -r_2*eye
    //      0    0    0        1 ]

    // TODO 3: Create an view matrix that transforms a vector from world space into
    // the camera coordinate system. The camera is located at 'eye' and looks
    // 'center'.

    // r = u x v
    // u = up
    // v = center
    // look_at = r u -v -r x e
    vec3 z_axis = (center - eye).normalized();
    vec3 x_axis = ((z_axis).cross(up.normalized())).normalized();
    vec3 y_axis = x_axis.cross(z_axis);


    mat4 look_at;
    look_at <<  x_axis(0), x_axis(1), x_axis(2), 0,
                y_axis(0), y_axis(1), y_axis(2), 0,
                -z_axis(0), -z_axis(1), -z_axis(2), 0,
                0,0,0,1;
                //-x_axis.dot(eye), -y_axis.dot(eye), -z_axis.dot(eye), 1;
    return look_at;
}

mat4 orthographicProjection() {
    double top (1.0);
    double bottom (-top);
    double right ((WIDTH * (top - bottom)) / (HEIGHT*2));
    double left (-right);
    double far (10);
    double near (-10);

    mat4 matrx = mat4::Identity();
    matrx(0,0) = 2 / (right - left);
    matrx(1,1) = 2 / (top - bottom);
    matrx(2,2) = -2 / (far - near);
    matrx(0,3) = -(right + left) / (right - left);
    matrx(1,3) = -(top + bottom) / (top - bottom);
    matrx(2,3) = -(far + near)/(far - near);

    return matrx;
}

// Gets called when the windows is resized.
void resize_callback(int width, int height) {
    WIDTH = width;
    HEIGHT = height;

    std::cout << "Window has been resized to " << WIDTH << "x" << HEIGHT << "." << std::endl;
    // TODO 1: Reset the OpenGL framebuffer size.
    glViewport(0,0,WIDTH,HEIGHT);

    // TODO 2: Set up an orthographic projection matrix.
    // The projection should depend on the aspect ratio (WIDTH / HEIGHT).
    // You could, e.g., fix 'top/bottom' to 1/-1 and calculate 'left/right' with
    // the given aspect ratio.
    // It might be useful to create a OrthographicProjection function that
    // such a projection matrix.
    projection_matrix = orthographicProjection();
}

void init(){
    // Sets background color.
    glClearColor(/*gray*/ .937,.937,.937, /*solid*/1.0);
    
    cube.init();
    quad.init();

    // Enable depth test.
    glEnable(GL_DEPTH_TEST);

    // TODO 3: Complete the LookAt function and use it here.
    view_matrix = LookAt(vec3(2.0f, 2.0f, 4.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
}

// Gets called for every frame.
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Scaling matrix to scale the cube down to a reasonable size.
    mat4 cube_scale;
    cube_scale << 0.25f, 0.0f,  0.0f,  0.0f,
                  0.0f,  0.25f, 0.0f,  0.0f,
                  0.0f,  0.0f,  0.25f, 0.0f,
                  0.0f,  0.0f,  0.0f,  1.0f;
    // TODO 5: Animate the cube so that it rotates around its Z axis.
    // For that you have to change the 'cube_model_matrix'.
    mat4 cube_model_matrix = cube_scale;

    cube.draw(cube_model_matrix, view_matrix, projection_matrix);

    // Draw a quad on the ground.
    mat4 quad_model_matrix = (Eigen::Affine3f(Eigen::Translation3f(vec3(0.0f, -0.25f, 0.0f))) *
                              Eigen::Affine3f(Eigen::AngleAxisf(M_PI * 0.5f, -vec3::UnitX()))).matrix();
    quad.draw(quad_model_matrix, view_matrix, projection_matrix);

    check_error_gl();
}

int main(int, char**){
    glfwInitWindowSize(WIDTH, HEIGHT);
    glfwCreateWindow("3D Transformations");
    glfwDisplayFunc(display);
    glfwSetWindowSizeCallback(&resize_callback);
    init();
    glfwMainLoop();
    return EXIT_SUCCESS;
}
