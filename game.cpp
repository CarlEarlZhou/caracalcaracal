#include "game.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include "stb_image.h"
using namespace std;
using namespace glm;

Game::Game(int screen_width, int screen_height) {
    memset(blocks, 0, sizeof(blocks));
    this->screen_width = screen_width;
    this->screen_height = screen_height;
    block_size = vec2(1.0f, 1.0f);
    width = 0;
    height = 0;
    model = mat4(1.0f);
    block_texture = 0;
    projection = ortho(0.0f, (float)screen_width, (float)screen_height, 0.0f, -1.0f, 1.0f);
}

Game::~Game() {
    delete blockShader;
}

void Game::createGame(string levelFilePath) {
    // read blocks info from file
    fstream levelFile(levelFilePath);
    levelFile >> height >> width;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            levelFile >> blocks[i][j];
        }
    }
    levelFile.close();
    //generate shader, VAO, texture
    blockShader = new MShader("./block.vert", "./block.frag");
    blockVAO = createBlockVAO();
    block_texture = createTexture();
    block_size = vec2((float)screen_width/width, (float)screen_height/height/2.5);
    // move block's left bottom vertex to (0, 0)
    // and scale based on the block number of row and col
    model = scale(model, vec3(block_size.x, block_size.y, 0.0f));
    model = translate(model, vec3(0.5f, 0.5f, 0.0f));
}

void Game::drawBlocks() {
    blockShader->use();
    glBindVertexArray(blockVAO);
    glBindTexture(GL_TEXTURE_2D, block_texture);
    blockShader->setMat4fv("projection", projection);
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            // switch block color
            // solid block's color will be 2.0
            glm::vec3 color = glm::vec3(1.0f);
            if (blocks[j][i] == 2)
                color = glm::vec3(0.2f, 0.6f, 1.0f);
            else if (blocks[j][i] == 3)
                color = glm::vec3(0.0f, 0.7f, 0.0f);
            else if (blocks[j][i] == 4)
                color = glm::vec3(0.8f, 0.8f, 0.4f);
            else if (blocks[j][i] == 5)
                color = glm::vec3(1.0f, 0.5f, 0.0f);
            else if (blocks[j][i] == 0) {
                color = vec3(2.0f);
            }
            mat4 tem_model = translate(model, vec3((float)i, (float)j, 0.0f));
            blockShader->setVec3fv("blockColor", color);
            blockShader->setMat4fv("model", tem_model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
}

unsigned int Game::createBlockVAO() {
    unsigned int VAO, VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
}

unsigned int Game::createTexture() {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, channels;
    unsigned char* data = stbi_load("./img/block.png", &width, &height, &channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else {
        cout << "err load texture\n";
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;

}