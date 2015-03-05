#version 330 core
in vec3 vpoint;
out vec3 fcolor;
uniform mat4 M;

const vec3 COLORS[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

void main() {
    mat4 T = mat4(1);
    T[3][0] = 0.5;
    T[3][1] = 0.5;

    mat4 S = mat4(1);
    S[0][0] = 0.1;
    S[1][1] = 0.1;

    float alpha = radians(60 /* degrees */);

    mat4 R = mat4(1);
    R[0][0] = cos(alpha);
    R[0][1] = sin(alpha);
    R[1][0] = -sin(alpha);
    R[1][1] = cos(alpha);

    gl_Position = M * vec4(vpoint, 1.0);
    fcolor = COLORS[gl_VertexID];
}
