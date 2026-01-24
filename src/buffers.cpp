#include"buffers.h"
#include"glm/glm.hpp"

VAO::VAO(){
    glGenVertexArrays(1, &ID);
}

void VAO::bind(){
    glBindVertexArray(ID);
}
void VAO::unbind(){
    glBindVertexArray(0);
}
void VAO::attribPointer(){
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}
void VAO::enable(){
    glEnableVertexAttribArray(ID);
}
void VAO::disable(){
    glEnableVertexAttribArray(0);
}

VBO::VBO(){
    glGenBuffers(1, &ID);
}

void VBO::bind(){
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}
void VBO::unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VBO::bufferData(GLsizeiptr size, const void *data){
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
void VBO::enable(){
    glEnableVertexAttribArray(ID);
}
void VBO::disable(){
    glEnableVertexAttribArray(0);
}