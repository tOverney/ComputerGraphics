#version 330 core

out vec3 color;

in vec2 uv;
in vec3 normal;
in vec3 light_dir;
in float height_v;

uniform sampler2D seamless_snow;
uniform sampler2D seamless_grass;
uniform sampler2D seamless_rock;
uniform sampler2D seamless_sand;

vec3 Id = vec3(1.0f, 1.0f, 1.0f);
vec3 Ia = vec3(0.1f, 0.1f, 0.1f);

vec3 ambient_shading(vec3 in_color) {
    return Ia * in_color;
}

vec3 diffuse_shading(vec3 in_color) {
    float diffuse_angle = max(dot(normal,light_dir),0);
    return Id * in_color * diffuse_angle;
}

vec3 shading(vec3 in_color) {
    return /*ambient_shading(in_color) + */diffuse_shading(in_color);
}

void main() {

    float a_sand = (height_v <= 0.35) ? 1.0 - height_v : 0.0;
    float a_snow = clamp((height_v >= 0.64) ? height_v + 0.2 : 0.0, 0, 1);
    float a_rock = 0;
    a_snow = clamp((height_v >= 0.69) ? a_snow * 1.2 : a_snow, 0, 1);
    float a_grass = clamp(1 - (a_sand + a_snow), 0, 1);



        a_snow = 0.0;
        a_sand = clamp(exp(-pow(height_v,2)/3), 0,1);
        a_rock = clamp(exp(-pow(height_v-6,2)/2),0,1);
        a_grass = 0.0;






    vec3 raw_color = texture(seamless_sand, 100*uv).rgb * a_sand +
                     texture(seamless_snow, 200*uv).rgb * a_snow +
                     texture(seamless_grass, 100*uv).rgb * a_grass+
                     texture(seamless_rock, 100*uv).rgb * a_rock;

    color = /*vec3( height_v, height_v, height_v); /*(normal_mv + vec3(1.0, 1.0, 1.0))/2; //*/shading(raw_color);
}
