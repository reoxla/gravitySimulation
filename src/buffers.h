#include "glad.h"
class VAO
{
private:
    
public:
    GLuint ID;
    VAO();
    void bind();
    void unbind();
    void attribPointer();
    void enable();
    void disable();
};

class VBO
{
private:
    
public:
    GLuint ID;
    VBO();
    void bind();
    void unbind();
    void bufferData(GLsizeiptr size, const void *data);
    void enable();
    void disable();
};
