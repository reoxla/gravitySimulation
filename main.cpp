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
    glm::vec3 position;
    glm::vec3 testPos;
    unsigned long long mass;
public:
    celestialBody(unsigned long long mass, glm::vec3 birthVelocity, float radius, int vCount, glm::vec3 initialPos);
    void applyVelocity(Shader &shader);
};

celestialBody::celestialBody(unsigned long long mass, glm::vec3 birthVelocity, float radius, int vCount, glm::vec3 initialPos)
{
    this->mass = mass;
    velocity = birthVelocity;
    position = initialPos;

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
        vertices.push_back(position + temp[0]);
        vertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        vertices.push_back(position + temp[i + 1]);
        vertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        vertices.push_back(position + temp[i + 2]);
        vertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    }
}

void celestialBody::applyVelocity(Shader &shader){
    position += velocity;
    int loc = glGetUniformLocation(shader.ID, "offset");
    
    if(loc != -1) {
        glUniform3f(loc, position.x, position.y, position.z);
    } else {
        
        std::cout << "Uniform positionOffset not found!" << std::endl;
    }
}

int main(){
    if (!glfwInit())
    {
        std::cout << "failed init";
        return -1;
    }
    float zoom = 5.0f;
    float aspectRatio = 1.6f;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(1280, 800, "window", NULL, NULL);
    
    if(!window){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "glad failed";
        return -1;
    }

    celestialBody planet1 = celestialBody(500, glm::vec3(0.06f), 0.5f, 100, glm::vec3(0.0f));
    celestialBody planet2 = celestialBody(500, glm::vec3(0.0f), 0.5f, 100, glm::vec3(2.0f, 0.0f, 0.0f));
    
    Shader shader = Shader("shaders/shader.vs", "shaders/shader.fs");
    shader.use();

    VBO vertexBufferObject = VBO();
    VAO vertexArrayObject = VAO();

    vertexBufferObject.bind();
    vertexArrayObject.bind();
    
    vertexArrayObject.attribPointer(vertexBufferObject);
    vertexBufferObject.bufferData(sizeof(glm::vec3) * vertices.size(), &vertices[0]);
    vertexArrayObject.disable();

    vertexArrayObject.enable();

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        
        glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        vertexArrayObject.bind();
        glDrawArrays(GL_TRIANGLES, 0, vertices.size()/2);

        shader.use();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        float currentAspect = (float)display_w / (float)display_h;

        glm::mat4 projectionMatrix = glm::ortho(-zoom * currentAspect, zoom * currentAspect, -zoom, zoom, -1.0f, 1.0f);
        int projLocation = glGetUniformLocation(shader.ID, "projection");
        if(projLocation == -1) {
            std::cout << "CRITICAL: Uniform 'projection' not found!" << std::endl;
        }
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        
        planet1.applyVelocity(shader);
        planet2.applyVelocity(shader);

        glfwSwapBuffers(window);
    }

    glDeleteProgram(shader.ID);
    glfwTerminate();
    return 0;
}

