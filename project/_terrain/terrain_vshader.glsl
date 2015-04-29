#version 330 core

uniform mat4 model_view;
uniform mat4 projection;
uniform sampler2D height_map;

in vec2 position;

out vec3 normal;
out vec3 light_dir;
out float height_v;


vec3 light_pos =  vec3(0.0f, 0.5f, 0.5f);
float offset = 1.0/512;

vec3 compute_normal(vec2 uv, float h) {

    vec2 uva = (uv + vec2(offset, 0));
    vec2 uvb = (uv + vec2(0, offset));

    float ha = texture(height_map, uva).r;
    float hb = texture(height_map, uvb).r;

    vec3 pos_3d_p = vec3(uv.x, h, -uv.y);
    vec3 pos_3d_a = vec3(uva.x, ha, -uva.y);
    vec3 pos_3d_b = vec3(uvb.x, hb, -uvb.y);

    vec3 pa = pos_3d_a - pos_3d_p;
    vec3 pb = pos_3d_b - pos_3d_p;

    return normalize(cross(pa, pb)); //normalize((inverse(transpose(model_view)) * vec4(cross(pa, pb),1.0)).xyz);
}

void main() {
    vec2 uv = (position + vec2(1, 1))/2;
    float height = texture(height_map, uv).r;

    normal = compute_normal(uv, height);

    height_v = height;
    vec3 pos_3d = vec3(position.x, height, -position.y);
    vec4 point_mv = model_view * vec4(pos_3d, 1.0);
    gl_Position = projection * point_mv;
    light_dir = normalize(light_pos);
}
