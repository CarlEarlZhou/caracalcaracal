#version 330 core

in vec2 fTexCoord;
uniform sampler2D ballTexture;
out vec4 FragColor;

void main() {
    FragColor = texture(ballTexture, fTexCoord);
    if (FragColor.x + FragColor.y + FragColor.z > 2.95) {
        discard;
    }
    // FragColor = vec4(1.0, 0.0, 0.0 ,1.0);
}