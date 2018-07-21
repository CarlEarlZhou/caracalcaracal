#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "game.h"
#include "mShader.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "caracal", NULL, NULL);
    if (window == NULL) {
        printf("err create window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("err load glad\n");
        return -1;
    }
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Game test_game(SCREEN_WIDTH, SCREEN_HEIGHT);
    test_game.createGame("./test.in");
    

    while(!glfwWindowShouldClose(window)) {
        test_game.processInput(window);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        test_game.drawSprites();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}