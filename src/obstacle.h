#include "main.h"

#ifndef OBSTACLE_H
#define OBSTACLE_H


class Obstacle
{
public:
    Obstacle() {}
    Obstacle(float x, float y, float radius, float speedX, float slope, color_t color);
    glm::vec3 position;
    glm::vec3 speed;
    float radius;
    float slope;
    int score;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bounding_box();
private:
    VAO* object;
    VAO* incline;
};

#endif // OBSTACLE_H
