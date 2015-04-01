#version 330 core
in vec2 uv;
uniform sampler2D colorTex;
uniform sampler2D velocityTex;
out vec4 color;

void main() {
    /// TODO: use the velocity vector stored in velocityTex to compute the line integral
    /// TODO: use a constant number of samples for integral (what happens if you take too few?)
    /// HINT: you can scale the velocity vector to make the motion blur effect more prominent
    /// HINT: to debug integration don't use the velocityTex, simply assume velocity is constant, e.g. vec2(1,0)
    
    vec2 textelSize = vec2(textureSize(colorTex, 0));
    vec2 screenTexCoords = gl_FragCoord.xy / textelSize;

    vec2 velocity = texture(velocityTex, screenTexCoords).rg * 2;

    color = texture(colorTex, screenTexCoords);

    int nSamples = 5;


    for (int i = 1; i < nSamples; ++i) {
        vec2 offset = velocity / float(i);
        color += texture(colorTex, screenTexCoords + offset);
    }

    color /= float(nSamples);

}

