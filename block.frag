#version 330 core

in vec2 fTexCoord;
uniform sampler2D blockTexture;
uniform vec3 blockColor;
out vec4 FragColor;

void main() {
    // solid blocks
    if (blockColor.x > 1.2) {
        FragColor = texture(blockTexture, vec2(fTexCoord.x/2+0.5, fTexCoord.y));
    }
    // other blocks
    else {
        FragColor = texture(blockTexture, vec2(fTexCoord.x/2, fTexCoord.y)) * vec4(blockColor, 1.0);
    }
}