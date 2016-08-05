#version 330

uniform sampler2D tex;

in vec4 outColour;
out vec4 vFragColour;

void main(void) {
    
    vFragColour = texture(tex, gl_PointCoord) * outColour;
}