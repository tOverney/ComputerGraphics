#version 330 core

const int GRID_SIZE = 512;
const int NB_CELLS = GRID_SIZE * GRID_SIZE;

out vec3 color;
in vec2 uv;
uniform sampler2D tex;


float interpolationFunction(float t){
    return t * t * t * (10.0f - t * ( 15.0f - 6.0f * t));
}

float rand(vec2 n) { 
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

/* found at https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83 */
float noise(vec2 p){
    vec2 ip = floor(p);
    vec2 u = fract(p);
    u = u*u*(3.0-2.0*u);

    float res = mix(
        mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
        mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
    return res*res;
}

/* found at https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83 */
float fbm(vec2 x) {
    float v = 0.0;
    float a = 0.5;
    vec2 shift = vec2(100);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50));
    for (int i = 0; i < 5; ++i) {
        v += a * noise(x);
        x = rot * x * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}


void main() {

    float noise = (fbm(5 *uv) + fbm(10 * uv) + fbm(2 * uv))/2;

    color = vec3(noise,noise,noise);
}


