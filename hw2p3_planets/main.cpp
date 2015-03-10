#include "icg_common.h"
#include "Quad.h"

#define FULL_CIRCLE 2 * M_PI

typedef Eigen::Transform<float,3,Eigen::Affine> Transform;

class Celestial_object {
public: 
    Quad object;
    float scale;
    float x, y;
    float alpha;
    float angular_speed; 

    Celestial_object() {};

    Celestial_object(float scale, float x, float angular_coef)
            : scale (scale), x (x), alpha (0),
              angular_speed (M_PI / angular_coef) {}

};

/*
    If you want to have moderate fun,
    set the ideal framerate to 10 to enable the potatoe box simulator
*/
const int IDEAL_FRAMERATE = 60;
const float MOON_TO_EARTH = 0.13;
const int NB_PLANETS = 3;

const char* TEXT_NAMES[] = {"sun.tga","earth.tga","moon.tga"};
    
Celestial_object solar_system[NB_PLANETS]; // 0 : sun, 1 : earth, 2 : moon
float last_time = 0;

void init(){
    glClearColor(/*white*/ 1.0f,1.0f,1.0f, /*solid*/1.0 );
    
    solar_system[0] = Celestial_object(0.2, 0.3, 12);  // sun
    solar_system[1] = Celestial_object(0.08,  0,  4);  // earth
    solar_system[2] = Celestial_object(0.03,  0,  2);  // moon
    
    for (int i = 0; i < NB_PLANETS; i++) {
        solar_system[i].object.init(TEXT_NAMES[i]);
    }
}

mat4 transRotScale(float x, float y, float alpha, float scaling_factor) {

    Transform _M = Transform::Identity();
    _M *= Eigen::Translation3f(x, y, 0);
    _M *= Eigen::AngleAxisf(alpha, Eigen::Vector3f::UnitZ());
    _M *= Eigen::AlignedScaling3f(scaling_factor, scaling_factor, scaling_factor);
    
    return _M.matrix();
}

void renderSun(float delta_t, float& alpha, float& x) {
    
    Celestial_object* current = &solar_system[0];

    x = current->x;
    alpha = current->alpha;
    float scaling_fact = (5 + cos(NB_PLANETS * alpha) )/ 5;

    mat4 M = transRotScale(x, 0, alpha, current->scale * scaling_fact);
    current->object.draw(M);

    current->alpha = alpha + current->angular_speed * delta_t;
}

void renderEarth(float delta_t, float alpha, float& x, float& y) {

    x = (x + 0.4) * cos(-alpha);
    y =  0.5 * sin(-alpha);

    Celestial_object* current = &solar_system[1];

    mat4 M = transRotScale(x, y, current->alpha, current->scale);
    current->object.draw(M);

    current->alpha = fmod(current->alpha + current->angular_speed * delta_t, FULL_CIRCLE);
}

void renderMoon(float delta_t, float x, float y) {
    Celestial_object* current = &solar_system[2];
    float alpha = current->alpha;

    x = x + MOON_TO_EARTH * cos(alpha);
    y = y + MOON_TO_EARTH * sin(alpha);
    
    mat4 M = transRotScale(x, y, alpha, current->scale);
    current->object.draw(M);

    current->alpha = fmod(alpha + current->angular_speed * delta_t, FULL_CIRCLE);
}

void display(){
    float time_s = glfwGetTime();
    float delta_t = time_s - last_time;

    if (delta_t <= (1.0/IDEAL_FRAMERATE)) {
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT);
    last_time = time_s;

    float x, y, alpha;

    renderSun(delta_t, alpha, x);
    renderEarth(delta_t, alpha, x, y);
    renderMoon(delta_t, x, y);
}

int main(int, char**){
    glfwInitWindowSize(800, 800);
    glfwCreateWindow("planets");
    glfwDisplayFunc(display);
    init();

    glfwMainLoop();

    for (int i = 0; i < NB_PLANETS; i++) {
        solar_system[i].object.cleanup();
    }

    return EXIT_SUCCESS;
}
