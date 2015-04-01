#version 330 core
out vec3 color;
in vec2 uv;
uniform sampler2D tex;
uniform sampler2D tex_mirror;


void main() {
    /// TODO: query window_width/height using the textureSize(..) function on tex_mirror
	ivec2 size = textureSize(tex_mirror, 0);

    /// TODO: use gl_FragCoord to build a new [_u,_v] coordinate to query the framebuffer
	
	float _u = gl_FragCoord.x / size.x;
	float _v = 1 - (gl_FragCoord.y / size.y);
    /// NOTE: make sure you normalize gl_FragCoord by window_width/height
    /// NOTE: you will have to flip the "v" coordinate as framebuffer is upside/down
    /// TODO: mix the texture(tex,uv).rgb with the value you fetch by texture(tex_mirror,vec2(_u,_v)).rgb

	color = mix(texture(tex,uv).rgb, texture(tex_mirror,vec2(_u,_v)).rgb, vec3(0.15)) ;
    ///color = texture(tex,uv).rgb;
}
