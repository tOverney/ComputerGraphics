#version 330 core

out vec3 color;

in vec2 uv;
in vec3 normal;
in vec3 light_dir;
in float height_v;

uniform sampler2D tex;

vec3 Id = vec3(1.0f, 1.0f, 1.0f);
vec3 green = vec3(0, 1, 0);
vec3 yellow = vec3(1, 1, 0);
vec3 white = vec3(1, 1, 1);


void main() {

    float yellow_amount = (height_v <= 0.4) ? 1 - height_v : 0;
    float white_amount = clamp((height_v >= 0.6) ? height_v * 1.1 : 0, 0, 1);
    float green_amount = 1 - (yellow_amount + white_amount);


    color = yellow * yellow_amount + white * white_amount + green * green_amount;

    float diffuse_angle = max(dot(normal,light_dir),0);
    vec3 diffuse = Id*diffuse_angle;

    color = normal ;
}
