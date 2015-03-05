#version 330 core
out vec3 color; ///< output color
in vec3 fcolor;

void main() {
    color = fcolor; /// color is red for every fragment
}
