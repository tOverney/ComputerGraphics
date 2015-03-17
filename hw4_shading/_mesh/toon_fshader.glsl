// Copyright (C) 2014 - LGG EPFL
#version 330 core
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;
uniform sampler1D tex1D;


out vec3 color;

in vec3 view_dir_mv;
in vec3 vnormal_mv;
in vec3 light_dir_mv;


void main() {
    color = vec3(0.0,0.0,0.0);

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 2.2: Toon shading.
    /// 1) compute ambient term.
    /// 2) compute diffuse term using the texture sampler tex.
    /// 3) compute specular term using the texture sampler tex.
    ///<<<<<<<<<< TODO <<<<<<<<<<<
    vec3 reflect_dir_mv =normalize(reflect(-light_dir_mv, vnormal_mv));

    float specular_angle = max(pow(dot(reflect_dir_mv,view_dir_mv),p),0);

    float diffuse_angle = max(dot(vnormal_mv,light_dir_mv),0);

    vec3 ambient = Ia*ka;


    color = ambient + Id*kd*texture(tex1D,diffuse_angle).xyz + Is*ks*texture(tex1D,specular_angle).xyz;

}
