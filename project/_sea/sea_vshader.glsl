#version 330 core

uniform mat4 model_view;
uniform mat4 projection;
uniform sampler2D height_map;

in vec2 position;

out vec2 uv;





void main() {
    uv = (position + vec2(1, 1))/2;
    vec3 pos_3d = vec3(position.x, 0, -position.y);
    vec4 point_mv = model_view * vec4(pos_3d, 1.0);
    gl_Position = projection * point_mv;

}
