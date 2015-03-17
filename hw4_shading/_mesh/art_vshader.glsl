// Copyright (C) 2014 - LGG EPFL
#version 330 core 
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light_pos;

in vec3 vpoint;
in vec3 vnormal;

out vec3 view_dir_mv;
out vec3 vnormal_mv;
out vec3 light_dir_mv;

//Same vshader as phong

void main() {
    mat4 MV = view * model;
    vec4 vpoint_mv = MV * vec4(vpoint, 1.0);
    gl_Position = projection * vpoint_mv;
    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 1.1: artistic shading.
    /// 1) compute normal_mv using the model_view matrix.
    /// 2) compute the light direction light_dir.
    /// 3) compute the view direction view_dir.
    ///<<<<<<<<<< TODO <<<<<<<<<<<
	
    view_dir_mv = normalize(-vpoint_mv.xyz);
    vnormal_mv = normalize((inverse(transpose(MV)) * vec4(vnormal,1.0)).xyz);
    light_dir_mv = normalize((vec4(light_pos,1.0) - vpoint_mv).xyz);

}
