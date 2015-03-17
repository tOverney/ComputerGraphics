// Copyright (C) 2014 - LGG EPFL
#version 330 core
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

out vec3 color;

in vec3 view_dir_mv;
in vec3 vnormal_mv;
in vec3 light_dir_mv;

void main() {
    color = vec3(0.0,0.0,0.0);

    const vec3 COLORS[6] = vec3[](
        vec3(1.0,0.0,0.0),
        vec3(0.0,1.0,0.0),
        vec3(0.0,0.0,1.0),
        vec3(1.0,1.0,0.0),
        vec3(0.0,1.0,1.0),
        vec3(1.0,0.0,1.0));
    int index = int( mod(gl_PrimitiveID,6) );
    color = COLORS[index];

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 1.2: Phong shading.
    /// 1) compute ambient term.
    /// 2) compute diffuse term.
    /// 3) compute specular term.
    ///<<<<<<<<<< TODO <<<<<<<<<<<
    vec3 reflect_dir_mv =normalize(reflect(-light_dir_mv, vnormal_mv));
    
    float specular_angle = max(pow(dot(reflect_dir_mv,view_dir_mv),p),0);
    vec3 specular = Is*ks*pow(specular_angle,p);

    float diffuse_angle = max(dot(vnormal_mv,light_dir_mv),0);
    vec3 diffuse = Id*kd*diffuse_angle;

    vec3 ambient = Ia*ka;

    color = ambient + diffuse + specular;



}
