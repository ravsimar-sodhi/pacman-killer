#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H

class Magnet
{
  public:
    Magnet() {}
    Magnet(float x, float y, color_t color);
    glm::vec3 position;
    glm::vec3 speed;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bounding_box();
    bool active;
  private:
    VAO *object;
    VAO * inner;
};

#endif // MAGNET_H
