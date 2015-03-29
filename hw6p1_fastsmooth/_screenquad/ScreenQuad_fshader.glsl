#version 330 core
uniform sampler2D tex;
uniform float tex_width;
uniform float tex_height;
uniform float filter1D[11];
in vec2 uv;
out vec3 color;

float rgb_2_luma(vec3 c){ return .3*c[0] + .59*c[1] + .11*c[2]; }

void main() {

    ///--- Gaussian convolution
    float std = 2; ///< standard deviation (<.3 disable)
    // float std = .1; ///< standard deviation (<.3 disable)
    vec3 color_tot = vec3(0,0,0);
    float weight_tot = 0;
    int SIZE = 1 + 2 * 3 * int( ceil(std) );
    for(int i=-SIZE; i<=SIZE; i++){
        for(int j=-SIZE; j<=SIZE; j++){
            float w = exp(-(i*i+j*j)/(2.0*std*std*std*std));
            vec3 neigh_color = texture(tex, uv+vec2(i/tex_width,j/tex_height)).rgb;
            color_tot += w * neigh_color; 
            weight_tot += w;
        }
    }
    color = color_tot / weight_tot; ///< ensure \int w = 1

}

