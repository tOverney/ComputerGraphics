#pragma once
#include "icg_common.h"
#include <math.h>
#include <Eigen/Dense>

typedef Eigen::Transform<float,3,Eigen::Affine> Transform;

class Trackball {
public:
    Trackball() : _radius(1.0f) {}

    // This function is called when the user presses the left mouse button down.
    // x, and y are in [-1, 1]. (-1, -1) is the bottom left corner while (1, 1)
    // is the top right corner.
    void begin_drag(float x, float y) {
      _anchor_pos = vec3(x, y, 0.0f);
      project_onto_surface(_anchor_pos);
    }

    // This fucntion is called while the user moves the curser around while the
    // left mouse button is still pressed.
    // x, and y are in [-1, 1]. (-1, -1) is the bottom left corner while (1, 1)
    // is the top right corner.
    // Returns the rotation of the trackball in matrix form.
    mat4 drag(float x, float y) {
      vec3 current_pos = vec3(x, y, 0.0f);
      project_onto_surface(current_pos);


      current_pos.normalize();
      _anchor_pos.normalize();
      vec3 N = _anchor_pos.cross(current_pos);
      double teta = acos(_anchor_pos.dot(current_pos));
      std::cout << "[_anchor_pos.x, _anchor_pos.y, _anchor_pos.z] = " << "[" << _anchor_pos.x() << ", " << _anchor_pos.y() << ", " << _anchor_pos.z() << "]  ";
      std::cout << "[current_pos.x, current_pos.y, current_pos.z] = " << "[" << current_pos.x() << ", " << current_pos.y() << ", " << current_pos.z() << "]  " << endl;
      Transform _M = Transform::Identity();
      _M *= Eigen::AngleAxisf(teta, N);

      mat4 rotation = _M.matrix();
      // TODO 3: Calculate the rotation given the projections of the anocher
      // point and the current position. The rotation axis is given by the cross
      // product of the two projected points, and the angle between them can be
      // used as the magnitude of the rotation.
      // You might want to scale the rotation magnitude by a scalar factor.
      // p.s. No need for using complicated quaternions as suggested inthe wiki
      // article.
      return rotation;
    }

private:
    // Projects the point p (whose z coordiante is still empty/zero) onto the
    // trackball surface. If the position at the mouse cursor is outside the
    // trackball, use a hyberbolic sheet as explained in:
    // https://www.opengl.org/wiki/Object_Mouse_Trackball.
    // The trackball radius is given by '_radius'.
    void project_onto_surface(vec3& p) const {

      // TODO 2: Implement this function. Read above link for details.
      p.y() = -p.y();
      double flag = pow(p.x(),2) + pow(p.y(),2);

      if (flag <= pow(_radius,2)/2.0) {
        p.z() = sqrt(pow(_radius,2) - flag);
      } else {
        p.z() = (pow(_radius,2)/2.0)/sqrt(flag);
      }
      
    }

    double radians(double degree) {
      return degree * M_PI/180;
    }

    float _radius;
    vec3 _anchor_pos;
    mat4 _rotation;
};
