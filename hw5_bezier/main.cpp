#include "icg_common.h"
#include "trackball.h"

#include "_quad/Quad.h"
#include "_cube/cube.h"
#include "_point/point.h"
#include "_bezier/bezier.h"

enum NAVIGATION_MODE {
    TRACKBALL,
    BEZIER
} navmode;

int window_width = 1300;
int window_height = 640;

mat4 projection;
mat4 view;
mat4 model;

GLuint _pid_bezier;
GLuint _pid_point;
GLuint _pid_point_selection;

///----------------- Trackball -----------------
mat4 trackball_matrix;
mat4 old_trackball_matrix;
Trackball trackball;

vec2 transform_screen_coords(int x, int y) {
    return vec2(2.0f * (float)x / window_width - 1.0f,
                1.0f - 2.0f * (float)y / window_height);
}

void trackball_button(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        int x_i, y_i;
        glfwGetMousePos(&x_i, &y_i);
        vec2 p = transform_screen_coords(x_i, y_i);
        trackball.begin_drag(p.x(), p.y());
        old_trackball_matrix = trackball_matrix;  // Store the current state of the model matrix.
    }
}

void trackball_pos(int x, int y) {
    if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        vec2 p = transform_screen_coords(x, y);
        trackball_matrix = trackball.drag(p.x(), p.y()) * old_trackball_matrix;
    }
}
///---------------------------------------------------
Quad quad;

BezierCurve cam_pos_curve;
BezierCurve cam_pos_curve_scnd;
BezierCurve cam_look_curve;
Cube cube;

std::vector<ControlPoint> cam_pos_points;
std::vector<ControlPoint> cam_look_points;
std::vector<BezierCurve> bezier_curves;
int index_last_control_point = 0;
int selected_point;

void init(){
    /// Compile the shaders here to avoid the duplication
    _pid_bezier = opengp::load_shaders("bezier_vshader.glsl", "bezier_fshader.glsl");
    if(!_pid_bezier) exit(EXIT_FAILURE);

    _pid_point = opengp::load_shaders("point_vshader.glsl", "point_fshader.glsl");
    if(!_pid_point) exit(EXIT_FAILURE);

    _pid_point_selection = opengp::load_shaders("point_selection_vshader.glsl", "point_selection_fshader.glsl");
    if(!_pid_point_selection) exit(EXIT_FAILURE);

    glClearColor(1,1,1, /*solid*/1.0 );    
    glEnable(GL_DEPTH_TEST);
    quad.init();
    cube.init();

    ///--- init cam_pos_curve
    cam_pos_curve.init(_pid_bezier);
    bezier_curves.push_back(cam_pos_curve);
    cam_pos_points.push_back(ControlPoint(-0.79, 0.09, 0.2, 0));
    cam_pos_points.push_back(ControlPoint(-0.88, -0.71, 0.2, 1));
    cam_pos_points.push_back(ControlPoint(1.3, -0.8, 0.2, 2));
    cam_pos_points.push_back(ControlPoint(0.71, 0.76, 0.2, 3));
    for (unsigned int i = 0; i < cam_pos_points.size(); i++) {
        cam_pos_points[i].id() = i;
        cam_pos_points[i].init(_pid_point, _pid_point_selection);
        index_last_control_point ++;
    }

    
    ///--- intit second cam_pos_curve
    cam_pos_curve_scnd.init(_pid_bezier);
    bezier_curves.push_back(cam_pos_curve_scnd);
    cam_pos_points.push_back(ControlPoint(0.8, 0.34, 0.6, 1));
    cam_pos_points.push_back(ControlPoint(0.4, 0.33, 0.2, 2));
    cam_pos_points.push_back(ControlPoint(-0.71, -0.76, 0.7, 3));
    for (unsigned int i = index_last_control_point; i < cam_pos_points.size(); i++) {
        cam_pos_points[i].id() = i;
        cam_pos_points[i].init(_pid_point, _pid_point_selection);
        index_last_control_point++;
    }


    for (unsigned int i = 0; (i/3) < bezier_curves.size(); i = i + 3) {
        bezier_curves.at(i/3).set_points(cam_pos_points[i].position(), cam_pos_points[i+1].position(), cam_pos_points[i+2].position(), cam_pos_points[i+3].position());
    }


    ///--- init cam_look_curve
    cam_look_curve.init(_pid_bezier);
    cam_look_points.push_back(ControlPoint(-1.79, 0.1, 0.2, 0));
    cam_look_points.push_back(ControlPoint(0.88, -0.71, 0.62, 1));
    cam_look_points.push_back(ControlPoint(1.3, 0.8, 0.2, 2));
    cam_look_points.push_back(ControlPoint(-0.71, -0.76, -0.2, 3));

    ///===================== TODO =====================
    ///--- TODO H5.3: Set points for cam_look_curve here
    /// Don't forget to set correct point ids.
    /// ===============================================
    for (unsigned int i = 0; i < cam_look_points.size(); i++) {
        cam_look_points[i].id() = i+cam_pos_points.size();
        cam_look_points[i].init(_pid_point, _pid_point_selection);
    }

    cam_look_curve.set_points(cam_look_points[0].position(), cam_look_points[1].position(), cam_look_points[2].position(), cam_look_points[3].position());

  ///--- Setup view-projection matrix
    float ratio = window_width / (float) window_height;
    projection = Eigen::perspective(45.0f, ratio, 0.1f, 10.0f);

    vec3 cam_pos(2.0f, 2.0f, 2.0f);
    vec3 cam_look(0.0f, 0.0f, 0.0f);
    vec3 cam_up(0.0f, 0.0f, 1.0f);

    view = Eigen::lookAt(cam_pos, cam_look, cam_up);
    model = mat4::Identity();
    trackball_matrix = mat4::Identity();
    selected_point = -1;
    navmode = TRACKBALL;
}

