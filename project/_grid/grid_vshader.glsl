#version 330 core

uniform mat4 mvp;

in vec2 position;
out vec2 uv;


void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.5;



    vec3 pos_3d = vec3(position.x, 0, -position.y);

    gl_Position = mvp* vec4(pos_3d, 1.0);
}
