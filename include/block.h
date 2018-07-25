#ifndef BLOCK_H
#define BLOCK_H

#include "sprite.h"
#include <iostream>

class Ball: public Sprite {
public:
    Ball(float size, float screen_width, float screen_height, string texture_path, 
        string vs_path, string fs_path):Sprite(vec2(size, size), screen_width, screen_height) {

        createTexture(texture_path);
        shader = new MShader(vs_path, fs_path);
        this->size = size;
        speed = vec2(0.1f, -0.01f);
        position = vec2(size/2, size/2);
    }

    void draw(mat4 projection) {
        shader->use();
        shader->setMat4fv("projection", projection);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader->setMat4fv("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void setPosition(float x, float y) {
        mat4 tem_mat = mat4(1.0f);
        tem_mat = translate(tem_mat, vec3(x-position.x, y-position.y, 0.0f));
        model = tem_mat * model;
        this->position = vec2(x, y);
    }

    void move() {
        setPosition(position.x + speed.x, position.y + speed.y);
        if (position.x < size/2 || position.x > screen_width - size/2) {
            speed.x = -speed.x;
            setPosition((position.x<size/2)?size/2:screen_width-size/2, position.y);
        }
        else if (position.y < size/2) {
            speed.y = -speed.y;
            setPosition(position.x, size/2);
        }
    }

    vec2 position;
    vec2 speed;
private:
    float size;
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

class Block: public Sprite {
public:
    Block(float sizex, float sizey, float screen_width, float screen_height,
        string texture_path, string vs_path, string fs_path, int width, 
        int height, int** blocks): Sprite(vec2(sizex, sizey), screen_width, screen_height) {
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
                if (blocks[j][i] == 0) {
                    continue;
                }
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
                else if (blocks[j][i] == 6) {
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

    bool collision(Ball* ball) {
        bool flag = false;
        for (int i = 0; i < height; ++i) {
            if (flag) {
                break;
            }
            for (int j = 0; j < width; ++j) {
                if (blocks[i][j] == 0) {
                    continue;
                }
                vec2 block_center = vec2(j*size.x + size.x/2, i*size.y + size.y/2);
                vec2 diff = ball->position - block_center;
                diff = clamp(diff, vec2(-size.x/2, -size.y), vec2(size.x/2, size.y/2));
                vec2 block_border_point = diff + block_center;
                diff = ball->position - block_border_point;
                // cout << length(diff) << " " << ball->getSize().x << "\n";
                if (length(diff) < ball->getSize().x/2) {
                    flag = true;
                    blocks[i][j] = blocks[i][j] - 1;
                    vec2 new_speed = normalize(reflect(ball->speed, normalize(diff)));
                    cout << i << " " << j << "\n";
                    cout << diff.x << " " << diff.y << "\n";
                    cout << new_speed.x << " " << new_speed.y << "\n";
                    new_speed = new_speed * length(ball->speed);
                    ball->speed = new_speed;
                    diff = normalize(diff) * ball->getSize().x;
                    ball->setPosition(block_border_point.x+diff.x, block_border_point.y+diff.y);
                    if (blocks[i][j] == 5) {
                        blocks[i][j] =6;
                    }
                    break;
                }
            }
        }

        // cout << "\n\n\n\n\n";
        return flag;
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
    Board(float sizex, float sizey, float screen_width, float screen_height, 
        string texture_path, string vs_path, string fs_path)
        :Sprite(vec2(sizex, sizey), screen_width, screen_height) {
        createTexture(texture_path);
        width = sizex;
        shader = new MShader(vs_path, fs_path);
        position = 0;
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

    bool collision(Ball* ball) {
        if (ball->position.y + ball->getSize().x/2 > screen_height - 30) {
            if (ball->position.x > position && ball->position.x < position + width) {
                float reflect_x = (ball->position.x-position-width/2)/width/1.5;
                vec2 new_speed = reflect(ball->speed, vec2(reflect_x, -1.0f));
                ball->speed = normalize(new_speed) * length(ball->speed);
                // cout << reflect_x << "\n";
                // cout << ball->speed.x << " " << ball->speed.y << "\n";
                // cout << "\n\n\n";
                ball->setPosition(ball->position.x, screen_height - 30 - ball->getSize().x);
				return true;
            }
            
        }
		return false;
    }
private:
    int width;
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

#endif