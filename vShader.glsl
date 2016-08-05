#version 330

layout (location = 0) in vec4 vVertex;
layout (location = 1) in vec4 vColour;

uniform mat4 matModelView;
uniform mat4 matProjection;

out vec4 outColour;

void main(void) {
    
    vec4 eyePos = matModelView * vVertex;
    gl_Position = matProjection * eyePos;
    
    outColour = vColour;
    
    gl_PointSize = 4.5f;
}