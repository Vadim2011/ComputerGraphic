
#version 410 core

out vec4 FragColor;

// uniform vec4 frag_colour;
uniform vec3 lightColor;

void main() {
   // FragColor = vec4(0.1f, 0.1f, 0.9f, 1.0f);
   // FragColor = frag_colour;   lightColor
   FragColor = vec4(lightColor, 1.0f);
};
