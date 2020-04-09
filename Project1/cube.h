#ifndef __CUBE_H
#define __CUBE_H

// Needed for the vector and glm.
#include <glm/glm.hpp>

#include "glsl.h"
#include "objloader.h"


const GLfloat colors[] = {
    // front colors
    0.0, 0.0, 0.20,
    0.0, 0.0, 0.20,
    0.0, 0.0, 0.20,
    0.0, 0.0, 0.20,
    // back colors
    0.0, 0.0, 0.20,
    0.0, 0.0, 0.20,
    0.0, 0.0, 0.20,
    0.0, 0.0, 0.20,
};

const GLfloat colors2[] = {
    // front colors
    0.20, 0.20, 0.40,
    0.20, 0.20, 0.40,
    0.20, 0.20, 0.40,
    0.20, 0.20, 0.40,
    // back colors
    0.20, 0.20, 0.40,
    0.20, 0.20, 0.40,
    0.20, 0.20, 0.40,
    0.20, 0.20, 0.40,
};



vector<glm::vec3> Create_Cube_Vertices(float width, float height, float depth);

#endif /* __CUBE_H */