bool unproject (int mouse_x, int mouse_y, vec3 &p) {
    ///===================== TODO =====================
    ///--- TODO H5.1: Screen-space unprojection
    /// 1) Compute the inverse of MVP
    /// 2) Find new screen-space coordinates from mouse position
    /// 3) Obtain object coordinates p
    ///================================================

    ///step 1
    mat4 MVP = projection * view * model * trackball_matrix;

    vec4 p4(p.x(), p.y(), p.z(), 1);
    vec4 p_screen = MVP * p4;

    vec2 screen_coords = transform_screen_coords(mouse_x, mouse_y);

    p_screen.x() = screen_coords.x()*p_screen.w();
    p_screen.y() = screen_coords.y()*p_screen.w();


    ///step 2
      mat4 invMVP = MVP.inverse();
      vec4 p_scr = invMVP * p_screen;

      vec3 p3(p_scr.x()/p_scr.w(), p_scr.y()/p_scr.w(), p_scr.z()/p_scr.w());

      p = p3;

    return true;
}

void display(){
    opengp::update_title_fps("shading");
    glViewport(0,0,window_width,window_height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (navmode == BEZIER) {
        vec3 cam_pos(2.0f, 2.0f, 2.0f);
        vec3 cam_look(0.0f, 0.0f, 0.0f);
        vec3 cam_up(0.0f, 0.0f, 1.0f);

        ///===================== TODO =====================
        ///--- TODO H5.3 sample cam_pos and cam_up from cam_pos_curve
        /// and cam_look_curve.
        /// Use glfwGetTime() and wrap it to [0, 1] to get the curve
        /// parameter.
        ///================================================
        float time = glfwGetTime()/5;
        for(unsigned int i = 0; i<bezier_curves.size(); i++ ) {
            bezier_curves.at(i).sample_point(time, cam_pos);
        }
        cam_look_curve.sample_point(time, cam_look);

        mat4 view_bezier = Eigen::lookAt(cam_pos, cam_look, cam_up);
        quad.draw(model, view_bezier, projection);
        cube.draw(model, view_bezier, projection);
    }

    if (navmode == TRACKBALL) {
        for (unsigned int i = 0; i < cam_pos_points.size(); i++) {
            cam_pos_points[i].draw(trackball_matrix * model, view, projection);
        }


        ///===================== TODO =====================
        ///--- TODO H5.3: Draw control points for cam_look_curve
        /// ===============================================


        for (unsigned int i = 0; i < cam_look_points.size(); i ++) {
            cam_look_points[i].draw(trackball_matrix * model, view, projection);
        }

        for (unsigned int i = 0; i<bezier_curves.size(); i++) {
            bezier_curves.at(i).draw(trackball_matrix * model, view, projection);
        }

        cam_look_curve.draw(trackball_matrix * model, view, projection);

        quad.draw(trackball_matrix * model, view, projection);
        cube.draw(trackball_matrix * model, view, projection);
    }
}

void render_selection() {
    glViewport(0,0,window_width,window_height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ///===================== TODO =====================
    ///--- TODO P5.2 Draw control points for selection
    ///================================================

	for (unsigned int i = 0; i < cam_pos_points.size(); i++) {
        cam_pos_points[i].draw_selection(trackball_matrix*model, view, projection);
    }

    ///===================== TODO =====================
    ///--- TODO H5.3 Draw control points for cam_look_curve
    ///================================================
    
    for (unsigned int i = 0; i < cam_look_points.size(); i++) {
        cam_look_points[i].draw_selection(trackball_matrix*model, view, projection);
    }
}

void selection_button(int button, int action) {
    static int x_pressed = 0, y_pressed = 0;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        int x = 0, y = 0;
        glfwGetMousePos(&x, &y);
        x_pressed = x; y_pressed = y;

        render_selection();

        glFlush();
        glFinish();

        unsigned char res[4];
        glReadPixels(x, window_height - y, 1,1,GL_RGBA, GL_UNSIGNED_BYTE, &res);
        selected_point = res[0];

        if (selected_point >= 0 && selected_point < cam_pos_points.size())
            cam_pos_points[selected_point].selected() = true;
        ///===================== TODO =====================
        ///--- TODO H5.3 Process cam_look_points for selection
        ///================================================
        
        if (selected_point >= cam_pos_points.size() && selected_point < cam_look_points.size()+cam_pos_points.size())
            cam_look_points[selected_point-cam_pos_points.size()].selected() = true;


    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        if (selected_point >= 0 && selected_point < cam_pos_points.size()) {
            cam_pos_points[selected_point].selected() = false;
        }
        ///===================== TODO =====================
        ///--- TODO H5.3 Process cam_look_points for selection
        ///================================================
        
        if (selected_point >= cam_pos_points.size() && selected_point < cam_look_points.size() + cam_pos_points.size()) {
            cam_look_points[selected_point - cam_pos_points.size()].selected() = false;
        }


        int x = 0, y = 0;
        glfwGetMousePos(&x, &y);
        if (x == x_pressed && y == y_pressed) {
            return;
        }

        if (selected_point >= 0 && selected_point < cam_pos_points.size()) {
            unproject(x, y, cam_pos_points[selected_point].position());

            for (unsigned int i = 0; (i/3) < bezier_curves.size(); i = i + 3) {
                bezier_curves.at(i/3).set_points(cam_pos_points[i].position(), cam_pos_points[i+1].position(), cam_pos_points[i+2].position(), cam_pos_points[i+3].position());
            }
        }
        
        

        ///===================== TODO =====================
        ///--- TODO H5.3 Update control points of cam_look_curve
        ///================================================

        if (selected_point >= cam_pos_points.size() && selected_point < cam_look_points.size() + cam_pos_points.size()) {
            unproject(x, y, cam_look_points[selected_point-cam_pos_points.size()].position());
            cam_look_curve.set_points(cam_look_points[0].position(), cam_look_points[1].position(), cam_look_points[2].position(), cam_look_points[3].position());
        }
    }
}

void keyboard(int key, int action){
    if(action != GLFW_RELEASE) return; ///< only act on release
    switch(key){
        case '1':
            navmode = TRACKBALL;
            std::cout << "Trackball mode.\n" << std::flush;
            break;
        case '2':
            navmode = BEZIER;
            std::cout << "Bezier mode.\n" << std::flush;
            break;
        default:
            break;
    }
}

void mouse_button(int button, int action) {
    if (navmode == BEZIER) {
        std::cout << "No mouse interaction in Bezier mode.\n" << std::flush;
        return;
    }

    if (glfwGetKey(GLFW_KEY_LSHIFT) == GLFW_PRESS) {
        trackball_button(button, action);
    } else {
        selection_button(button, action);
    }
}

void mouse_pos(int x, int y) {
    if (navmode == BEZIER) {
        return;
    }
    if (glfwGetKey(GLFW_KEY_LSHIFT) == GLFW_PRESS) {
        trackball_pos(x, y);
    }
}
void cleanup(){
    glDeleteProgram(_pid_bezier);
    glDeleteProgram(_pid_point);
    glDeleteProgram(_pid_point_selection);
}

int main(int, char**){
    glfwInitWindowSize(window_width, window_height);
    glfwCreateWindow();
    glfwDisplayFunc(display);
    glfwSetKeyCallback(keyboard);
    glfwSetMouseButtonCallback(mouse_button);
    glfwSetMousePosCallback(mouse_pos);
    init();
    keyboard(GLFW_KEY_KP_1, 0);
    glfwMainLoop();
    cleanup();
    return EXIT_SUCCESS;
}
