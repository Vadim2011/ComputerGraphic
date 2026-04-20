
#version 410 core

out vec4 FragColor;

uniform vec4 frag_colour;

void main() {
   // FragColor = vec4(0.1f, 0.1f, 0.9f, 1.0f);
   FragColor = frag_colour;
};
