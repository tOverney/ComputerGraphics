#version 330 core

out vec3 color;

in vec2 uv;

uniform sampler2D sea;

void main() {



    //color= vec3(0.0, 0.0, 1.0);
    color = texture(sea, uv).rgb;
}
