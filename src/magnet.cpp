#include "magnet.h"
#include "main.h"
#define PI 3.14159265

Magnet::Magnet(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->active = 0;
    static const int n = 25;
    static GLfloat vertex_buffer_data[n * 9];

    GLfloat angle = 1.57;

    GLfloat radius = 0.5;
    for (int i = 0; i < n * 9; i += 9)
    {
        // The points for the center of the polygon
        vertex_buffer_data[i] = 0;
        vertex_buffer_data[i + 1] = 0;
        vertex_buffer_data[i + 2] = 0;

        GLfloat curr = (PI) / n;

        vertex_buffer_data[i + 3] = (GLfloat)radius * cos(angle);
        vertex_buffer_data[i + 4] = (GLfloat)radius * sin(angle);
        vertex_buffer_data[i + 5] = (GLfloat)0;

        vertex_buffer_data[i + 6] = (GLfloat)radius * cos(angle + curr);
        vertex_buffer_data[i + 7] = (GLfloat)radius * sin(angle + curr);
        vertex_buffer_data[i + 8] = (GLfloat)0;

        angle = angle + curr;
    }
    this->object = create3DObject(GL_TRIANGLES, n * 3, vertex_buffer_data, color, GL_FILL);
}

void Magnet::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Magnet::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Magnet::tick()
{
    this->position.x += this->speed.x;
    this->position.y += this->speed.y;
}

bounding_box_t Magnet::bounding_box()
{
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = {x, y, 1, 1};
    return bbox;
}
