// Copyright (C) 2014 - LGG EPFL
#version 330 core
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

in vec4 vpoint_mv;
in vec3 light_dir, view_dir;

out vec3 color;

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
    /// TODO 4.2: Flat shading.
    /// 1) compute triangle normal using dFdx and dFdy
    /// 1) compute ambient term.
    /// 2) compute diffuse term.
    /// 3) compute specular term.
    ///<<<<<<<<<< TODO <<<<<<<<<<<

    vec3 vpoint_mv_3 = vpoint_mv.xyz;
    vec3 t_x = normalize(dFdx(vpoint_mv_3));
    vec3 t_y = normalize(dFdy(vpoint_mv_3));
    vec3 tri_norm = cross(t_x,t_y);
;

    vec3 reflect_dir_mv =normalize(reflect(-light_dir, tri_norm));
    
    float specular_angle = max(pow(dot(reflect_dir_mv,view_dir),p),0);
    vec3 specular = Is*ks*pow(specular_angle,p);

    float diffuse_angle = max(dot(tri_norm,light_dir),0);
    vec3 diffuse = Id*kd*diffuse_angle;

    vec3 ambient = Ia*ka;

    color = ambient + diffuse + specular;
}
