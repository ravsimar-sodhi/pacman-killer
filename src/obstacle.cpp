#include "obstacle.h"
#include "main.h"
#define PI 3.14159265

Obstacle::Obstacle(float x, float y, float r, float speedX, float slope, color_t color) 
{
    this->position = glm::vec3(x, y, 0);
    this->speed = glm::vec3(speedX,0,0);
    this->radius = r;
    static const int n = 25;
    GLfloat vertex_buffer_data[n*9];
    GLfloat angle = 0;
    this->slope = slope;
    // The radius of the circle that will be formed.

    GLfloat radius = r;
    for(int i=0;i<n*9;i+=9)
    {
        // The points for the center of the polygon
        vertex_buffer_data[i] = 0;
        vertex_buffer_data[i+1] = 0;
        vertex_buffer_data[i+2] = 0;

        GLfloat curr = (2*PI)/n;

        vertex_buffer_data[i+3] = (GLfloat) radius * cos(angle);
        vertex_buffer_data[i+4] = (GLfloat) radius * sin(angle);
        vertex_buffer_data[i+5] = (GLfloat) 0;

        vertex_buffer_data[i+6] = (GLfloat) radius * cos(angle+curr);
        vertex_buffer_data[i+7] = (GLfloat) radius * sin(angle+curr);
        vertex_buffer_data[i+8] = (GLfloat) 0;

        angle = angle+curr;
    }
    
    GLfloat slope_vertex_buffer_data[] = {
        -r, 1.2*r, 0, // vertex 1
        r,  1.2*r, 0, // vertex 2
        -r,  0.6*r, 0, // vertex 3

        -r,  0.6*r, 0, // vertex 3
        r,  0.6*r, 0, // vertex 4
        r, 1.2*r, 0, // vertex 2

        };


    this->object = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data, color, GL_FILL);
    this->incline = create3DObject(GL_TRIANGLES, 6, slope_vertex_buffer_data, COLOR_BLACK, GL_FILL);

}

void Obstacle::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (slope * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    if(abs(slope) > 20)
    draw3DObject(this->incline);
}

void Obstacle::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Obstacle::tick() {
    this->position.x += (this->speed).x;
}

bounding_box_t Obstacle::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 2*radius, 2*radius };
    return bbox;
}
