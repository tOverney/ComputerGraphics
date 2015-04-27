#version 330 core
out vec3 color;
in vec2 uv;
uniform sampler2D tex;
uniform float colors[][];

const int GRID_SIZE = 2;
const float[] VECTORS = float[](0, 0.2, 0.4, 0.6);

void main() {
    vec2 cell = uv * GRID_SIZE;
    float color_val = colors[int(cell.x)][int(cell.y)];
    color = vec3(color_val, color_val, color_val);
}
