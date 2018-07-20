#ifndef GAME_H
#define GAME_H
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mShader.h"
static float vertices[] = {
    -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,
     0.5f, -0.5f, 0.0f, 1.0f,  0.0f,
     0.5f,  0.5f, 0.0f, 1.0f,  1.0f,
     0.5f,  0.5f, 0.0f, 1.0f,  1.0f,
    -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,
    -0.5f, -0.5f, 0.0f, 0.0f,  0.0f
};

class Game {
public:
    Game(int screen_width, int screen_height);
    ~Game();
    void createGame(std::string levelFile);
    void drawBlocks();
private:
    vec2 block_size;
    int screen_width, screen_height;
    int width, height;
    int blocks[30][30];
    unsigned int blockVAO;
    unsigned int block_texture;
    glm::mat4 model, projection;
    MShader* blockShader;
    unsigned int createBlockVAO();
    unsigned int createTexture();
};
#endif