#version 330 core

out vec3 color;

in vec3 normal_mv;
in vec3 light_dir_mv;
in float height_v;

uniform sampler2D height_map;

vec3 Id = vec3(1.0f, 1.0f, 1.0f);
vec3 Ia = vec3(1.0f, 1.0f, 1.0f);
vec3 green = vec3(0, 1, 0);
vec3 yellow = vec3(1, 1, 0);
vec3 white = vec3(1, 1, 1);

vec3 ambient_shading(vec3 in_color) {
    return Ia * in_color;
}

vec3 diffuse_shading(vec3 in_color) {
    float diffuse_angle = max(dot(normal_mv,light_dir_mv),0);
    return Id * in_color * diffuse_angle;
}

vec3 shading(vec3 in_color) {
    return /*ambient_shading(in_color) + */ diffuse_shading(in_color);
}

void main() {

    float yellow_amount = (height_v <= 0.4) ? 1 - height_v : 0;
    float white_amount = clamp((height_v >= 0.6) ? height_v * 1.1 : 0, 0, 1);
    float green_amount = 1 - (yellow_amount + white_amount);


    vec3 raw_color = yellow * yellow_amount + white * white_amount + green * green_amount;

    color = shading(raw_color);
}
