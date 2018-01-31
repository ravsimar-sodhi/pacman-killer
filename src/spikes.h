#include "main.h"

#ifndef SPIKES_H
#define SPIKES_H


class Spikes
{
    public:
    Spikes() {}
    Spikes(float x, float y, color_t color);
    glm::vec3 position;
    glm::vec3 speed;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bool active;
    bounding_box_t bounding_box();
    private:
    VAO *object;
};

#endif // SPIKES_H
