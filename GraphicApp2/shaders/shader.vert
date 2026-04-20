
#version 410 core
layout (location=0) in vec3 vp;

uniform float u_aspect_ratio;

void main() {
   gl_Position = vec4( vp.x * u_aspect_ratio, vp.yz, 1.0);
};
