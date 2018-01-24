#include "floor.h"
#include "main.h"

Floor::Floor(float x, float y, float X, float Y, color_t color)
{
    static const GLfloat vertex_buffer_data[] = {
        x, y, 0, // vertex 1
        x,  Y, 0, // vertex 2
        X,  Y, 0, // vertex 3

        X,  Y, 0, // vertex 3
        X, y, 0, // vertex 4
        x, y, 0 // vertex 1
    };
    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}
void Floor::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (0 * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
//    Matrices.model *= translate;
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}
