#version 330 core

out vec3 color;

in vec2 uv;
//in vec3 normal;
//in vec3 light_dir;
in float height_v;

uniform sampler2D height_map;

vec3 Id = vec3(1.0f, 1.0f, 1.0f);
vec3 green = vec3(0, 1, 0);
vec3 yellow = vec3(1, 1, 0);
vec3 white = vec3(1, 1, 1);

float diffuse() {
    float height0 = texture(height_map, vec2(uv.x, uv.y)).r;
    float height1 = texture(height_map, vec2(uv.x + 1, uv.y)).r;
    float height2 = texture(height_map, vec2(uv.x - 1, uv.y)).r;
    float height3 = texture(height_map, vec2(uv.x, uv.y + 1)).r;
    float height4 = texture(height_map, vec2(uv.x, uv.y - 1)).r;

    vec2 position = uv * 2 - vec2(1.0, 1.0);
    vec3 pos0 = vec3(position.x, height0, -position.y);
    vec3 pos1 = vec3(position.x, height1, -position.y);
    vec3 pos2 = vec3(position.x, height2, -position.y);
    vec3 pos3 = vec3(position.x, height3, -position.y);
    vec3 pos4 = vec3(position.x, height4, -position.y);

    vec3 diffuse = ((pos1 - pos0) + (pos2 - pos0) + (pos3 - pos0) + (pos4 - pos0))/1.5;
    return length(diffuse);
}

void main() {

    float yellow_amount = (height_v <= 0.4) ? 1 - height_v : 0;
    float white_amount = clamp((height_v >= 0.6) ? height_v * 1.1 : 0, 0, 1);
    float green_amount = 1 - (yellow_amount + white_amount);


    color = yellow * yellow_amount + white * white_amount + green * green_amount;

    color = diffuse() * color ;
}
