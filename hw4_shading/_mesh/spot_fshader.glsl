#version 330 core

const float spot_cos_cutoff = 0.985; // cos 10 degrees
const float spot_exp = 150;

out vec3 color;

in vec3 view_dir_mv;
in vec3 vnormal_mv;
in vec3 light_dir_mv;

void main() {    
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
    /// TODO 5: Spot light.
    /// Complete the shader to obtain spot light effect
    ///<<<<<<<<<< TODO <<<<<<<<<<<

    vec3 reflect_dir_mv =normalize(reflect(-light_dir_mv, vnormal_mv));
    
    float specular_angle = max(pow(dot(reflect_dir_mv,view_dir_mv),p),0);
    vec3 specular = Is*ks*pow(specular_angle,p);

    float diffuse_angle = max(dot(vnormal_mv,light_dir_mv),0);
    vec3 diffuse = Id*kd*diffuse_angle;

    vec3 ambient = Ia*ka;


    color = ambient + diffuse + specular;
}
