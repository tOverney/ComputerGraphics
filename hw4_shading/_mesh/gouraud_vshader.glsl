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
    vec3 light_pos_mv = light_pos;
    vec3 vnormal_mv =normalize((inverse(transpose(MV)) * vec4(vnormal, 1.0f)).xyz);
    vec3 light_dir_mv = normalize((vec4(light_pos_mv,1.0f) - vpoint_mv).xyz);
    vec3 view_dir_mv = normalize((-vpoint_mv).xyz);
    

    vec3 reflect_dir_mv =normalize(reflect(-light_dir_mv, vnormal_mv));
    


    
    float diffuse_angle = max(dot(vnormal_mv,light_dir_mv),0.0);
    vec3 diffuse = Id*kd*diffuse_angle;
    
    float specular_angle = max(pow(dot(view_dir_mv,reflect_dir_mv),p),0.0);
    vec3 specular = Is*ks*specular_angle;
    
    vec3 ambient = Ia*ka;
    
    


    vcolor = ambient + diffuse + specular;

}
