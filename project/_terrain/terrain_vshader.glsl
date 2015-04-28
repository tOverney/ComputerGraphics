#version 330 core

uniform mat4 mvp;
uniform sampler2D height_map;

in vec2 position;
out vec2 uv;
out float height_v;
out vec3 normale;


void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.5;


    float height = texture(height_map, uv).r;

    //calcule la position des 4 points adjacents -> 2 vecteurs -> cross -> vecteur normale

    vec2 uva = (position + vec2(1.0, 0.0)) * 0,5;
    vec2 uvb = (position + vec2(0.0, 1.0))* 0,5;
    vec2 uvb = (position + vec2(2.0, 1.0))* 0,5;
    vec2 uvb = (position + vec2(1.0, 2.0))* 0,5;

    float ha = texture(height_map, uva).r;
    float hb = texture(height_map, uvb).r;
    float hc = texture(height_map, uvc).r;
    float hd = texture(height_map, uvd).r;

    vec3 pos_3d_a = vec3(position.x, ha, -(position.y-1));
    vec3 pos_3d_b = vec3(position.x-1, hb, -position.y);
    vec3 pos_3d_c = vec3(position.x+1, hc, -position.y);
    vec3 pos_3d_d = vec3(position.x, hd, -(position.y+1);

    vec3 ac = pos_3d_b - pos_3d_a;
    vec3 bd = pos_3d_d - pos_3d_b;

    normale = cross(ac, bd);

    height_v = height;

    vec3 pos_3d = vec3(position.x, height, -position.y);

    gl_Position = mvp* vec4(pos_3d, 1.0);
}
