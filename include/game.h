#ifndef GAME_H
#define GAME_H
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mShader.h"
#include "block.h"
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
    void createGame(std::string levelFile);
    void drawSprites();
    void processInput(GLFWwindow* window);
private:
    Block* block;
    Board* board;
    Ball* ball;
    int screen_width, screen_height;
    int width, height;
    int blocks[30][30];
    unsigned int blockVAO;
    glm::mat4 model, projection;
    unsigned int createBlockVAO();
};
#endif