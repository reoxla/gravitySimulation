#include <iostream>
#include <vector>
#include "src/glm/glm.hpp"
#include "src/glm/gtc/matrix_transform.hpp"
#include "src/glad.h"
#include "src/buffers.h"
#include "src/shaders.h"
#include <GLFW/glfw3.h>

std::vector<glm::vec3> vertices;

class celestialBody
{
private:
    glm::vec3 velocity; 
    unsigned long long mass;
public:
    celestialBody(unsigned long long mass, glm::vec3 birthVelocity);
    void buildCircle(float radius, int vCount);
};

celestialBody::celestialBody(unsigned long long mass, glm::vec3 birthVelocity)
{
    this->mass = mass;
    velocity = birthVelocity;
}

void celestialBody::buildCircle(float radius, int vCount)
{
    float angle = 360.0f / vCount;

    int triangleCount = vCount - 2;

    std::vector<glm::vec3> temp;
    // positions
    for (int i = 0; i < vCount; i++)
    {
        float currentAngle = angle * i;
        float x = radius * cos(glm::radians(currentAngle));
        float y = radius * sin(glm::radians(currentAngle));
        float z = 0.0f;

        temp.push_back(glm::vec3(x, y, z));
    }
    // triangles
    for (int i = 0; i < triangleCount; i++)
    {
        vertices.push_back(temp[0]);
        vertices.push_back(glm::vec3(0.2f, 0.4f, 0.6f));
        vertices.push_back(temp[i + 1]);
        vertices.push_back(glm::vec3(0.2f, 0.4f, 0.6f));
        vertices.push_back(temp[i + 2]);
        vertices.push_back(glm::vec3(0.2f, 0.4f, 0.6f));
    }
}

void buildCircle(float radius, int vCount);
int main(){
    if (!glfwInit())
    {
        std::cout << "failed init";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(800, 800, "window", NULL, NULL);
    
    if(!window){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "glad failed";
        return -1;
    }

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    celestialBody planet = celestialBody(500, glm::vec3(0.0f));
    planet.buildCircle(0.5f, 20);
    
    Shader shader = Shader("shaders/shader.vs", "shaders/shader.fs");
    shader.use();

    VBO vertexBufferObject = VBO();
    VAO vertexArrayObject = VAO();

    vertexBufferObject.bind();
    vertexArrayObject.bind();
    
    vertexArrayObject.attribPointer(vertexBufferObject);
    vertexBufferObject.bufferData(sizeof(glm::vec3) * vertices.size(), &vertices[0]);
    vertexArrayObject.disable();

    //glm::mat4 translationMatrix;
    vertexArrayObject.enable();
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        
        glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        vertexArrayObject.bind();
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        //glm::translate(translationMatrix, glm::vec3(0.01f));
        //vertices[0]*=translationMatrix;

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

