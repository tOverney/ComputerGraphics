#version 330 core
out vec3 color;
in vec2 uv;
in float height_v;
uniform sampler2D tex;

void main() {
    vec3 green = vec3(0, 1, 0);
    vec3 yellow = vec3(1, 1, 0);
    vec3 white = vec3(1, 1, 1);
    float yellow_amount = (height_v <= 0.4) ? 1 - height_v : 0;
    float white_amount = clamp((height_v >= 0.6) ? height_v * 1.1 : 0, 0, 1);
    float green_amount = 1 - (yellow_amount + white_amount);


    color = yellow * yellow_amount + white * white_amount + green * green_amount;
}
