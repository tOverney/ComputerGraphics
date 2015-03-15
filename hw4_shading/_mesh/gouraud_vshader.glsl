// Copyright (C) 2014 - LGG EPFL
#version 330 core 
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light_pos;

in vec3 vpoint;
in vec3 vnormal;

vec4 light_dir;
vec4 view_dir;
vec4 reflect_dir;

out vec3 vcolor;



void main() {
    mat4 MV = view * model;
    vec4 vpoint_mv = MV * vec4(vpoint, 1.0);
    gl_Position = projection * vpoint_mv;
    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 0.1: Gouraud shading.
    /// 1) compute the normal using the model_view matrix.
    /// 2) compute the light direction light_dir.
    /// 3) compute the view direction view_dir.
    /// 4) compute per vertex color
    ///<<<<<<<<<< TODO <<<<<<<<<<<

    /// Computation
    vec4 vnormal_mv = inverse(transpose(MV)) * vec4(vnormal, 1.0);
    light_dir = vec4(light_pos, 1.0) - vpoint_mv;
    view_dir = vpoint_mv;
    reflect_dir = reflect(-light_dir, vec4(vnormal,1.0));

    ///Projection to view_model
    vec4 light_dir_mv = normalize(light_dir);
    vec4 view_dir_mv = normalize(view_dir);
    vec4 reflect_dir_mv = normalize(reflect_dir);
    vnormal_mv = normalize(vnormal_mv);

    //Color computation 
    float NL = dot(vnormal_mv,light_dir_mv);
    float VR = dot(view_dir_mv, reflect_dir_mv);

    if(NL < 0) {
        NL = 0;
    }

    if(VR < 0) {
        VR = 0;
    }

    vcolor = Ia*ka + Id*kd*(dot(vnormal_mv,light_dir_mv)) + Is*ks*pow(dot(view_dir_mv, reflect_dir_mv),p);

}
