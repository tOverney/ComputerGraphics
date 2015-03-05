#include "icg_common.h"
#include "Triangle.h"

typedef Eigen::Transform<float,3,Eigen::Affine> Transform;

const static int NB_FERMAT = 450;
const static int NB_ARCHIMEDE = 70;

Triangle triangle;
string mode;

void init(){
    glClearColor(/*gray*/ .937,.937,.937, /*solid*/1.0 );
    triangle.init();
}

double radians (double d) {
    return d / 180 * M_PI;
}

mat4 transRotScale(double r, double alpha, double scaling_factor) {

    Transform _M = Transform::Identity();
    _M *= Eigen::Translation3f(r * cos(alpha), r * sin(alpha),0);
    _M *= Eigen::AngleAxisf(alpha, Eigen::Vector3f::UnitZ());
    _M *= Eigen::AlignedScaling3f(scaling_factor, scaling_factor, scaling_factor);
    
    return _M.matrix();
}

void drawFermatSpiral () {

    double c = 0.025;
    double scaling_fact = 0.02;

    for (int n = 1; n < NB_FERMAT; n++) {

        double alpha = radians(n * 137.508);
        double r = c * sqrt(alpha);

        mat4 M (transRotScale(r, alpha, scaling_fact));

        triangle.draw(M);
    }
}

void drawClassicSpiral () {
    
    double b = 0.00000001;

    for (int n = 1; n < NB_ARCHIMEDE; n++) {

        double a = (n * 0.04);
        double alpha = n * M_PI/12;
        double r = (a + b / alpha) / 5;
        double scaling_fact = n * 0.001;

        mat4 M (transRotScale(r, alpha, scaling_fact));

        triangle.draw(M);
    }
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    #ifdef FERMAT
        drawFermatSpiral();
    #else
        drawClassicSpiral();
    #endif
}

int main(int argc, char** argv){

    glfwInitWindowSize(512, 512);
    glfwCreateWindow("spiral");
    glfwDisplayFunc(display);
    init();

    glfwMainLoop();

    triangle.cleanup();
    return EXIT_SUCCESS;
}

