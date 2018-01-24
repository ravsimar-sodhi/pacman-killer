#include "main.h"

#ifndef FLOOR_H
#define FLOOR_H


class Floor
{
public:
    Floor() {}
    Floor(float x, float y, float X, float Y, color_t color);
    glm::vec3 position;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // FLOOR_H
