#ifndef BLOCK_H
#define BLOCK_H

#include "sprite.h"
#include <iostream>

class Block: public Sprite {
public:
    Block(float sizex, float sizey, string texture_path, string vs_path, 
            string fs_path, int width, int height, int** blocks): Sprite(vec2(sizex, sizey)) {
        createTexture(texture_path);
        shader = new MShader(vs_path, fs_path);
        memcpy(this->blocks, blocks, 900*sizeof(int));
        this->width = width;
        this->height = height;
    }
    void draw(mat4 projection) {
        shader->use();
        shader->setMat4fv("projection", projection);
        glBindTexture(GL_TEXTURE_2D, texture);

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
                shader->setVec3fv("blockColor", color);
                shader->setMat4fv("model", tem_model);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
private:
    int width, height;
    int blocks[30][30];
    void createTexture(string texture_path) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        int width, height, channels;
        unsigned char* data = stbi_load(texture_path.c_str(), &width, &height, &channels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }
        else {
            cout << "err load texture\n";
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

class Board: public Sprite {
public:
    Board(float sizex, float sizey, string texture_path, float screen_height,
        float screen_width, string vs_path, string fs_path)
        :Sprite(vec2(sizex, sizey)) {
        createTexture(texture_path);
        width = sizex;
        shader = new MShader(vs_path, fs_path);
        position = 0;
        this->screen_height = screen_height;
        this->screen_width = screen_width;
        mat4 tem_mat = mat4(1.0f);
        tem_mat = translate(tem_mat, vec3(0.0f, float(screen_height - sizey), 0.0f));
        model = tem_mat * model;
    }

    void draw(mat4 projection) {
        shader->use();
        shader->setMat4fv("projection", projection);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader->setMat4fv("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void move(bool direction) {
        float move_speed = 4.0f;
        if (!direction) {
            move_speed *= -1;
        }
        position += move_speed;
        if (position < -0.1 || position + (float)width > screen_width) {
            float new_position = (position < 0) ? 0 : screen_width - width;
            move_speed = move_speed + new_position - position;
            position = new_position;
        }
        mat4 tem_mat = mat4(1.0f);
        tem_mat = translate(tem_mat, vec3(move_speed, 0.0f, 0.0f));
        model = tem_mat * model;
    }
private:
    int width;
    float screen_height, screen_width;
    float position;
    void createTexture(string texture_path) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        int width, height, channels;
        unsigned char* data = stbi_load(texture_path.c_str(), &width, &height, &channels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }
        else {
            cout << "err load texture\n";
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

class Ball: public Sprite {
public:
    Ball(float sizex, float sizey, string texture_path, 
        string vs_path, string fs_path):Sprite(vec2(sizex, sizey)) {

        createTexture(texture_path);
        shader = new MShader(vs_path, fs_path);
        size = 20.0f;
        speed = vec2(0.0f, -1.0f);
        position = vec2(0.0f, 0.0f);
    }

    void draw(mat4 projection) {
        shader->use();
        shader->setMat4fv("projection", projection);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader->setMat4fv("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void setPosition(vec2 position) {
        this->position = position;
    }
private:
    float size;
    vec2 speed;
    vec2 position;
    void createTexture(string texture_path) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        int width, height, channels;
        unsigned char* data = stbi_load(texture_path.c_str(), &width, &height, &channels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }
        else {
            cout << "err load texture\n";
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

#endif