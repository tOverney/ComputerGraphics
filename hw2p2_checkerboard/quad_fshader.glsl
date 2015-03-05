#version 330 core 

#define PI 3.1415926535897932384626433832795

uniform sampler1D colormap;
in vec2 uv;
out vec3 color;

void main() {
    float angle_adjust = 10 * PI;

    float first_sin = sin(uv[0]*angle_adjust);
    float second_sin = sin(uv[1]*angle_adjust);
    float value = ((first_sin * second_sin) + 1)/2;

    color = texture(colormap, value).rgb;
}


