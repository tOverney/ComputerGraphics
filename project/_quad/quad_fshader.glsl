#version 330 core

const int GRID_SIZE = 512;
const int NB_CELLS = GRID_SIZE * GRID_SIZE;

out vec3 color;
in vec2 uv;
uniform sampler2D tex;


float interpolationFunction(float t){
    return t * t * t * (10.0f - t * ( 15.0f - 6.0f * t)); //6.0f * pow(value, 5) - 15.0f * pow(value, 4) + 10 * pow;
}

// float rand(vec2 point) {
//     return fract(sin(dot(point ,vec2(12.9898,78.233))) * 43758.5453);
// }

float rand(vec2 n) { 
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}


float noise(vec2 p){
    vec2 ip = floor(p);
    vec2 u = fract(p);
    u = u*u*(3.0-2.0*u);

    float res = mix(
        mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
        mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
    return res*res;
}

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

    // vec2 pos = 64*uv;
    // // float color_val = colors[int(cell.x)][int(cell.y)];
    // vec2 p00 = vec2(floor(pos.x), floor(pos.y));
    // vec2 p01 = vec2(p00.x, p00.y + 1);
    // vec2 p10 = vec2(p00.x + 1, p00.y);
    // vec2 p11 = vec2(p10.x, p01.y);

    // //Find celle that contains the point
    // vec2 g00 = grand(p00);
    // vec2 g01 = grand(p01);
    // vec2 g10 = grand(p10);
    // vec2 g11 = grand(p11);
    

    // //calculate random gradients
    // // vec2 g00=vec2(permutation[cellRow], permutation[cellColumn])/255;
    // // vec2 g01=vec2(permutation[cellRow+1], permutation[cellColumn])/255;
    // // vec2 g10=vec2(permutation[cellRow], permutation[cellColumn+1])/255;
    // // vec2 g11=vec2(permutation[cellRow+1], permutation[cellColumn+1])/255;


    // // calculate random gradient at cell corners
    // vec2 ap = pos - p00;
    // vec2 bp = pos - p01;
    // vec2 cp = pos - p10;
    // vec2 dp = pos - p11;


    // float s = dot(g00, ap)* 0.5 + 0.5;
    // float t = dot(g10, bp)* 0.5 + 0.5;
    // float u = dot(g01, cp) * 0.5 + 0.5;
    // float v = dot(g11, dp) * 0.5 + 0.5;



    // float st = mix(s,t,interpolationFunction(ap.x));
    // float uv2 = mix(u, v, interpolationFunction(ap.x));
    // float noise = mix(st, uv2, interpolationFunction(ap.y));


    float noise = 1.5 *(fbm(5 *uv) + fbm(10 * uv) + fbm(2 * uv))/3;

    color = vec3(noise,noise,noise);
}


