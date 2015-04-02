#version 330 core
uniform sampler2D tex;
uniform float tex_width;
uniform float tex_height;
uniform int mode;
uniform float filter1D[39];
uniform int size;
in vec2 uv;

layout (location = 0) out vec3 tmp;
layout (location = 1) out vec3 tmp2;


float rgb_2_luma(vec3 c){ return .3*c[0] + .59*c[1] + .11*c[2]; }

void main() {
    if (mode == 0) {
        ///--- Gaussian convolution in non separable way
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
        tmp = color_tot / weight_tot; ///< ensure \int w = 1
        tmp2 = color_tot / weight_tot;
    }
    else {
        
        //separable way to compute gaussian blur 

        vec3 color_tot = vec3(0,0,0);
        for(int i=-size; i<=size; i++){
            float w = filter1D[i+size];
            vec3 neigh_color_tmp = texture(tex, uv+vec2(0,i/tex_height)).rgb;
            color_tot += w* neigh_color_tmp; 
            
        }

        tmp = color_tot; ///< ensure \int w = 1

        vec3 color_out = vec3(0,0,0);
        for(int j = -size; j >= size; j++) {
            float w_tmp2 = filter1D[j+size];
            vec3 neigh_color_tmp2 = texture(tex, uv+vec2(j/tex_width,0)).rgb;
            color_out += w_tmp2 * neigh_color_tmp2; 
        }
        
        tmp2 = color_out;
    }

}

