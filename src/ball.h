#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball 
{
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    glm::vec3 speed;
    glm::vec3 accel;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bounding_box();
    void jump();
private:
    VAO *object;
};

#endif // BALL_H
