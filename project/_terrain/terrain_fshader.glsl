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
vec3 green = vec3(0, 1, 0);
vec3 yellow = vec3(1, 1, 0);
vec3 white = vec3(1, 1, 1);

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

    float yellow_amount = (height_v <= 0.35) ? 1 - height_v : 0;
    float white_amount = clamp((height_v >= 0.64) ? height_v + 0.2 : 0, 0, 1);
    white_amount = (height_v >= 0.69) ? white_amount * 1.2 : white_amount;
    float green_amount = clamp(1 - (yellow_amount + white_amount), 0, 1);


    vec3 raw_color = texture(seamless_sand, 100*uv).rgb * yellow_amount +
                     texture(seamless_snow, 200*uv).rgb * white_amount +
                     texture(seamless_grass, 100*uv).rgb * green_amount;

    color = /*(normal_mv + vec3(1.0, 1.0, 1.0))/2; //*/shading(raw_color);
}
