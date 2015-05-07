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
    float diffuse_angle = max(dot(normal, light_dir),0);
    return Id * in_color * diffuse_angle;
}

vec3 shading(vec3 in_color) {
    return diffuse_shading(in_color);
}

float pow4 (float x)
{
    return x*x*x*x;
}


void main() {



        float a_snow = clamp(exp(-pow4((height_v-1)*3)), 0,1);

        float a_rock = clamp(exp(-pow4((height_v-0.7)*4)), 0,1);
        float a_grass = clamp(exp(-pow4((height_v-0.4)*5)), 0,1);
        float a_sand = clamp(exp(-pow4((height_v)*5)), 0,1);

    vec3 raw_color = texture(seamless_sand, 10*uv).rgb * a_sand +
                     texture(seamless_snow, 20*uv).rgb * a_snow +
                     texture(seamless_grass, 10*uv).rgb * a_grass+
                     texture(seamless_rock, 10*uv).rgb * a_rock;

    color = /*vec3( height_v, height_v, height_v); /*(normal_mv + vec3(1.0, 1.0, 1.0))/2; //*/shading(raw_color);

}
