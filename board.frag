#version 330 core

in vec2 fTexCoord;
uniform sampler2D boardTexture;
out vec4 FragColor;

void main() {
    FragColor = texture(boardTexture, fTexCoord);
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}