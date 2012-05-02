#pragma once

/* Pool Data */

static const GLfloat poolVertexData[8 * 3] = {
    -5.0, -5.0, 0.0,    // 0
    5.0, -5.0, 0.0,     // 1
    -5.0, 5.0, 0.0,     // 2
    5.0, 5.0, 0.0,      // 3
    
    -5.0, 5.0, -10.0,   // 4
    5.0, 5.0, -10.0,    // 5
    -5.0, -5.0, -10.0,  // 6
    5.0, -5.0, -10.0    // 7
};

static const GLuint poolElementData[14] = {
    // Top face
    3, 5, 2, 4,
    
    // Left face
    0, 6,
    
    // Bottom face
    1, 7,
    
    // Right face
    3, 5,
    
    // Back face
    5, 4, 7, 6
};

static const GLfloat poolNormalData[5 * 3] = {
    // Top face
    0.0, 1.0, 0.0,
    
    // Left face
    -1.0, 0.0, 0.0,
    
    // Bottom face
    0.0, -1.0, 0.0,
    
    // Right face
    1.0, 0.0, 0.0,
    
    // Back face
    0.0, 0.0, -1.0
};

static const GLfloat poolColorData[8 * 3] = {
    0.5, 0.5, 0.5,
    0.5, 0.5, 0.5,
    0.5, 0.5, 0.5,
    0.5, 0.5, 0.5,
    
    0.25, 0.25, 0.25,
    0.25, 0.25, 0.25,
    0.25, 0.25, 0.25,
    0.25, 0.25, 0.25
};


/* Photo Data */

static const GLfloat photoVertexData[4 * 3] = {
    -5.0, -5.0, -9.5,
    5.0, -5.0, -9.5,
    -5.0, 5.0, -9.5,
    5.0, 5.0, -9.5
};

static const GLfloat photoTexCoordData[4 * 2] = {
    0.0, 1.0,
    1.0, 1.0,
    0.0, 0.0,
    1.0, 0.0
};
