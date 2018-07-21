#ifndef SPRITE_H
#define SPRITE_H

#include "mShader.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

using namespace std;
using namespace glm;

class Sprite {
public:
    Sprite(vec2 size, float screen_width, float screen_height) {
        // move sprite's left bottom vertex to (0, 0) and scale 
        model = mat4(1.0f);
        model = scale(model, vec3(size.x, size.y, 0.0f));
        model = translate(model, vec3(0.5f, 0.5f, 0.0f));

        this->size = size;
        this->screen_width = screen_width;
        this->screen_height = screen_height;
    }

    vec2 getSize() {
        return size;
    }
    virtual void draw(mat4) = 0;
    MShader* shader;
protected:
    unsigned int texture;
    float screen_height, screen_width;
    vec2 size;
    mat4 model;
};
#endif