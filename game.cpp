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
    width = 0;
    height = 0;
    projection = ortho(0.0f, (float)screen_width, (float)screen_height, 0.0f, -1.0f, 1.0f);
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
    //generate VAO, texture
    blockVAO = createBlockVAO();
    // create blocks
    block = new Block((float)screen_width/width, (float)screen_height/height/2.5, 
                    "./img/block.png", "./block.vert", "./block.frag", width, height, (int**)blocks);
    board = new Board((float)screen_width/6, 30.0f, "./img/board.png", (float)screen_height,
                    "./block.vert", "./board.frag");
}

void Game::drawSprites() {
    glBindVertexArray(blockVAO);
    block->draw(projection);
    board->draw(projection);
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