#version 330 core

uniform mat4 mvp;

uniform float time;

in vec2 position;
out vec2 uv;

struct wave_conf {
    float freq;
    float phase;
    float amplitude;
    vec2 vec_vit;
};

uniform wave_conf waves[4] = wave_conf[4](
    wave_conf(22.0, 1.0, 0.04, vec2(1.0, 1.0)),
    wave_conf(43.0, 13.0, 0.008, vec2(1.0, -1.0)),
    wave_conf(8.0, 1.0, 0.1, vec2(1.0, 1.0)),
    wave_conf(22.0, 1.0, 0.04, vec2(-1.0, 1.0))
);

float h_computing(int index) {

    float sine = sin(waves[index].freq * 
        dot(uv, waves[index].vec_vit) + waves[index].phase * time);

    // bug with negative values so here is an abs
    return waves[index].amplitude*pow(abs((sine + 1)/2), 2);
}

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.5;

    // Convert the 2D position into 3D positions that all lay in a horizontal
    // plane.
    // TODO 6: Animate the height of the grid points as a sine function of the
    // 'time' and the position ('uv') within the grid.
   
    float height = 0;
    for (int i = 0; i <= 3; i ++ ) {
        height += h_computing(i);
    }

    vec3 pos_3d = vec3(position.x, height, -position.y);

    gl_Position = mvp * vec4(pos_3d, 1.0);
}
