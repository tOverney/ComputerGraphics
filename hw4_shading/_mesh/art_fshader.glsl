// Copyright (C) 2014 - LGG EPFL
#version 330 core

uniform float p;
uniform sampler2D tex2D;

in vec3 view_dir_mv;
in vec3 vnormal_mv;
in vec3 light_dir_mv;

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
    /// TODO 3.2: Artistic shading.
    /// 1) compute the output color by doing a look-up in the texture using the texture sampler tex.
    ///<<<<<<<<<< TODO <<<<<<<<<<<
	
	vec3 reflect_dir_mv =normalize(reflect(-light_dir_mv, vnormal_mv));
    float specular_angle = max(pow(dot(reflect_dir_mv,view_dir_mv),p),0); 
	
	float diffuse_angle = max(dot(vnormal_mv,light_dir_mv),0);
	color = texture(tex2D, vec2(diffuse_angle, specular_angle)).rgb;
	
}
