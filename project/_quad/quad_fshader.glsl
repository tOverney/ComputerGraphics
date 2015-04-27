#version 330 core
out vec3 color;
in vec2 uv;
uniform sampler2D tex;
//uniform float colors[][];

const int GRID_SIZE = 2;
//const float[] VECTORS = float[](0, 0.2, 0.4, 0.6);
const int[] permutation = int[](151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
   );

float interpolationFunction(float value){
    return pow(cos(value),2);
}
float mix(float x, float y, float alpha){
    return (1-alpha)*x+alpha*y;

}

void main() {
    vec2 cell = uv * GRID_SIZE;
   // float color_val = colors[int(cell.x)][int(cell.y)];

    //Find celle that contains the point
    int cellRow = int(cell.y);
    int cellColumn = int(cell.x);

    //calculate random gradients
    vec2 g00=vec2(permutation[cellRow], permutation[cellColumn])/255;
    vec2 g01=vec2(permutation[cellRow+1], permutation[cellColumn])/255;
    vec2 g10=vec2(permutation[cellRow], permutation[cellColumn+1])/255;
    vec2 g11=vec2(permutation[cellRow+1], permutation[cellColumn+1])/255;


    // calculate random gradient at cell corners
    vec2 op = vec2(cell.x-int(cell.x), cell.y-int(cell.y));
    vec2 ap = op;
    vec2 bp = op - vec2(1,0);
    vec2 cp = op - vec2(0,1);
    vec2 dp = op - vec2(1,1);

    float s = dot(g00, ap);
    float t = dot(g10,bp);
    float u = dot(g01, cp);
    float v = dot(g11, dp);



    float st = mix(s,t,interpolationFunction(cell.x));
    float uv2 = mix(u, v, interpolationFunction(cell.x));
    float noise = mix(st, uv2, interpolationFunction(cell.y));


    color = vec3(noise,noise,noise);
}


