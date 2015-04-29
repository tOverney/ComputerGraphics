#version 330 core

uniform mat4 model_view;
uniform mat4 projection;
uniform sampler2D height_map;

in vec2 position;

out vec3 normal_mv;
out vec3 light_dir_mv;
out float height_v;


vec3 light_pos =  vec3(0.0f, 1.0f, 0.01f);
float offset = 0.01;

vec3 compute_normal(vec2 position) {
    vec2 uva = (position + vec2(-offset, -offset));
    vec2 uvb = (position + vec2(-offset, offset));
    vec2 uvc = (position + vec2(offset, offset));
    vec2 uvd = (position + vec2(offset, -offset));

    float ha = texture(height_map, uva).r;
    float hb = texture(height_map, uvb).r;
    float hc = texture(height_map, uvc).r;
    float hd = texture(height_map, uvd).r;

    vec3 pos_3d_a = vec3(position.x, ha, -(position.y-1));
    vec3 pos_3d_b = vec3(position.x-1, hb, -position.y);
    vec3 pos_3d_c = vec3(position.x+1, hc, -position.y);
    vec3 pos_3d_d = vec3(position.x, hd, -(position.y+1));

    vec3 ac = pos_3d_c - pos_3d_a;
    vec3 bd = pos_3d_d - pos_3d_b;

    return normalize((inverse(transpose(model_view)) * vec4(cross(ac, bd),1.0)).xyz);
}

void main() {
    vec2 uv = (position + vec2(1, 1))/2;

    normal_mv = compute_normal(uv);

    float height = texture(height_map, uv).r;
    height_v = height;
    vec3 pos_3d = vec3(position.x, height, -position.y);
    vec4 point_mv = model_view * vec4(pos_3d, 1.0);
    gl_Position = projection * point_mv;
    light_dir_mv = normalize((vec4(light_pos,1.0) - point_mv).xyz);
}
