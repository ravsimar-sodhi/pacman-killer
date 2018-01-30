#include "main.h"

#ifndef POND_H
#define POND_H

class Pond
{
  public:
    Pond() {}
    Pond(float x, float y, float r);
    glm::vec3 position;
    float radius;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bounding_box();
  private:
    VAO *object;
};

#endif // BALL_H
