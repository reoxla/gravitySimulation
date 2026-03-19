#define GLFW_INCLUDE_NONE
#define HoldOrPress (action == GLFW_PRESS || action == GLFW_REPEAT)
#include <iostream>
#include <vector>
#include "src/glad.h"
#include "src/glm/glm.hpp"
#include "src/glm/gtc/matrix_transform.hpp"
#include "src/buffers.h"
#include "src/shaders.h"
#include "celestialBody.h"
#include <GLFW/glfw3.h>

float zoom = 40.0f;
float zoomSpeed = 0.2f;
glm::vec3 cameraPos = glm::vec3(-50.0f, 0.0f, 0.0f);
float cameraSpeed = 0.2f;

void keycallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(){
    if (!glfwInit())
    {
        std::cout << "failed init";
        return -1;
    }
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

	generateCircleTemplate(200, 0.06f);
    std::vector<celestialBody> plantets = {
        celestialBody(50, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(3.0f), 1),
        celestialBody(50, glm::vec3(0.0f), glm::vec3(50.0f, 0.0f, 0.0f), glm::vec3(9.0f), 2)
	};  
       
    Shader shader = Shader("shaders/shader.vs", "shaders/shader.fs");

    shader.use();

    VBO vertexBufferObject = VBO();
    VAO vertexArrayObject = VAO();

    vertexBufferObject.bind();
    vertexArrayObject.bind();
    
    vertexArrayObject.attribPointer(vertexBufferObject);
    vertexBufferObject.bufferData(sizeof(glm::vec3) * circleTemplate.size(), &circleTemplate[0]);
    vertexArrayObject.disable();

    vertexArrayObject.enable();

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
		glfwSetKeyCallback(window, keycallback);

        glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        vertexArrayObject.bind();

        shader.use();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        float currentAspect = (float)display_w / (float)display_h;

        glm::mat4 projectionMatrix = glm::ortho(-zoom * currentAspect, zoom * currentAspect, -zoom, zoom, -1.0f, 1.0f);

		int transformLocation = glGetUniformLocation(shader.ID, "transform");
		if (transformLocation == -1) {
            std::cout << "CRITICAL: Uniform 'transform' not found!" << std::endl;
        }

		glm::mat4 transformationMatrix = glm::mat4(1.0f);

        transformationMatrix = glm::translate(transformationMatrix, plantets[0].position);
        transformationMatrix = glm::translate(transformationMatrix, cameraPos);
        transformationMatrix = glm::scale(transformationMatrix, plantets[0].radius);
        plantets[0].applyGravity(plantets);
        plantets[0].applyVelocity(shader);
		transformationMatrix = projectionMatrix * transformationMatrix;
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, &transformationMatrix[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, circleTemplate.size()/2);


        transformationMatrix = glm::mat4(1.0f);

        transformationMatrix = glm::translate(transformationMatrix, plantets[1].position);
        transformationMatrix = glm::translate(transformationMatrix, cameraPos);
        transformationMatrix = glm::scale(transformationMatrix, plantets[1].radius);
        plantets[1].applyGravity(plantets);
        plantets[1].applyVelocity(shader);
        transformationMatrix = projectionMatrix * transformationMatrix;
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, &transformationMatrix[0][0]);

        for each(celestialBody body in plantets)
        {
            printf("position: %f, %f, %f\n", body.position.x, body.position.y, body.position.z);
            printf("velocity: %f, %f, %f\n", body.velocity.x, body.velocity.y, body.velocity.z);
        }

		glDrawArrays(GL_TRIANGLES, 0, circleTemplate.size() / 2);
        

        glfwSwapBuffers(window);
    }

    glDeleteProgram(shader.ID);
    glfwTerminate();
    return 0;
}

void keycallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_O && HoldOrPress)
        zoom += zoomSpeed;

    if (key == GLFW_KEY_P && HoldOrPress)
        zoom -= zoomSpeed;

	glm::mat4 tempVec = glm::mat4(1.0f);

    if (key == GLFW_KEY_W && HoldOrPress)
        cameraPos -= glm::vec3(0.0f, cameraSpeed, 0.0f);
	if (key == GLFW_KEY_S && HoldOrPress) 
		cameraPos += glm::vec3(0.0f, cameraSpeed, 0.0f);
	if (key == GLFW_KEY_A && HoldOrPress)
		cameraPos += glm::vec3(cameraSpeed, 0.0f, 0.0f);
    if (key == GLFW_KEY_D && HoldOrPress)
		cameraPos -= glm::vec3(cameraSpeed, 0.0f, 0.0f);

    if (key == GLFW_KEY_W && key == GLFW_KEY_D && HoldOrPress)
        cameraPos -= glm::vec3(cameraSpeed, cameraSpeed, 0.0f);
    if (key == GLFW_KEY_W && key == GLFW_KEY_A && HoldOrPress)
        cameraPos += glm::vec3(cameraSpeed, -cameraSpeed, 0.0f);
    if (key == GLFW_KEY_S && key == GLFW_KEY_D && HoldOrPress)
        cameraPos -= glm::vec3(cameraSpeed, -cameraSpeed, 0.0f);
    if (key == GLFW_KEY_S && key == GLFW_KEY_A && HoldOrPress)
        cameraPos += glm::vec3(cameraSpeed, cameraSpeed, 0.0f);

}